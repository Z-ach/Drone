#include "ctrlmgr_pid.h"
#ifdef HW_BUILD

#define roll_ax 0
#define pitch_ax 1
#define yaw_ax 2

rc_vector_t acc_err = RC_VECTOR_INITIALIZER;
rc_vector_t gyro_err = RC_VECTOR_INITIALIZER;
rc_vector_t delta_acc_err = RC_VECTOR_INITIALIZER;
rc_vector_t delta_gyro_err = RC_VECTOR_INITIALIZER;
rc_vector_t last_acc_err = RC_VECTOR_INITIALIZER;
rc_vector_t last_gyro_err = RC_VECTOR_INITIALIZER;
rc_vector_t sum_acc_err = RC_VECTOR_INITIALIZER;
rc_vector_t sum_gyro_err = RC_VECTOR_INITIALIZER;
int initialized = 0;
uint64_t last_time, dt;
uint64_t timeout = 1000000000;
double est_dt = 1/LOOP_HZ;
double roll_output, pitch_output, yaw_output;
double MOTOR_THROT_MAX = 0.25;


// Called when first starting up or after a timeout
void init_pid_vals(){
    rc_vector_zeros(&acc_err, 3);
    rc_vector_zeros(&gyro_err, 3);
    rc_vector_zeros(&delta_acc_err, 3);
    rc_vector_zeros(&delta_gyro_err, 3);
    rc_vector_zeros(&last_acc_err, 3);
    rc_vector_zeros(&last_gyro_err, 3);
    rc_vector_zeros(&sum_acc_err, 3);
    rc_vector_zeros(&sum_gyro_err, 3);
    initialized = 1;
    last_time = rc_nanos_since_epoch();
}


void run_pid_loop(_Atomic(rc_vector_t) *motor_thr, kPID_t roll_pid, kPID_t pitch_pid, kPID_t yaw_pid, rc_mpu_data_t mpu_data, rc_vector_t goal_gyro, rc_vector_t goal_accel){
    if(!initialized){
        init_pid_vals();
    }else{
        dt = rc_nanos_since_epoch() - last_time;
        if(dt > timeout){
            init_pid_vals();
        }
    }
    // calculate errors
    for(int i = 0; i < 3; i++){
        acc_err.d[i] = mpu_data.accel[i] - goal_accel.d[i];
        gyro_err.d[i] = mpu_data.gyro[i] - goal_gyro.d[i];
        // add to accumulators
        sum_acc_err.d[i] += acc_err.d[i] * est_dt;
        sum_acc_err.d[i] += acc_err.d[i] * est_dt;
    }
    rc_vector_sum_inplace(&sum_acc_err, acc_err);
    rc_vector_sum_inplace(&sum_gyro_err, acc_err);

    // store the deltas
    rc_vector_subtract(acc_err, last_acc_err, &delta_acc_err);
    rc_vector_subtract(gyro_err, last_gyro_err, &delta_gyro_err);

    // mult by loop freq (expected dt)
    rc_vector_times_scalar(&delta_acc_err, LOOP_HZ);
    rc_vector_times_scalar(&delta_gyro_err, LOOP_HZ);

    // save error as last error
    for(int i = 0; i < 3; i++){
        last_acc_err.d[i] = acc_err.d[i];
        last_gyro_err.d[i] = gyro_err.d[i];
    }

    roll_output  = (roll_pid.kP * gyro_err.d[roll_ax]) + (roll_pid.kI * sum_gyro_err.d[roll_ax]) + (roll_pid.kD * delta_gyro_err.d[roll_ax]);
    pitch_output = (pitch_pid.kP * gyro_err.d[pitch_ax]) + (pitch_pid.kI * sum_gyro_err.d[pitch_ax]) + (pitch_pid.kD * delta_gyro_err.d[pitch_ax]);
    yaw_output   = (yaw_pid.kP * gyro_err.d[yaw_ax]) + (yaw_pid.kI * sum_gyro_err.d[yaw_ax]) + (yaw_pid.kD * delta_gyro_err.d[yaw_ax]);

    // left/right side: roll components same sign
    // back/front side: pitch components same sign
    // cw/ccw corners: yaw components same sign
    motor_thr->d[M_BR] = roll_output + pitch_output + yaw_output;
    motor_thr->d[M_BL] = -roll_output + pitch_output - yaw_output;
    motor_thr->d[M_FR] = roll_output - pitch_output - yaw_output;
    motor_thr->d[M_FL] = -roll_output - pitch_output + yaw_output;

    // ensure values are not out of bounds
    for(int i = 0; i < 4; i++){
        if(motor_thr->d[i] < 0){
            motor_thr->d[i] = 0;
        }else if(motor_thr->d[i] > MOTOR_THROT_MAX){
            motor_thr->d[i] = MOTOR_THROT_MAX;
        }
    }
}

#endif