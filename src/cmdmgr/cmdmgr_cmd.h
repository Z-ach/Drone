#ifndef CMDMGR_CMD_H
#define CMDMGR_CMD_H


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <ctrlmgr/ctrlmgr_stat.h>

#include "cmdmgr_pub.h"
#include "cmdmgr_crc.h"


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
