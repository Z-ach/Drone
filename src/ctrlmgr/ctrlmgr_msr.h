#ifndef DRONE_CTRLMGR_PID_H
#define DRONE_CTRLMGR_PID_H

#include <stdio.h>
#include <signal.h>
#include <math.h> // for M_PI
#include <rc/math/kalman.h>
#include <rc/math/filter.h>
#include <rc/math/quaternion.h>
#include <rc/time.h>
#include <rc/bmp.h>
#include <rc/mpu.h>
#include <fusion/Fusion.h>

#include "ctrlmgr_hw_build.h"
#include <iomgr/logmgr.h>

#define Nx 3
#define Ny 1
#define Nu 1
#define SAMPLE_RATE     100      // hz
#define DT              (1.0/SAMPLE_RATE)
#define ACCEL_LP_TC     20*DT   // fast LP filter for accel
#define PRINT_HZ        10
#define BMP_RATE_DIV    10      // optionally sample bmp less frequently than mpu
#define ATT_PUBLISH_RATE 200    // Rate at which to publish the shared att for telem
#define G_TO_MS2        9.80665


FusionAhrs ahrs;

int init_msr_system();
rc_mpu_data_t get_mpu_data();
double get_est_alt();
FusionEuler get_attitude();

#endif //DRONE_CTRLMGR_PID_H
