#include "ctrlmgr_bat.h"


static double bat_msrs[BAT_BUF_SIZE] = {0};
int bat_idx = 0;
int ready = 0;

void init_bat(){
    rc_adc_init();
    for (int i = 0; i < BAT_BUF_SIZE; i++){
        update_bat();
    }
    ready = 1;
    rc_usleep(1000000/LOOP_HZ);
}

void update_bat(){
    bat_msrs[bat_idx] = rc_adc_dc_jack();
    bat_idx = (bat_idx + 1) % BAT_BUF_SIZE;
}

OperationStatus get_bat(double *res){
    if (!ready){
        return STATUS_FAIL;
    }
    update_bat();
    double sum = 0;
    for (int i = 0; i < BAT_BUF_SIZE; i++){
        sum += bat_msrs[i];
    }
    *res = sum / BAT_BUF_SIZE;
    return STATUS_OK;
}
