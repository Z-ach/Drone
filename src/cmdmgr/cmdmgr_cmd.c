#include "cmdmgr_cmd.h"

static CommandNode *cmd_node_head;
static uint8_t cmd_buf_size;

OperationStatus init_cmd_buf(void){
	OperationStatus status = STATUS_OK;
	cmd_buf_size = 0;
	return status;
}

OperationStatus handoff_recv_cmd(uint32_t cmd){
	
	//All fields that will be extracted
	uint8_t counter;
	uint8_t mode;
	uint16_t params;

	Command actual_cmd;

	//First, validate CRC8
	if(crc8(cmd) != STATUS_OK){
		//Bad crc8, something went wrong in transmission
		printf("CRC8 validation failed. Could not process command\n");
		return STATUS_FAIL;
	}

	//Next, extract counter, mode, and params
	counter =	(cmd & CMD_COUNT_MASK) >> 28;
	mode = 		(cmd & CMD_MODE_MASK) >> 24;
	params =	(cmd & CMD_PARAM_MASK) >> 8;

	//Assign proper mode and process params (TBD)
	actual_cmd.mode = mode;
	switch(mode){
		case TAKE_OFF:
			printf("Registered cmd as TAKEOFF\n\tParams:\t0x%04X\n", params);
			break;
		case LAND:
			//process
			printf("Registered cmd as LAND\n\tParams:\t0x%04X\n", params);
			//ensure that it is not an emergency landing request
			break;
		case HOVER:
			//process
			printf("Registered cmd as HOVER\n\tParams:\t0x%04X\n", params);
			break;
		case PATROL:
			//unimplemented
			printf("Registered cmd as PATROL\n\tParams:\t0x%04X\n", params);
			break;
		default:
			//No valid mode sent
			fprintf(stderr, "INVALID COMMAND MODE DETECTED\n");
			//Insert log message
			return STATUS_FAIL;
	}

	actual_cmd.status = STATUS_WAITING;
	actual_cmd.counter = counter;

	printf("Attempting to insert into buffer\n");
	BufferStatus stat = insert_into_cmd_buf(actual_cmd);
	printf("Success!\n");

	return (stat == CMD_BUFFER_OK) ? STATUS_OK : STATUS_FAIL;
}

OperationStatus crc8(uint32_t cmd){
	uint8_t data[4];
	OperationStatus crc_status = STATUS_OK; 
	OperationStatus segment_success;

	segment_success = segment_bytes(data, cmd);
	if(segment_success != STATUS_OK){
		printf("Unable to segment command data. Invalid?\n");
		crc_status = STATUS_FAIL;
	}else{
		for(int i = 0; i < 4; i++){
        	;//crc = crc8x_table[crc ^ *data++];		
        }
	}
	return crc_status;
}

OperationStatus segment_bytes(uint8_t *buffer, uint32_t cmd){
	OperationStatus segment_status = STATUS_OK;
	for(int i = 0; i < 4; i++){
		buffer[i] = cmd & 0xFF;
		cmd >>= 8;
	}
	return segment_status;
}

BufferStatus insert_into_cmd_buf(Command cmd){
	//Ensure there is room in the buffer
	if(cmd_buf_size >= CMD_BUF_MAX)
		return CMD_BUFFER_FULL;
	
	CommandNode *new_cmd = malloc(sizeof(CommandNode));
	new_cmd->command = cmd;
	new_cmd->next = NULL;

	//Check if buffer is empty
	if(cmd_buf_size == 0){
		cmd_node_head = new_cmd;
	}else{
		CommandNode *iter = cmd_node_head;
		while(iter->next != NULL){
			iter = iter->next;
		}
		iter->next = new_cmd;
	}
	cmd_buf_size++;
	return CMD_BUFFER_OK;
}

BufferStatus fetch_next_cmd(Command *cmd){
	if(cmd_buf_size == 0) return CMD_BUFFER_EMPTY;
	
	//fetch command at head
	*cmd = cmd_node_head->command; 
	
	//store next so head can be freed
	CommandNode *next = cmd_node_head->next;
	free(cmd_node_head);

	//move head to next
	cmd_node_head = next;

	printf("count:\t%d\nmode:\t%d\n", cmd->counter, cmd->mode);
	cmd_buf_size--;
	return CMD_BUFFER_OK;
}

