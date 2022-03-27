#include "ctrlmgr_pid.h"

#define roll_ax 1
#define pitch_ax 0
#define yaw_ax 2
#define dbg_flag 1

rc_vector_t acc_err = RC_VECTOR_INITIALIZER;
rc_vector_t gyro_err = RC_VECTOR_INITIALIZER;
rc_vector_t delta_acc_err = RC_VECTOR_INITIALIZER;
rc_vector_t delta_gyro_err = RC_VECTOR_INITIALIZER;
rc_vector_t last_acc_err = RC_VECTOR_INITIALIZER;
rc_vector_t last_gyro_err = RC_VECTOR_INITIALIZER;
rc_vector_t sum_acc_err = RC_VECTOR_INITIALIZER;
rc_vector_t sum_gyro_err = RC_VECTOR_INITIALIZER;
double alt_err, delta_alt_err, last_alt_err, sum_alt_err;
int initialized = 0;
uint64_t last_time, dt;
uint64_t timeout = 1000000000;
double est_dt = 1/LOOP_HZ;
double roll_output, pitch_output, yaw_output, alt_output;

// Helpful for printing debug values
char *axis_names[3] = {"ROLL", "PITCH", "YAW"};


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
    alt_err = 0;
    last_alt_err = 0;
    initialized = 1;
    last_time = rc_nanos_since_epoch();
}

void update_errors(rc_mpu_data_t mpu_data, rc_vector_t goal_gyro, rc_vector_t goal_accel){
    if(!initialized){
        init_pid_vals();
    }else{
        dt = rc_nanos_since_epoch() - last_time;
        if(dt > timeout){
            init_pid_vals();
        }
    }
    // calculate errors
    //alt_err = goal_alt - est_alt;
    //sum_alt_err += alt_err * est_dt;
    for(int i = 0; i < 3; i++){
        acc_err.d[i] = mpu_data.accel[i] - goal_accel.d[i];
        gyro_err.d[i] = (mpu_data.fused_TaitBryan[i]*RAD_TO_DEG) - goal_gyro.d[i];
        if(i == 2){
            // Ignore yaw error when less than 5 deg in either direction
            if(gyro_err.d[i] < 5 && gyro_err.d[i] > -5){
                gyro_err.d[i] = 0;
            }
        }
        // add to accumulators
        sum_acc_err.d[i] += acc_err.d[i] * est_dt;
        sum_acc_err.d[i] += acc_err.d[i] * est_dt;
    }
    //rc_vector_sum_inplace(&sum_acc_err, acc_err);
    //rc_vector_sum_inplace(&sum_gyro_err, acc_err);

    // store the deltas
    //delta_alt_err = alt_err - last_alt_err;
    rc_vector_subtract(acc_err, last_acc_err, &delta_acc_err);
    rc_vector_subtract(gyro_err, last_gyro_err, &delta_gyro_err);

    // mult by loop freq (expected dt)
    //delta_alt_err *= LOOP_HZ;
    rc_vector_times_scalar(&delta_acc_err, LOOP_HZ);
    rc_vector_times_scalar(&delta_gyro_err, LOOP_HZ);

    // save error as last error
    //last_alt_err = alt_err;
    for(int i = 0; i < 3; i++){
        last_acc_err.d[i] = acc_err.d[i];
        last_gyro_err.d[i] = gyro_err.d[i];
    }
}

void run_pid_loops(_Atomic(rc_vector_t) *motor_thr, PIDContainer pid_container, rc_mpu_data_t mpu_data, rc_vector_t goal_gyro, rc_vector_t goal_accel, double thr){
    update_errors(mpu_data, goal_gyro, goal_accel);
    double roll_output = single_axis_pid(pid_container.roll, roll_ax);
    double pitch_output = single_axis_pid(pid_container.pitch, pitch_ax);
    double yaw_output = single_axis_pid(pid_container.yaw, yaw_ax);

    // left/right side: roll components same sign
    // back/front side: pitch components same sign
    // cw/ccw corners: yaw components same sign
    motor_thr->d[M_BR] = roll_output + pitch_output - yaw_output + thr;
    motor_thr->d[M_BL] = -roll_output + pitch_output + yaw_output + thr;
    motor_thr->d[M_FR] = roll_output - pitch_output + yaw_output + thr;
    motor_thr->d[M_FL] = -roll_output - pitch_output - yaw_output + thr;
}

