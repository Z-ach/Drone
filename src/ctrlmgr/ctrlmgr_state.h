#ifndef DRONE_CTRLMGR_STATE_H
#define DRONE_CTRLMGR_STATE_H

#include <pthread.h>

#include <cmdmgr/cmdmgr_pub.h>


/*	State Information		*/
typedef enum RunStatus{
    RUNNING = 0,
    STOP = 1
} RunStatus;

typedef struct State{
    Command *current_cmd;
    RunStatus run_status;
} State;

typedef struct SharedStatus{
    State *state;
    pthread_mutex_t *lock;
    pthread_cond_t *buffer_cond;
    pthread_cond_t *command_cond;
} SharedStatus;


#endif //DRONE_CTRLMGR_STATE_H
