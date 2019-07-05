#include "cmdmgr_cmd.h"

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

	if(stat == CMD_BUFFER_OK)
		printf("Success!\n");
	else
		printf("BUFFER FULL\n");

	return (stat == CMD_BUFFER_OK) ? STATUS_OK : STATUS_FAIL;
}