//void run_pid_loop(_Atomic(rc_vector_t) *motor_thr, kPID_t roll_pid, kPID_t pitch_pid, kPID_t yaw_pid, rc_mpu_data_t mpu_data, rc_vector_t goal_gyro, rc_vector_t goal_accel){
double single_axis_pid(kPID_t pid_v, int axis){
    double axis_output = (pid_v.kP * gyro_err.d[axis]) + (pid_v.kI * sum_gyro_err.d[axis]) + (pid_v.kD * delta_gyro_err.d[axis]);

    if(dbg_flag == 1){
        LOG_CTRL("\t\t\t\t\t\tAXIS: %s - Error: %3.2f - Output: %3.2f\n", axis_names[axis], gyro_err.d[axis], axis_output);
    }

    return axis_output;
}

/*

    roll_output  = -0.1*((pid_v.kP * gyro_err.d[roll_ax]) + (pid_v.kI * sum_gyro_err.d[roll_ax]) + (pid_v.kD * delta_gyro_err.d[roll_ax]));
    pitch_output = 0.1*((pid_v.kP * gyro_err.d[pitch_ax]) + (pid_v.kI * sum_gyro_err.d[pitch_ax]) + (pid_v.kD * delta_gyro_err.d[pitch_ax]));
    yaw_output   = 0.01*((pid_v.kP * gyro_err.d[yaw_ax]) + (pid_v.kI * sum_gyro_err.d[yaw_ax]) + (pid_v.kD * delta_gyro_err.d[yaw_ax]));
    //alt_output   = 0;//1*((pid_v.kP * alt_err) + (pid_v.kI * sum_alt_err) + (pid_v.kD * delta_alt_err));

    roll_output  = (roll_pid.kP * gyro_err.d[roll_ax]) + (roll_pid.kI * sum_gyro_err.d[roll_ax]) + (roll_pid.kD * delta_gyro_err.d[roll_ax]);
    pitch_output = (pitch_pid.kP * gyro_err.d[pitch_ax]) + (pitch_pid.kI * sum_gyro_err.d[pitch_ax]) + (pitch_pid.kD * delta_gyro_err.d[pitch_ax]);
    yaw_output   = (yaw_pid.kP * gyro_err.d[yaw_ax]) + (yaw_pid.kI * sum_gyro_err.d[yaw_ax]) + (yaw_pid.kD * delta_gyro_err.d[yaw_ax]);
    LOG_CTRL("\t\t\t\t\t\troll_err: %3.2f pitch_err: %3.2f yaw_err: %3.2f\n", gyro_err.d[roll_ax], gyro_err.d[pitch_ax], gyro_err.d[yaw_ax]);
    LOG_CTRL("\t\t\t\t\tp: %f i: %f d: %f \n", pid_v.kP, pid_v.kI, pid_v.kD );
    LOG_CTRL("\t\t\t\t\t\troll: %3.2f pitch: %3.2f yaw: %3.2f alt: %3.2f\n", roll_output, pitch_output, yaw_output, thr);

    // left/right side: roll components same sign
    // back/front side: pitch components same sign
    // cw/ccw corners: yaw components same sign
    motor_thr->d[M_BR] = roll_output + pitch_output - yaw_output + thr;
    motor_thr->d[M_BL] = -roll_output + pitch_output + yaw_output + thr;
    motor_thr->d[M_FR] = roll_output - pitch_output + yaw_output + thr;
    motor_thr->d[M_FL] = -roll_output - pitch_output - yaw_output + thr;
}
*/