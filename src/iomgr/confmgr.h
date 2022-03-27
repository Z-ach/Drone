#ifndef CONFMGR_H
#define CONFMGR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wordexp.h>

#include <ctrlmgr/ctrlmgr_pid.h>
#include "logmgr.h"

typedef struct config{
    PIDContainer pid_container;
    double base_thr;
} config_t;

typedef enum cfg_t{
    roll_kP,
    roll_kI,
    roll_kD,
    pitch_kP,
    pitch_kI,
    pitch_kD,
    yaw_kP,
    yaw_kI,
    yaw_kD,
    thr
} cfg_t;

void read_config();
config_t get_config();
void resolve_conf_path();
void update_cfg_from_net(cfg_t type, double val);
void write_config(config_t new_config);
int cfg_to_resp(char *resp_buf, int buf_size);

#endif