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

typedef enum PIDAxisType{
    ROLL,
    PITCH,
    YAW
} PIDAxisType;


//void run_pid_loop(_Atomic(rc_vector_t) *motor_thr, kPID_t pid, rc_mpu_data_t mpu_data, rc_vector_t goal_gyro, rc_vector_t goal_accel);
//void run_pid_loop(_Atomic(rc_vector_t) *motor_thr, kPID_t roll_pid, kPID_t pitch_pid, kPID_t yaw_pid, rc_mpu_data_t mpu_data, rc_vector_t goal_gyro, rc_vector_t goal_accel);
void run_pid_loop(_Atomic(rc_vector_t) *motor_thr, kPID_t pid_v, rc_mpu_data_t mpu_data, double est_alt, rc_vector_t goal_gyro, rc_vector_t goal_accel, double goal_alt);



#endif //CTRLMGR_PID_H
