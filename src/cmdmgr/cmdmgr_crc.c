#include "cmdmgr_crc.h"

OperationStatus crc8(uint32_t cmd){
	uint8_t data[4];
	OperationStatus crc_status = STATUS_OK;
	OperationStatus segment_success;

	segment_success = segment_bytes(data, cmd);
	if(segment_success != STATUS_OK){
		LOG_CMD("Unable to segment command data. Invalid?\n");
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
