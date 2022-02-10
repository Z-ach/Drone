#ifndef NETMGR_H
#define NETMGR_H

#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <ctrlmgr/ctrlmgr_state.h>
#include <ctrlmgr/ctrlmgr_hw.h>
#include <logmgr/logmgr.h>
#include "cmdmgr/cmdmgr_cmd.h"


typedef struct server{
    uint32_t listen_fd;
} server_t;

typedef enum {
    NET_DISPATCH_MASK = 0xFFFF0000
} NetMask;

typedef enum{
    RECV_BUF_SIZE = 50,
    RESP_BUF_SIZE = 50,
} NetBufferSizes;

void *net_handler(void *status);
int create_socket(server_t *socket_fd, int timeout);
int bind_socket(server_t *socket_fd, struct sockaddr_in *server);
void msg_to_uint32(char *msg, uint32_t *cmd);


# endif