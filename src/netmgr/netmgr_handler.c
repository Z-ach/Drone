#include "netmgr_handler.h"

int recv_timeout = 10;

int create_socket(server_t *socket_fd, int timeout){
    int err;
    struct timeval tv;
    tv.tv_sec = timeout;
    tv.tv_usec = 0;
    err = (socket_fd->listen_fd = socket(AF_INET, SOCK_STREAM, 0));
    setsockopt(socket_fd->listen_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    LOG_NET("set timeout to %d\n", timeout);
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
    LOG_NET("Network handler successfully initialized.\n");
    int err, c, read_size;
    int running = 1;
    char ack_message[] = "Acknowledged.";
    char client_message[50];
    server_t server_socket, client_socket;
    struct sockaddr_in server, client;
    struct timeval tv;
    tv.tv_sec = recv_timeout;
    tv.tv_usec = 0;
    SharedStatus *status = shared_status;
    OperationStatus stat;

    err = create_socket(&server_socket, recv_timeout);
    if(err < 0){
        LOG_NET("Unable to create socket.\n");
        pthread_mutex_lock(status->lock);
        status->state->run_status = STOP;
        pthread_mutex_unlock(status->lock);
        return NULL;
    }

    err = bind_socket(&server_socket, &server);
    if(err < 0){
        LOG_NET("Unable to establish connection.\n");
        pthread_mutex_lock(status->lock);
        status->state->run_status = STOP;
        pthread_mutex_unlock(status->lock);
        return NULL;
    }
    listen(server_socket.listen_fd, 3);

	c = sizeof(struct sockaddr_in);
    client_socket.listen_fd = accept(server_socket.listen_fd, (struct sockaddr *)&client, (socklen_t*)&c);
    setsockopt(client_socket.listen_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    LOG_NET("Connection accepted.\n");
    while(running){
        pthread_mutex_lock(status->lock);

        read_size = recv(client_socket.listen_fd, client_message, 2000, 0);
        if(read_size > 0){
            client_message[read_size] = '\0';
            LOG_NET("recv: %s (%d bytes)\n", client_message, read_size);
            stat = handoff_recv_cmd(client_message);
            pthread_cond_signal(status->buffer_cond);
            write(client_socket.listen_fd, ack_message, strlen(ack_message));
        } else if(read_size == 0){
            LOG_NET("Client disconnected.\n");
            running = 0;
        } else if(read_size < 0){
            LOG_NET("Timeout exceeded.\n");
            running = 0;
        }

        if(status->state->run_status == STOP){
            running = 0;
        }
        if(!running){
            status->state->netmgr_status = STOP;
            pthread_mutex_unlock(status->lock);
            break;
        }
        pthread_mutex_unlock(status->lock);
    }
    close(server_socket.listen_fd);
    LOG_NET("Server shutdown successful.\n");
    return NULL;
}