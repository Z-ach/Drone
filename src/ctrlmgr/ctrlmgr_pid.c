#include "ctrlmgr_pid.h"
#ifdef HW_BUILD

rc_vector_t acc_err = RC_VECTOR_INITIALIZER;
rc_vector_t gyro_err = RC_VECTOR_INITIALIZER;
rc_vector_t delta_acc_err = RC_VECTOR_INITIALIZER;
rc_vector_t delta_gyro_err = RC_VECTOR_INITIALIZER;
rc_vector_t last_acc_err = RC_VECTOR_INITIALIZER;
rc_vector_t last_gyro_err = RC_VECTOR_INITIALIZER;
int initialized = 0;
uint64_t time, dt;
uint64_t timeout = 1000000000;
double deriv_mult = 1/LOOP_HZ;


// Called when first starting up or after a timeout
void init_pid_vals(){
    rc_vector_zeros(&acc_err, 3);
    rc_vector_zeros(&gyro_err, 3);
    rc_vector_zeros(&delta_acc_err, 3);
    rc_vector_zeros(&delta_gyro_err, 3);
    rc_vector_zeros(&last_acc_err, 3);
    rc_vector_zeros(&last_gyro_err, 3);
    initialized = 1;
    time = rc_nanos_since_epoch()
}


void run_pid_loop(_Atomic(rc_vector_t) *motor_thr, kPID_t pid, rc_mpu_data_t mpu_data, rc_vector_t goal_gyro, rc_vector_t goal_accel){
    if(!initialized){
        init_pid_vals();
    else{
        dt = rc_nanos_since_epoch()  - time;
        if(dt > timeout){
            init_pid_vals();
        }
    }
    // calculate errors
    for(int i = 0; i < 3; i++){
        acc_err.d[i] = mpu_data.accel[i] - goal_accel.d[i];
        gyro_err.d[i] = mpu_data.gyro[i] - goal_gyro.d[i];
    }
    // store the deltas
    rc_vector_subtract(acc_err, last_acc_err, &delta_acc_err);
    rc_vector_subtract(gyro_err, last_gyro_err, &delta_gyro_err);

    // mult by loop freq (expected dt)
    rc_vector_times_scalar(&delta_acc_err, deriv_mult);
    rc_vector_times_scalar(&delta_gyro_err, deriv_mult);

    // save error as last error
    for(int i = 0; i < 3; i++){
        last_acc_err.d[i] = acc_err.d[i];
        last_gyro_err.d[i] = gyro_err.d[i];
    }

    





    motor_thr->d[M_BR] = pid.kP * mpu_data.accel[0];
    initialized = 1;
}

#endif