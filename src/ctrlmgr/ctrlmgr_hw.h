#ifndef CTRLMGR_HW_H
#define CTRLMGR_HW_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdatomic.h>

#include "ctrlmgr_bat.h"
#include "ctrlmgr_hw_build.h"
#include "ctrlmgr_pid.h"
#include "ctrlmgr_msr.h"
#include "ctrlmgr_state.h"
#include <iomgr/logmgr.h>
#include <iomgr/confmgr.h>

#include <rc/time.h>
#include <rc/led.h>
#include <rc/servo.h>
#include <rc/mpu.h>
#include <rc/bmp.h>
#include <rc/math/vector.h>

// limit motor output while testing
extern const double MOTOR_THROT_MAX;

void init_hardware();
void handle_shutdown();
void update_config(config_t new_config);
void enable_leds();
void read_mpu();
void hover(_Atomic(CommandInfo) *cmd_info);
void idle(_Atomic(CommandInfo) *cmd_info);
int telem_to_resp(char *resp_buff, int buf_size);
void set_global_throttle(double d);
int esc_wakeup();
int write_to_motors(int wakeup);

#endif
