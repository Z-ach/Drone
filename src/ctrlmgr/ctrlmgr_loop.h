#ifndef DRONE_CTRLMGR_LOOP_H
#define DRONE_CTRLMGR_LOOP_H

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "ctrlmgr_hw.h"
#include "ctrlmgr_state.h"
#include "ctrlmgr_stat.h"
#include <cmdmgr/cmdmgr_pub.h>
#include <logmgr/logmgr.h>

void ctrl_loop_run(SharedStatus *status);
OperationStatus dispatch_cmd(SharedStatus *status);
CommandStatus exc_takeoff(Parameters params);
CommandStatus exc_landing(Parameters params);
CommandStatus exc_hover(Parameters params);
CommandStatus exc_patrol(Parameters params);

#endif //DRONE_CTRLMGR_LOOP_H
