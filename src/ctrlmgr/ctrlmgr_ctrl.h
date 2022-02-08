#ifndef CTRLMGR_CTRL_H
#define CTRLMGR_CTRL_H

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "ctrlmgr_stat.h"
#include "ctrlmgr_state.h"
#include "ctrlmgr_loop.h"
#include "ctrlmgr_hw.h"
#include <cmdmgr/cmdmgr_cmd.h>
#include <cmdmgr/cmdmgr_handler.h>
#include <cmdmgr/cmdmgr_buffer.h>
#include <netmgr/netmgr_handler.h>


/*	Functions				*/
//Checks to see if current command has changed
OperationStatus check_state_cmd();
void init();
void start();



#endif
