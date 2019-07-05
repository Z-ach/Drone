#ifndef DRONE_CTRLMGR_STATE_H
#define DRONE_CTRLMGR_STATE_H

#include <pthread.h>

#include <cmdmgr/cmdmgr_pub.h>


/*	State Information		*/
typedef struct State{
    Command *current_cmd;
} State;

typedef struct SharedStatus{
    State *state;
    pthread_mutex_t *lock;
} SharedStatus;


#endif //DRONE_CTRLMGR_STATE_H
