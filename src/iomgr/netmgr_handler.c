#include "netmgr_handler.h"

int recv_timeout = 30;

int create_socket(server_t *socket_fd, int timeout){
    int err;
    struct timeval tv;
    tv.tv_sec = timeout;
    tv.tv_usec = 0;
    err = (socket_fd->listen_fd = socket(AF_INET, SOCK_STREAM, 0));
    setsockopt(socket_fd->listen_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    LOG_IO("set timeout to %d\n", timeout);
    return err;
}

int bind_socket(server_t *socket_fd, struct sockaddr_in *server){
    int err;
    server->sin_addr.s_addr = INADDR_ANY;
    server->sin_family = AF_INET;
    server->sin_port = htons(8888);
    err = bind(socket_fd->listen_fd, (struct sockaddr*)server, sizeof(*server));
    return err;
}

void *net_handler(void *shared_status){
    LOG_IO("Network handler successfully initialized.\n");
    int err, c, read_size, sig_req;
    int running = 1;
    char ack_message[] = "Acknowledged.";
    char client_message[RECV_BUF_SIZE] = {0};
	char resp_buf[RESP_BUF_SIZE] = {0};
    server_t server_socket, client_socket;
    struct sockaddr_in server, client;
    struct timeval tv;
    tv.tv_sec = recv_timeout;
    tv.tv_usec = 0;
    SharedStatus *status = shared_status;
    OperationStatus stat;

    err = create_socket(&server_socket, recv_timeout);
    if(err < 0){
        LOG_IO("Unable to create socket.\n");
        //pthread_mutex_lock(status->lock);
        status->state->run_status = STOP;
        //pthread_mutex_unlock(status->lock);
        return NULL;
    }

    err = bind_socket(&server_socket, &server);
    if(err < 0){
        LOG_IO("Unable to establish connection.\n");
        //pthread_mutex_lock(status->lock);
        status->state->run_status = STOP;
        //pthread_mutex_unlock(status->lock);
        return NULL;
    }
    listen(server_socket.listen_fd, 3);

	c = sizeof(struct sockaddr_in);
    client_socket.listen_fd = accept(server_socket.listen_fd, (struct sockaddr *)&client, (socklen_t*)&c);
    setsockopt(client_socket.listen_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    LOG_IO("Connection accepted.\n");
    while(running){

        read_size = recv(client_socket.listen_fd, client_message, 2000, 0);
        if(read_size <= 10 && read_size > 0){
            client_message[read_size] = '\0';
            LOG_IO("recv: %s (%d bytes)\n", client_message, read_size);
            pthread_mutex_lock(status->lock);
			sig_req = dispatch_recv_msg(client_message, resp_buf);
            if(sig_req){
                pthread_cond_signal(status->buffer_cond);
            }
            pthread_mutex_unlock(status->lock);
            write(client_socket.listen_fd, resp_buf, strlen(resp_buf));
        } else if(read_size > 10){
            LOG_IO("Received more than 10 bytes, ignoring message.\n");
        } else if(read_size == 0){
            LOG_IO("Client disconnected.\n");
            running = 0;
        } else if(read_size < 0){
            LOG_IO("Timeout exceeded.\n");
            running = 0;
        }

        if(status->state->run_status == STOP){
            running = 0;
        }
        if(!running){
            //pthread_mutex_lock(status->lock);
            status->state->netmgr_status = STOP;
            //pthread_mutex_unlock(status->lock);
            break;
        }
    }
    close(server_socket.listen_fd);
    LOG_IO("Server shutdown successful.\n");
    return NULL;
}

void msg_to_uint32(char *msg, uint32_t *cmd){
	sscanf(msg, "%"SCNu32, cmd);
	LOG_IO("cvt msg %s to val: 0x%08X\n", msg, *cmd);
}

int check_mask(uint32_t cmd, uint32_t mask, int shift){
    return (cmd & mask) >> shift == (mask >> shift);
}

double determine_cfg_req(uint32_t cmd){
    // figure out the new value to set within the cfg
    uint32_t raw_val = (cmd & NET_CFG_VAL_MASK);
    return (double)raw_val;
}

// Send received message to proper location
int dispatch_recv_msg(char *client_message, char *resp){
    int signal_req = 0;
    uint32_t cmd;
	msg_to_uint32(client_message, &cmd);

    if(check_mask(cmd, NET_TELEM_MASK, 16)){
        LOG_IO("Request for telemetry received.\n");
		telem_to_resp(resp, RESP_BUF_SIZE);
	}else if(check_mask(cmd, NET_READ_CFG_MASK, 16)){
        LOG_IO("Request for config received.\n");
		cfg_to_resp(resp, RESP_BUF_SIZE);
	}else if(check_mask(cmd, NET_SET_THR_MASK, 16)){
        LOG_IO("Request for thr update received.\n");
        update_cfg_from_net(thr, determine_cfg_req(cmd));
	}else if(check_mask(cmd, NET_SET_KD_MASK, 16)){
        LOG_IO("Request for kD update received.\n");
        update_cfg_from_net(kD, determine_cfg_req(cmd));
	}else if(check_mask(cmd, NET_SET_KI_MASK, 16)){
        LOG_IO("Request for kI update received.\n");
        update_cfg_from_net(kI, determine_cfg_req(cmd));
	}else if(check_mask(cmd, NET_SET_KP_MASK, 16)){
        LOG_IO("Request for kP update received.\n");
        update_cfg_from_net(kP, determine_cfg_req(cmd));
    }else{
        LOG_IO("Command received.\n");
		handoff_recv_cmd(cmd);
        memset(resp, 0, RESP_BUF_SIZE);
        snprintf(resp, RESP_BUF_SIZE, "cmd:Command acknowledegd.");
        resp[RESP_BUF_SIZE-1] = '\0';
        signal_req = 1;
	}
    return signal_req;
}