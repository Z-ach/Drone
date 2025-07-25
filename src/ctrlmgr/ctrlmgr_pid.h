#ifndef CTRLMGR_PID_H
#define CTRLMGR_PID_H

#include <rc/mpu.h>
#include <rc/time.h>
#include <rc/math/vector.h>
#include <iomgr/logmgr.h>
#include "ctrlmgr_hw_build.h"

typedef struct kPID_t{
    double kP;
    double kI;
    double kD;
} kPID_t;

typedef struct PIDContainer{
    kPID_t roll;
    kPID_t pitch;
    kPID_t yaw;
} PIDContainer;

typedef enum PIDAxisType{
    ROLL,
    PITCH,
    YAW
} PIDAxisType;


void update_errors(rc_mpu_data_t mpu_data, rc_vector_t goal_gyro);
void run_pid_loops(_Atomic(rc_vector_t) *motor_thr, PIDContainer pid_container,
                   rc_mpu_data_t mpu_data, rc_vector_t goal_gyro, double thr);
double single_axis_pid(kPID_t pid_v, int axis);



#endif //CTRLMGR_PID_H
