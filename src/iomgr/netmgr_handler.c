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
    int err, c, read_size, sig_req, new_read;
    int running = 1;
    int expected_read = 8;
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
        memset(client_message, 0, 16);
        read_size = recv(client_socket.listen_fd, client_message, expected_read, 0);
        while(read_size > 0 && read_size < expected_read){
            LOG_IO("recv: %.*s (%d bytes) - less than required read. waiting for rest of message.\n", read_size, client_message, read_size);
            new_read = recv(client_socket.listen_fd, client_message+read_size, expected_read-read_size, 0);
            // catch disconnect / error while waiting for rest of message
            if(new_read <= 0){
                read_size = new_read;
                break;
            }
            read_size += new_read;
        }

        if(read_size <= 10 && read_size > 0){
            client_message[read_size] = '\0';
            //LOG_IO("recv: %s (%d bytes)\n", client_message, read_size);
            pthread_mutex_lock(status->lock);
            sig_req = dispatch_recv_msg(client_message, resp_buf);
            if(sig_req){
                LOG_IO("Tripping buffer, sig_req was %d.\n", sig_req);
                pthread_cond_signal(status->buffer_cond);
            }
            pthread_mutex_unlock(status->lock);
            write(client_socket.listen_fd, resp_buf, strlen(resp_buf));
        } else if(read_size > 10){
            LOG_IO("Received more than 8 bytes, ignoring message. (Got %d bytes).\n", read_size);
        } else if(read_size == 0){
            LOG_IO("Client disconnected.\n");
            running = 0;
        } else if(read_size < 0){
            LOG_IO("Client disconnected or timeout exceeded.\n");
            running = 0;
        }

        if(status->state->run_status == STOP){
            running = 0;
        }
        if(!running){
            pthread_mutex_lock(status->lock);
            // signal command manager to indicate no more commands will be queued
            // that way, if buf is empty, cmd mgr can end
            status->state->netmgr_status = STOP;
            pthread_cond_signal(status->buffer_cond);
            pthread_mutex_unlock(status->lock);
            break;
        }
    }
    close(server_socket.listen_fd);
    LOG_IO("Server shutdown successful.\n");
    return NULL;
}

void msg_to_uint32(char *msg, uint32_t *cmd){
    sscanf(msg, "%08X", cmd);
    LOG_IO_L(LOG_DBG, "cvt msg %s to val: 0x%08X\n", msg, *cmd);
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
    OperationStatus stat;
    int signal_req = 0;
    int write_len = 0;
    int dbg_level = 0;
    uint32_t cmd;
    static uint32_t last_cmd = 0;
    msg_to_uint32(client_message, &cmd);

    // If we are repeating the same command, don't output
    if(cmd == last_cmd){
        dbg_level = LOG_DBG;
    }
    last_cmd = cmd;

    if(check_mask(cmd, NET_TELEM_MASK, 16)){
        LOG_IO_L(dbg_level,"Request for telemetry received.\n");
        write_len = telem_to_resp(resp, RESP_BUF_SIZE);
    }else if(check_mask(cmd, NET_SET_YAW_KD_MASK, 16)){
        LOG_IO_L(dbg_level,"Request for kD update received.\n");
        update_cfg_from_net(yaw_kD, determine_cfg_req(cmd));
    }else if(check_mask(cmd, NET_SET_YAW_KI_MASK, 16)){
        LOG_IO_L(dbg_level,"Request for kI update received.\n");
        update_cfg_from_net(yaw_kI, determine_cfg_req(cmd));
    }else if(check_mask(cmd, NET_SET_YAW_KP_MASK, 16)){
        LOG_IO_L(dbg_level,"Request for kP update received.\n");
        update_cfg_from_net(yaw_kP, determine_cfg_req(cmd));
    }else if(check_mask(cmd, NET_SET_PITCH_KD_MASK, 16)){
        LOG_IO_L(dbg_level,"Request for kD update received.\n");
        update_cfg_from_net(pitch_kD, determine_cfg_req(cmd));
    }else if(check_mask(cmd, NET_SET_PITCH_KI_MASK, 16)){
        LOG_IO_L(dbg_level,"Request for kI update received.\n");
        update_cfg_from_net(pitch_kI, determine_cfg_req(cmd));
    }else if(check_mask(cmd, NET_SET_PITCH_KP_MASK, 16)){
        LOG_IO_L(dbg_level,"Request for kP update received.\n");
        update_cfg_from_net(pitch_kP, determine_cfg_req(cmd));
    }else if(check_mask(cmd, NET_SET_ROLL_KD_MASK, 16)){
        LOG_IO_L(dbg_level,"Request for kD update received.\n");
        update_cfg_from_net(roll_kD, determine_cfg_req(cmd));
    }else if(check_mask(cmd, NET_SET_ROLL_KI_MASK, 16)){
        LOG_IO_L(dbg_level,"Request for kI update received.\n");
        update_cfg_from_net(roll_kI, determine_cfg_req(cmd));
    }else if(check_mask(cmd, NET_SET_ROLL_KP_MASK, 16)){
        LOG_IO_L(dbg_level,"Request for kP update received.\n");
        update_cfg_from_net(roll_kP, determine_cfg_req(cmd));
    }else if(check_mask(cmd, NET_READ_CFG_MASK, 16)){
        LOG_IO_L(dbg_level,"Request for config received.\n");
        write_len = cfg_to_resp(resp, RESP_BUF_SIZE);
    }else if(check_mask(cmd, NET_SET_THR_MASK, 16)){
        LOG_IO_L(dbg_level,"Request for thr update received.\n");
        update_cfg_from_net(thr, determine_cfg_req(cmd));
    }else{
        LOG_IO("Command received.\n");
        stat = handoff_recv_cmd(cmd);
        memset(resp, 0, RESP_BUF_SIZE);
        write_len = snprintf(resp, RESP_BUF_SIZE, "cmd:Command acknowledged.");
        if(stat == STATUS_OK){
            signal_req = 1;
        }
    }
    if(write_len > 0){
        // add delimiter for easy processing
        resp[write_len] = ';';
    }
    resp[RESP_BUF_SIZE-1] = '\0';
    return signal_req;
}
