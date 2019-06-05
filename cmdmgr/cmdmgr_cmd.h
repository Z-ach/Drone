#pragma once

#include <stdio.h>
#include <stdint.h>

#include "../ctrlmgr/ctrlmgr_ctrl.h"

/*	Command Modes	*/
typedef enum {
	TAKE_OFF	=	0x00,
	LAND		=	0x01,
	HOVER		=	0x02,
	PATROL		=	0x03
} CommandMode;

/*	Parameters		*/
typedef enum {
	LOCATION_STAY	=	0x00
} Location;

typedef enum {
	EMERGENCY_OFF	= 0x00,
	EMERGENCY_ON	= 0x02
} Emergency;


/*	Data Types		*/
typedef struct{
	uint8_t inches;
} Altitude;

typedef enum {
	STATUS_WAITING		= 0x00,
	STATUS_EXECUTING	= 0x01,
	STATUS_FINISHED		= 0x02,
	STATUS_FAILED		= 0xFF
} CommandStatus;

typedef struct {
	uint8_t counter;
	CommandMode mode;
	uint16_t parameters;
	CommandStatus status;
} Command;

typedef struct CommandBuffer{
	struct CommandBuffer *next;
	Command command;

} CommandBuffer;

typedef enum {
	CMD_BUFFER_OK		= 0x00,
	CMD_BUFFER_FULL		= 0x01,
	CMD_BUFFER_EMPTY	= 0x02,
	CMD_BUFFER_ERR		= 0xFF
} BufferStatus;

/*	Functions	*/
OperationStatus init_cmd_buf(void);
OperationStatus handoff_recv_cmd(uint32_t cmd);
OperationStatus parse_cmd(Command *cmd, uint32_t cmd_raw);
BufferStatus insert_into_cmd_buf(Command cmd);
BufferStatus fetch_next_cmd(Command *cmd);


/*	Useful Values	*/
typedef enum {
	CMD_COUNT_MASK	= (uint32_t)0xF0000000,
	CMD_MODE_MASK	= 0x0F000000,
	CMD_PARAM_MASK	= 0x00FFFF00,
	CMD_CRC_MASK	= 0x000000FF
} CmdMasks;
