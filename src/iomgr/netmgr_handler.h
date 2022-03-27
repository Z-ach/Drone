#ifndef NETMGR_H
#define NETMGR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <ctrlmgr/ctrlmgr_state.h>
#include <ctrlmgr/ctrlmgr_hw.h>
#include <iomgr/logmgr.h>
#include "cmdmgr/cmdmgr_cmd.h"


typedef struct server{
    uint32_t listen_fd;
} server_t;

typedef enum {
    NET_READ_CFG_MASK        = (uint32_t)0xFFF00000,
    NET_SET_THR_MASK         = (uint32_t)0xFFF10000,
    NET_SET_ROLL_KP_MASK     = (uint32_t)0xFFF20000,
    NET_SET_ROLL_KI_MASK     = (uint32_t)0xFFF30000,
    NET_SET_ROLL_KD_MASK     = (uint32_t)0xFFF40000,
    NET_SET_PITCH_KP_MASK    = (uint32_t)0xFFF50000,
    NET_SET_PITCH_KI_MASK    = (uint32_t)0xFFF60000,
    NET_SET_PITCH_KD_MASK    = (uint32_t)0xFFF70000,
    NET_SET_YAW_KP_MASK      = (uint32_t)0xFFF80000,
    NET_SET_YAW_KI_MASK      = (uint32_t)0xFFF90000,
    NET_SET_YAW_KD_MASK      = (uint32_t)0xFFFA0000,
    NET_TELEM_MASK           = (uint32_t)0xFFFF0000,
    NET_CFG_VAL_MASK         = (uint32_t)0x0000FFFF
} NetMask;

typedef enum{
    RECV_BUF_SIZE = 2048,
    RESP_BUF_SIZE = 2048,
} NetBufferSizes;

void *net_handler(void *status);
int create_socket(server_t *socket_fd, int timeout);
int bind_socket(server_t *socket_fd, struct sockaddr_in *server);
void msg_to_uint32(char *msg, uint32_t *cmd);
int dispatch_recv_msg(char *client_message, char *resp);
int check_mask(uint32_t cmd, uint32_t mask, int shift);
double determine_cfg_req(uint32_t cmd);


# endif