#ifndef CMDMGR_CMD_H
#define CMDMGR_CMD_H


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#include <ctrlmgr/ctrlmgr_stat.h>
#include <logmgr/logmgr.h>

#include "cmdmgr_pub.h"
#include "cmdmgr_crc.h"
#include "cmdmgr_buffer.h"


/*	Functions	*/
OperationStatus handoff_recv_cmd(char *cmd);
OperationStatus parse_cmd(Command *cmd, uint32_t cmd_raw);

void emergency_landing(Command *cmd);
void execute_emergency_landing(void);
const char *get_cmd_mode_name(CommandMode mode);
const char *get_cmd_status_name(CommandStatus status);



/*	Useful Values	*/
typedef enum {
	CMD_COUNT_MASK	= (uint32_t)0xF0000000,
	CMD_MODE_MASK	= 0x0F000000,
	CMD_PARAM_MASK	= 0x00FFFF00,
	CMD_CRC_MASK	= 0x000000FF
} CmdMasks;

typedef enum {
	PARAM_ALT_MASK	= 0xFF00,
	PARAM_EMER_MASK	= 0x0003,
	PARAM_LOC_MASK	= 0xFF00,
	PARAM_MNTN_MASK	= 0x0300
} ParamMasks;




#endif
