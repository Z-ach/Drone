#include "cmdmgr_handler.h"

typedef enum {
	NEXT_BUFFER = 0,
	FORCE_LAND = 1
} StateUpdateMethod;

BufferStatus update_state_from_buffer(SharedStatus *status, StateUpdateMethod update_method);

// Use void * then cast to allow usage with pthreads
void *cmd_handler(void *shared_status){
    printf("Command handler successfully initialized.\n");
	SharedStatus *status = shared_status;
	printf("Found initial command: %d\n", status->state->current_cmd->status);

	uint8_t max_iter = 10;
	while(max_iter-- > 5){
		pthread_mutex_lock(status->lock);
		printf("Command state mutex acquired.\n");
		printf("Command status: %d\n", status->state->current_cmd->status);
		switch(status->state->current_cmd->status){
			case STATUS_EXECUTING:
				// Can't start new command until current one finishes
				printf("Command currently executing. Waiting for completion.\n");
				pthread_cond_wait(status->command_cond, status->lock);
				break;
			case STATUS_FAILED:
				// Probably want to land if this happens
				update_state_from_buffer(status, FORCE_LAND);
				break;
			case STATUS_FINISHED:
				// Safe to execute next command
				update_state_from_buffer(status, NEXT_BUFFER);
				printf("back, checking state: %d\n", status->state->current_cmd->status);
				break;
		}
		if(status->state->run_status != RUNNING){
			pthread_mutex_unlock(status->lock);
			return NULL;
		}
		printf("Command state mutex released.\n");
		pthread_mutex_unlock(status->lock);
	}
	return NULL;
}

BufferStatus update_state_from_buffer(SharedStatus *status, StateUpdateMethod update_method){
    printf("Updating state from buffer\n");
	Command *next_command = malloc(sizeof(*next_command));
	BufferStatus stat = CMD_BUFFER_OK;

	if(update_method == NEXT_BUFFER){
		stat = fetch_next_cmd(next_command);
		if(stat == CMD_BUFFER_EMPTY){
			printf("Buffer is empty. Waiting for signal.\n");
			pthread_cond_wait(status->buffer_cond, status->lock);
			stat = fetch_next_cmd(next_command);
		}
	} else{
		emergency_landing(next_command);
	}

	status->state->current_cmd = next_command;
	printf("State update successful.\n");
	printf("\tcounter: %d\n", next_command->counter);
	printf("\tmode: %d\n", next_command->mode);
	printf("\tstatus: %d\n\n", next_command->status);
	next_command->status = STATUS_EXECUTING; // TODO REMOVE

	return stat;
}

void dispatch_command(Command *cmd){
	cmd->status = STATUS_EXECUTING;
	switch(cmd->mode){
		case TAKE_OFF:
			// Not implemented
			break;
		case LAND:
			// Not implemented
			break;
		case HOVER:
			// Not implemented
			break;
		case PATROL:
			// Not implemented
			break;
	}
}
