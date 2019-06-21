#include "cmdmgr_cmd.h"

#ifndef CMDMGR_CMD_
#define CMDMGR_CMD_


typedef struct CommandNode{
	struct CommandNode *next;
	Command command;

} CommandNode;

typedef enum {
	CMD_BUFFER_OK		= 0x00,
	CMD_BUFFER_FULL		= 0x01,
	CMD_BUFFER_EMPTY	= 0x02,
	CMD_BUFFER_ERR		= 0xFF
} BufferStatus;


/*	Functions	*/
OperationStatus init_cmd_buf(void);
BufferStatus insert_into_cmd_buf(Command cmd);
BufferStatus fetch_next_cmd(Command *cmd);


typedef enum {
	CMD_BUF_MAX = 10
} Constants;

#endif
