#include "ctrlmgr_ctrl.h"

SharedStatus status; //contains state and mutex lock

void init(){
	status.state = NULL;
	pthread_mutex_init(status.lock, NULL);
}

void start(){
	//pthread_t cmd_thread;

	//pthread_create(&cmd_thread, NULL, &cmd_handler, NULL);
}




void test(){
	init_cmd_buf();

	uint32_t cmds[] = {
		0x000F0000, //0 = COUNTER, 0 = TAKEOFF, 0F00 = 16 INCHES, 00 = CRC
		0x12C00000, //1 = COUNTER, 2 = HOVER, C000 = IN PLACE HOVER, 00 = CRC
		0x21000000  //2 = COUNTER, 1 = LAND, 0000 = NON EMERGENCY LOCATION 0, 00 = CRC
	};

	for (int i = 0; i < 15; i++){
		printf("\n\ncount=%d\n", i);
		handoff_recv_cmd(cmds[2]);
		printf("\n\n");
	}

	Command cur_cmd;
	for(int i = 0; i < 15; i++){
		printf("\n\ncount=%d\n", i);
		BufferStatus stat = fetch_next_cmd(&cur_cmd);
		if(stat == CMD_BUFFER_OK) printf("recv cmd: 0x%0X\n", cur_cmd.mode);
		else printf("nothing in buffer");
		printf("\n\n");
	}

}

int main(int argc, char *argv[]){
	init();
	start();
	test();
}