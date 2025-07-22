#include "ctrlmgr_pid.h"

#define roll_ax 1
#define pitch_ax 0
#define yaw_ax 2
#define dbg_flag 1

rc_vector_t att_err = RC_VECTOR_INITIALIZER;
rc_vector_t delta_att_err = RC_VECTOR_INITIALIZER;
rc_vector_t last_att_err = RC_VECTOR_INITIALIZER;
rc_vector_t sum_att_err = RC_VECTOR_INITIALIZER;
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
    rc_vector_zeros(&att_err, 3);
    rc_vector_zeros(&delta_att_err, 3);
    rc_vector_zeros(&last_att_err, 3);
    rc_vector_zeros(&sum_att_err, 3);
    alt_err = 0;
    last_alt_err = 0;
    initialized = 1;
    last_time = rc_nanos_since_epoch();
}

void update_errors(rc_mpu_data_t mpu_data, rc_vector_t goal_att){
    if(!initialized){
        init_pid_vals();
    }else{
        dt = rc_nanos_since_epoch() - last_time;
        if(dt > timeout){
            dt = est_dt;
        }
    }
    // calculate errors
    //alt_err = goal_alt - est_alt;
    //sum_alt_err += alt_err * dt;
    for(int i = 0; i < 3; i++){
        att_err.d[i] = (mpu_data.fused_TaitBryan[i]*RAD_TO_DEG) - goal_att.d[i];
        if(i == 2){
            // Ignore yaw error when less than 5 deg in either direction
            if(att_err.d[i] < 5 && att_err.d[i] > -5){
                att_err.d[i] = 0;
            }
        }
        // add to accumulators
        sum_att_err.d[i] += att_err.d[i] * dt;
    }
    //rc_vector_sum_inplace(&sum_att_err, acc_err);

    // store the deltas
    //delta_alt_err = alt_err - last_alt_err;
    rc_vector_subtract(att_err, last_att_err, &delta_att_err);

    // mult by loop freq (expected dt)
    //delta_alt_err *= LOOP_HZ;
    rc_vector_times_scalar(&delta_att_err, dt);

    // save error as last error
    //last_alt_err = alt_err;
    for(int i = 0; i < 3; i++){
        last_att_err.d[i] = att_err.d[i];
    }
}

void run_pid_loops(_Atomic(rc_vector_t) *motor_thr, PIDContainer pid_container, rc_mpu_data_t mpu_data, rc_vector_t goal_att, double thr){
    update_errors(mpu_data, goal_att);
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

double single_axis_pid(kPID_t pid_v, int axis){
    double axis_output = (pid_v.kP * att_err.d[axis]) + (pid_v.kI * sum_att_err.d[axis]) + (pid_v.kD * delta_att_err.d[axis]);

    if(dbg_flag == 1){
        LOG_CTRL("\t\t\t\t\t\tAXIS: %s - Error: %3.2f - Output: %3.2f\n", axis_names[axis], att_err.d[axis], axis_output);
    }

    return axis_output;
}
