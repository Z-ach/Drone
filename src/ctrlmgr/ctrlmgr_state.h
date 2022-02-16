#ifndef DRONE_CTRLMGR_STATE_H
#define DRONE_CTRLMGR_STATE_H

#include <pthread.h>
#include <stdatomic.h>

#include <cmdmgr/cmdmgr_pub.h>


/*	State Information		*/
typedef enum RunStatus{
    RUNNING = 0,
    STOP = 1
} RunStatus;

typedef enum CommandInfo{
    NO_COMMANDS_QUEUED = 0,
    WAIT_FOR_FINISH = 1,
    PREEMPT_REQUESTED = 2,
} CommandInfo;

typedef struct State{
    Command *current_cmd;
    Command *next_cmd;
    _Atomic(CommandInfo) command_info;
    _Atomic(RunStatus) run_status;
    _Atomic(RunStatus) netmgr_status;
    _Atomic(RunStatus) cmdmgr_status;
} State;

typedef struct SharedStatus{
    State *state;
    pthread_mutex_t *lock;
    pthread_cond_t *buffer_cond;
    pthread_cond_t *command_cond;
} SharedStatus;


#endif //DRONE_CTRLMGR_STATE_H
