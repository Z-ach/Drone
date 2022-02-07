#include "ctrlmgr_ctrl.h"

pthread_t cmdmgr_thread;
pthread_t netmgr_thread;

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t buffer_cond = PTHREAD_COND_INITIALIZER;
static pthread_cond_t command_cond = PTHREAD_COND_INITIALIZER;


void init_shared_status(SharedStatus *status){
	// Create shared status
	status->state = malloc(sizeof(*status->state));
	status->state->current_cmd = malloc(sizeof(*status->state->current_cmd));
	status->state->current_cmd->counter = 0;
	status->state->current_cmd->mode = NO_OP;
	status->state->current_cmd->status = STATUS_FINISHED;
	status->state->run_status = RUNNING;

	status->lock = &lock;	
	status->buffer_cond = &buffer_cond;
	status->command_cond = &command_cond;
}


void init(SharedStatus *status){
	init_shared_status(status);
	// Initialize command buffer
	init_cmd_buf();
}

void start(SharedStatus *status){
	printf("Starting threads.\n");
	// Start the command handler
	pthread_create(&cmdmgr_thread, NULL, &cmd_handler, status);
	sleep(1);
	// Start network manager
	pthread_create(&netmgr_thread, NULL, &net_handler, NULL);
}

void test(SharedStatus *status){
	uint32_t cmds[] = {
		0x01100000, //0 = COUNTER, 1 = TAKEOFF, 1000 = 16 INCHES, 00 = CRC
		0x13C00000, //1 = COUNTER, 3 = HOVER, C000 = IN PLACE HOVER, 00 = CRC
		0x22031100  //2 = COUNTER, 2 = LAND, 03 = LOCATION 3, 11 = EMERGENCY, 00 = CRC
	};

	for(int i = 0; i < 3; i++){
		printf("Handing off command: %d\n", i);
		pthread_mutex_lock(status->lock);
		printf("Buffer mutex acquired.\n");
		handoff_recv_cmd(cmds[i]);
		printf("Triggering buffer condition signal.\n");
		pthread_cond_signal(status->buffer_cond);
		pthread_mutex_unlock(status->lock);
		printf("Buffer mutex released.\n");
		sleep(1);
		printf("Marking command as complete.\n");
		pthread_mutex_lock(status->lock);
		status->state->current_cmd->status = STATUS_FINISHED;
		printf("Triggering command condition signal.\n");
		pthread_cond_signal(status->command_cond);
		pthread_mutex_unlock(status->lock);
	}

	/*
	handoff_recv_cmd(cmds[0]);
	handoff_recv_cmd(cmds[1]);
	handoff_recv_cmd(cmds[2]);
	*/
}

void end(SharedStatus *status){
	pthread_join(netmgr_thread, NULL);
	pthread_join(cmdmgr_thread, NULL);
	free(status->state);
	free(status);
}

int main(int argc, char *argv[]){
	printf("running main\n");
	SharedStatus *status = malloc(sizeof(SharedStatus));
	init(status);
	start(status);
	test(status);
	//end();
}
