#ifndef CMDMGR_HANDLER_H
#define CMDMGR_HANDLER_H

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#include <ctrlmgr/ctrlmgr_state.h>
#include "cmdmgr_buffer.h"
#include "cmdmgr_cmd.h"

void *cmd_handler();

#endif