#include "ctrlmgr_loop.h"

FILE *fp;

void ctrl_loop_run(SharedStatus *status){
    int running = 1;
    OperationStatus stat;
    fp = fopen("test.txt", "w");
    while(running){
        stat = dispatch_cmd(status);
        pthread_mutex_lock(status->lock);
		if(status->state->run_status != RUNNING){
            running = 0;
        }
        pthread_mutex_unlock(status->lock);
    }
    fclose(fp);
}

OperationStatus dispatch_cmd(SharedStatus *status){
    Command *cur_cmd;
    CommandStatus stat;
    int keep_running = 1;
    //Issue: command status set back to EXECUTING before cmdmgr thread starts again
    pthread_mutex_lock(status->lock);
    cur_cmd = status->state->current_cmd;
    // If the current command has been set to finished, and another command
    // is available (via next_cmd), then start executing the next command.
    if (status->state->current_cmd->status == STATUS_FINISHED && status->state->next_cmd != NULL){
        cur_cmd = status->state->next_cmd;
        printf("Next command was available, using that one!\n");
    }else if(status->state->current_cmd->status == STATUS_FINISHED){
        //printf("Current command is set to finished, but no next_cmd available.\n");
    }else{
        //printf("State not finished, next cmd may be available.\n");
    }


    cur_cmd->status = STATUS_EXECUTING;
    pthread_mutex_unlock(status->lock);

    switch(status->state->current_cmd->mode){
        case NO_OP:
            stat = STATUS_FINISHED;
            break;
        case TAKE_OFF:
            stat = exc_takeoff(cur_cmd->params);
            break;
        case LAND:
            stat = exc_landing(cur_cmd->params);
            keep_running = 0;
            break;
        case HOVER:
            stat = exc_hover(cur_cmd->params);
            break;
        case PATROL:
            stat = exc_patrol(cur_cmd->params);
            break;
    }
    pthread_mutex_lock(status->lock);
    cur_cmd->status = stat;
    //printf("ctrl loop: cmd status updated to %d\n", stat);
    if(!keep_running){
        status->state->run_status = STOP;
        printf("Ctrl loop service has stopped.\n");
        // Signal buffer just in case cmd handler is waiting on empty buf
        pthread_cond_signal(status->buffer_cond);
    }
    pthread_cond_signal(status->command_cond);
    pthread_mutex_unlock(status->lock);

    return (stat == STATUS_FINISHED) ? STATUS_OK : STATUS_FAIL;
}

CommandStatus exc_takeoff(Parameters params){
    fprintf(fp, "EXECUTING COMMAND: TakeOff\n");
    fprintf(fp, "\tRequested altitude: %d inches.\n", params.TakeOff.altitude);
    printf("EXECUTING COMMAND: TakeOff\n");
    printf("\tRequested altitude: %d inches.\n", params.TakeOff.altitude);
    enable_leds();
    //sleep(2);
    return STATUS_FINISHED;
}

CommandStatus exc_landing(Parameters params){
    fprintf(fp, "EXECUTING COMMAND: Landing\n");
    fprintf(fp, "\tRequested location: %d\tEmergency flag: %d.\n", params.Land.location, params.Land.emergency);
    printf("EXECUTING COMMAND: Landing\n");
    printf("\tRequested location: %d\tEmergency flag: %d.\n", params.Land.location, params.Land.emergency);
    //sleep(2);
    return STATUS_FINISHED;

}

CommandStatus exc_hover(Parameters params){
    fprintf(fp, "EXECUTING COMMAND: Hover\n");
    fprintf(fp, "\tRequested pos: %d\tMaintain flag: %d.\n", params.Hover.location, params.Hover.maintain);
    printf("EXECUTING COMMAND: Hover\n");
    printf("\tRequested pos: %d\tMaintain flag: %d.\n", params.Hover.location, params.Hover.maintain);
    //sleep(2);
    return STATUS_FINISHED;

}

CommandStatus exc_patrol(Parameters params){
    return STATUS_FINISHED;

}
