#include "cmdmgr_cmd.h"

static CommandBuffer cmd_buf[10];

OperationStatus init_cmd_buf(void){
	OperationStatus status = STATUS_OK;

	return status;
}

OperationStatus handoff_recv_cmd(uint32_t cmd){
	
	//All fields that will be extracted
	uint8_t counter;
	uint8_t mode;
	uint16_t params;
	uint8_t crc8;

	Command actual_cmd;
	CommandStatus cmd_stat = STATUS_WAITING;
	CommandMode cmd_mode;


	//First, validate CRC8
	crc8 = 		cmd & CMD_CRC_MASK; 
	//TODO implement crc8, then verify
	//if crc is invalid, return status fail

	//Next, extract counter, mode, and params
	counter =	(cmd & CMD_COUNT_MASK) >> 28;
	mode = 		(cmd & CMD_MODE_MASK) >> 24;
	params =	(cmd & CMD_PARAM_MASK) >> 8;

	//Assign proper mode and process params (TBD)
	switch(mode){
		case TAKE_OFF:

			break;
		case LAND:
			//process
			break;
		case HOVER:
			//process
			break;
		case PATROL:
			//unimplemented
			break;
		default:
			//No valid mode sent
			//Insert log message
			return STATUS_FAIL;
	}

}
