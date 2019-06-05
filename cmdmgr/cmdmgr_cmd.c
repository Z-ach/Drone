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

	//First, validate CRC8
	crc8 = 		cmd & CMD_CRC_MASK; 
	//TODO implement crc8, then verify

	//Next, extract counter, mode, and params
	counter =	(cmd & CMD_COUNT_MASK) >> 28;
	mode = 		(cmd & CMD_MODE_MASK) >> 24;
	params =	(cmd & CMD_PARAM_MASK) >> 8;

}
