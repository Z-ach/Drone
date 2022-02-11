#ifndef CTRLMGR_HW_H
#define CTRLMGR_HW_H


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdatomic.h>

#include "ctrlmgr_hw_build.h"
#include "ctrlmgr_pid.h"
#include "ctrlmgr_state.h"
#include <logmgr/logmgr.h>

#ifdef HW_BUILD
#include <rc/time.h>
#include <rc/led.h>
#include <rc/servo.h>
#include <rc/mpu.h>
#include <rc/math/vector.h>

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

void telem_to_resp(Telemetry telem, char *resp_buff, int buf_size);

#endif

void init_hardware();
void handle_shutdown();
void enable_leds();
void read_mpu();
void hover(_Atomic(CommandInfo) *cmd_info);
void get_telemetry(char *resp_buf, int buf_size);
void set_global_throttle(double d);
int esc_wakeup();
int write_to_motors();

#endif
