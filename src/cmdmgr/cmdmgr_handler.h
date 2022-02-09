#ifndef CMDMGR_HANDLER_H
#define CMDMGR_HANDLER_H

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#include <ctrlmgr/ctrlmgr_state.h>
#include <logmgr/logmgr.h>
#include "cmdmgr_buffer.h"
#include "cmdmgr_cmd.h"

typedef enum {
	NEXT_BUFFER = 0,
	FORCE_LAND = 1
} StateUpdateMethod;

void *cmd_handler();
BufferStatus update_state_from_buffer(SharedStatus *status, StateUpdateMethod update_method);

#endif