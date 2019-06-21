#ifndef CTRLMGR_CTRL_
#define CTRLMGR_CTRL_

#include <stdio.h>
#include <pthread.h>

#include "ctrlmgr_stat.h"

#ifndef CMDMGR_CMD_
#include "../cmdmgr/cmdmgr_cmd.h"
#endif


/*	State Information		*/
typedef struct State{
	Command *current_cmd;
} State;

typedef struct SharedStatus{
	State *state;
	pthread_mutex_t *lock;
} SharedStatus;


/*	Functions				*/
//Checks to see if current command has changed
OperationStatus check_state_cmd();


/*	Assertion-Like Helpers	*/
//OperationStatus assert_equalilty(void *var1, void *var2, Equality equality);


#endif
