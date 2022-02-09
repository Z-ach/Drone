#ifndef NETMGR_H
#define NETMGR_H

#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <ctrlmgr/ctrlmgr_state.h>
#include <logmgr/logmgr.h>
#include "cmdmgr/cmdmgr_cmd.h"


typedef struct server{
    uint32_t listen_fd;
} server_t;

void *net_handler(void *status);
int create_socket(server_t *socket_fd, int timeout);
int bind_socket(server_t *socket_fd, struct sockaddr_in *server);

//char IP_ADDRESS[] = "127.0.0.1";

# endif