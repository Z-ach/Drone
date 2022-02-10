#ifndef CTRLMGR_HW_H
#define CTRLMGR_HW_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ctrlmgr_hw_build.h"
#include <logmgr/logmgr.h>

#ifdef HW_BUILD
#include <rc/time.h>
#include <rc/servo.h>
#include <rc/mpu.h>
#endif

typedef struct esc_vals{
    double front_left;
    double front_right;
    double back_left;
    double back_right;
} esc_vals;

typedef struct Telemetry{
    rc_mpu_data_t mpu_data;
    esc_vals esc_data;
} Telemetry;


void init_hardware();
void enable_leds();
void read_mpu();
void get_telemetry(char *resp_buf, int buf_size);
void telem_to_resp(Telemetry telem, char *resp_buff, int buf_size);

#endif
