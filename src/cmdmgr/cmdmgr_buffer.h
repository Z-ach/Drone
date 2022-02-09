#ifndef CMDMGR_BUFFER_H
#define CMDMGR_BUFFER_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <ctrlmgr/ctrlmgr_stat.h>
#include <logmgr/logmgr.h>

#include "cmdmgr_pub.h"

/*	Functions	*/
OperationStatus init_cmd_buf(void);
BufferStatus insert_into_cmd_buf(Command cmd);
BufferStatus fetch_next_cmd(Command *cmd);


#endif
