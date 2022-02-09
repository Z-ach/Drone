#include "cmdmgr_handler.h"

// Use void * then cast to allow usage with pthreads
void *cmd_handler(void *shared_status){
    LOG_CMD("Command handler successfully initialized.\n");
	SharedStatus *status = shared_status;
	LOG_CMD("Found initial command: %d\n", status->state->current_cmd->status);

	while(1){
		pthread_mutex_lock(status->lock);
		//LOG_CMD("Command state mutex acquired.\n");
		LOG_CMD("Command status: %d\n", status->state->current_cmd->status);
		switch(status->state->current_cmd->status){
			case STATUS_EXECUTING:
			case STATUS_WAITING:
				// Can't start new command until current one finishes
				LOG_CMD("Command currently executing or waiting. Pausing for completion.\n");
				update_state_from_buffer(status, NEXT_BUFFER);
				//LOG_CMD("Next command has been set.\n");
				pthread_cond_wait(status->command_cond, status->lock);
				LOG_CMD("Received command condition signal.\n");
				LOG_CMD("Command status after waiting is now: %d\n", status->state->current_cmd->status);
				break;
			case STATUS_FAILED:
				// Probably want to land if this happens
				update_state_from_buffer(status, FORCE_LAND);
				break;
			case STATUS_FINISHED:
				// Safe to execute next command
				if(status->state->next_cmd != NULL){
					LOG_CMD("Next command already set, updating.\n");
				}else{
					update_state_from_buffer(status, NEXT_BUFFER);
					LOG_CMD("back, checking state: %d\n", status->state->current_cmd->status);
				}
				LOG_CMD("Freeing cmd, setting cur to next\n");
				free(status->state->current_cmd);
				status->state->current_cmd = status->state->next_cmd;
				status->state->next_cmd = NULL;
				break;
		}
		if(status->state->run_status != RUNNING){
			LOG_CMD("Cmd handler service has stopped.\n");
			pthread_mutex_unlock(status->lock);
			return NULL;
		}
		//LOG_CMD("Command state mutex released.\n");
		pthread_mutex_unlock(status->lock);
	}
	return NULL;
}

BufferStatus update_state_from_buffer(SharedStatus *status, StateUpdateMethod update_method){
    LOG_CMD("Updating state from buffer\n");
	Command *next_command = malloc(sizeof(*next_command));
	BufferStatus stat = CMD_BUFFER_OK;

	if(update_method == NEXT_BUFFER){
		stat = fetch_next_cmd(next_command);
		if(stat == CMD_BUFFER_EMPTY){
			LOG_CMD("Buffer is empty. Waiting for signal.\n");
			pthread_cond_wait(status->buffer_cond, status->lock);
			stat = fetch_next_cmd(next_command);
			LOG_CMD("Buffer condition tripped, resuming.\n");
		}
	} else{
		emergency_landing(next_command);
	}

	//free(status->state->current_cmd);
	status->state->next_cmd = next_command;
	LOG_CMD("State update successful.\n");
	LOG_CMD("\tcounter: %d\n", next_command->counter);
	LOG_CMD("\tmode: %d\n", next_command->mode);
	LOG_CMD("\tstatus: %d\n\n", next_command->status);

	return stat;
}
