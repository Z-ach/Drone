#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../ctrlmgr/ctrlmgr_stat.h"

#ifndef CMDMGR_CMD_
#define CMDMGR_CMD_

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
	struct TakeOff{
		uint8_t altitude;
	} TakeOff;

	struct Land{
		uint8_t location;
		uint8_t emergency;
	} Land;

	struct Hover{
		uint8_t location;
		uint8_t maintain;
	} Hover;

	struct Patrol{
		uint8_t location_a;
		uint8_t location_b;
	} Patrol;
} Parameters;

typedef enum {
	STATUS_WAITING		= 0x00,
	STATUS_EXECUTING	= 0x01,
	STATUS_FINISHED		= 0x02,
	STATUS_FAILED		= 0xFF
} CommandStatus;

typedef struct {
	uint8_t counter;
	CommandMode mode;
	Parameters params;
	CommandStatus status;
} Command;


/*	Functions	*/
OperationStatus handoff_recv_cmd(uint32_t cmd);
OperationStatus parse_cmd(Command *cmd, uint32_t cmd_raw);

void execute_emergency_landing(void);



/*	Useful Values	*/
typedef enum {
	CMD_COUNT_MASK	= (uint32_t)0xF0000000,
	CMD_MODE_MASK	= 0x0F000000,
	CMD_PARAM_MASK	= 0x00FFFF00,
	CMD_CRC_MASK	= 0x000000FF
} CmdMasks;

typedef enum {
	PARAM_ALT_MASK	= 0xFF,
	PARAM_EMER_MASK	= 0x03,
	PARAM_LOC_MASK	= 0xFC,
	PARAM_MNTN_MASK	= 0x03
} ParamMasks;




#endif
