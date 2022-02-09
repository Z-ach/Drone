#include "cmdmgr_cmd.h"

void msg_to_uint32(char *msg, uint32_t *cmd){
	sscanf(msg, "%"SCNu32, cmd);
	LOG_CMD("msg val: %s\n", msg);
	LOG_CMD("cmd val: 0x%16X\n", *cmd);
}

OperationStatus handoff_recv_cmd(char *msg){
	Command actual_cmd;
	uint32_t cmd;
	msg_to_uint32(msg, &cmd);
	OperationStatus parse_result = parse_cmd(&actual_cmd, cmd);

	if(parse_result != STATUS_OK){
		return parse_result;
	}

	LOG_CMD("Attempting to insert into buffer\n");
	BufferStatus stat = insert_into_cmd_buf(actual_cmd);

	if(stat == CMD_BUFFER_OK){
		LOG_CMD("Success!\n");
	}
	else{
		LOG_CMD("BUFFER FULL\n");
	}

	return (stat == CMD_BUFFER_OK) ? STATUS_OK : STATUS_FAIL;
}

OperationStatus parse_cmd(Command *actual_cmd, uint32_t raw_cmd){
	//All fields that will be extracted
	uint8_t counter;
	uint8_t mode;
	uint16_t params;

	//First, validate CRC8
	if(crc8(raw_cmd) != STATUS_OK){
		//Bad crc8, something went wrong in transmission
		LOG_CMD("CRC8 validation failed. Could not process command\n");
		return STATUS_FAIL;
	}

	//Next, extract counter, mode, and params
	counter =	(raw_cmd & CMD_COUNT_MASK) >> 28;
	mode = 		(raw_cmd & CMD_MODE_MASK) >> 24;
	params =	(raw_cmd & CMD_PARAM_MASK) >> 8;

	//Assign proper mode and process params (TBD)
	actual_cmd->mode = mode;
	switch(mode){
		case TAKE_OFF:
			actual_cmd->params.TakeOff.altitude = params >> 8;
			LOG_CMD("Registered cmd as TAKEOFF\n\tParams:\t0x%04X\n", params);
			LOG_CMD("\tProceeding to altitude: %d inches\n", actual_cmd->params.TakeOff.altitude );
			break;
		case LAND:
			actual_cmd->params.Land.emergency = params & PARAM_EMER_MASK;
			actual_cmd->params.Land.location = (params & PARAM_LOC_MASK) >> 8;
			LOG_CMD("Registered cmd as LAND\n\tParams:\t0x%04X\n", params);
			LOG_CMD("\tEmergency flag: %d\tLanding location: %d\n", actual_cmd->params.Land.emergency, actual_cmd->params.Land.location);
			//ensure that it is not an emergency landing request
			break;
		case HOVER:
			//process
			LOG_CMD("Registered cmd as HOVER\n\tParams:\t0x%04X\n", params);
			break;
		case PATROL:
			//unimplemented
			LOG_CMD("Registered cmd as PATROL\n\tParams:\t0x%04X\n", params);
			break;
		default:
			//No valid mode sent
			fprintf(stderr, "INVALID COMMAND MODE DETECTED\n");
			//Insert log message
			return STATUS_FAIL;
	}

	actual_cmd->status = STATUS_WAITING;
	actual_cmd->counter = counter;

	return STATUS_OK;
}

// Create an emergency landing command
void emergency_landing(Command *cmd){

	//Command *cmd = malloc(sizeof(*cmd));
	cmd->counter = -1;
	cmd->mode = LAND;
	//cmd->params = EMERGENCY_ON,
	cmd->status = STATUS_WAITING;
}
