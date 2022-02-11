#ifndef DRONE_CTRLMGR_PID_H
#define DRONE_CTRLMGR_PID_H

#include "ctrlmgr_hw_build.h"
#include <logmgr/logmgr.h>

typedef struct kPID_t{
    double kP;
    double kI;
    double kD;
} kPID_t;

typedef enum PIDAxisType{
    ROLL,
    PITCH,
    YAW
} PIDAxisType;

#ifdef HW_BUILD
#include <rc/mpu.h>
#include <rc/time.h>
#include <rc/math/vector.h>

//void run_pid_loop(_Atomic(rc_vector_t) *motor_thr, kPID_t pid, rc_mpu_data_t mpu_data, rc_vector_t goal_gyro, rc_vector_t goal_accel);
void run_pid_loop(_Atomic(rc_vector_t) *motor_thr, kPID_t roll_pid, kPID_t pitch_pid, kPID_t yaw_pid, rc_mpu_data_t mpu_data, rc_vector_t goal_gyro, rc_vector_t goal_accel);

#endif


#endif //DRONE_CTRLMGR_PID_H
