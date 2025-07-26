#include "ctrlmgr_hw.h"

_Atomic(rc_vector_t) motor_thr = RC_VECTOR_INITIALIZER;
rc_vector_t goal_gyro = RC_VECTOR_INITIALIZER;
rc_vector_t goal_accel = RC_VECTOR_INITIALIZER;
const double MOTOR_THROT_MAX = 0.55;
config_t config;

void handle_shutdown(){
    rc_led_cleanup();
    rc_mpu_power_off();
    rc_bmp_power_off();
    rc_servo_cleanup();
    LOG_CTRL("Hardware shutdown successful.\n");
}

// interrupt handler to process shutdown request
static void __signal_handler(__attribute__ ((unused)) int dummy){
    handle_shutdown();
    exit(0);
}

void update_config(config_t new_config){
    config = new_config;
}

void init_hardware(){
    // register interrupt handler to ensure everything shuts down properly
    signal(SIGINT, __signal_handler);
    // initialize PRU and make sure power rail is OFF
    int min_us = RC_ESC_DEFAULT_MIN_US;
    int max_us = RC_ESC_DEFAULT_MAX_US;

    // get config values
    config = get_config();

    // ignore warning about pointer types
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
    // init throttle vector
    rc_vector_zeros(&motor_thr, 4);
    #pragma GCC diagnostic pop
    rc_vector_zeros(&goal_gyro, 3);
    rc_vector_zeros(&goal_accel, 3);

    // init servos
    rc_servo_init();
    rc_servo_set_esc_range(min_us,max_us);
    rc_servo_power_rail_en(0);
    LOG_CTRL("Performing ESC wakeup\n");
    esc_wakeup();

    // init measurement system: mpu/bmp/dmp
    init_msr_system();
    LOG_CTRL("Hardware init success.\n");
    goal_gyro.d[2] = get_mpu_data().fused_TaitBryan[2] * RAD_TO_DEG;

    // init battery system
    init_bat();
}

int esc_wakeup(){
    double wakeup_s_req = 3.0;
    set_global_throttle(-0.1);
    // throttle requires -0.1 for wakeup
    for(int i = 0; i <= LOOP_HZ*wakeup_s_req; i++){
        write_to_motors(1);
        rc_usleep(1000000/LOOP_HZ);
    }
    set_global_throttle(0.05);
    write_to_motors(0);
}

void enable_leds(){
    #define WAIT_US 500000 // time to light each LED in microseconds

    rc_led_set(RC_LED_GREEN,1);
    rc_usleep(WAIT_US);
    rc_led_set(RC_LED_GREEN,0);

    rc_led_set(RC_LED_RED,1);
    rc_usleep(WAIT_US);
    rc_led_set(RC_LED_RED,0);

    rc_led_cleanup();
}

int telem_to_resp(char *resp_buf, int buf_size){
    int resp_ptr = 0;
    int bytes_written;
    rc_mpu_data_t mpu_data = get_mpu_data();
    double alt_est = get_est_alt();
    double bat_volt = 0;
    get_bat(&bat_volt);
    memset(resp_buf, 0, buf_size);
    bytes_written = snprintf(resp_buf+resp_ptr, buf_size-resp_ptr, "tlm:");
    resp_ptr += bytes_written;

    // Record attitude
    FusionEuler attitude = get_attitude();
    for(int i = 0; i < 3; i++){
        bytes_written = snprintf(resp_buf+resp_ptr, buf_size-resp_ptr, "%3.4f,", attitude.array[i]);
        resp_ptr += bytes_written;
    }

    // Record accel data
    for(int i = 0; i < 3; i++){
        bytes_written = snprintf(resp_buf+resp_ptr, buf_size-resp_ptr, "%3.4f,", mpu_data.accel[i]);
        resp_ptr += bytes_written;
    }

    // Record gyro data
    for(int i = 0; i < 3; i++){
        bytes_written = snprintf(resp_buf+resp_ptr, buf_size-resp_ptr, "%3.4f,", mpu_data.gyro[i]);
        resp_ptr += bytes_written;
    }

    // Add altitude estimation
    bytes_written = snprintf(resp_buf+resp_ptr, buf_size-resp_ptr, "%3.4f,", alt_est);
    resp_ptr += bytes_written;

    // Record ESC values
    for(int i = 0; i < 4; i++){
        bytes_written = snprintf(resp_buf+resp_ptr, buf_size-resp_ptr, "%3.4f,", motor_thr.d[i]);
        resp_ptr += bytes_written;
    }

    // Record battery info
    bytes_written = snprintf(resp_buf+resp_ptr, buf_size-resp_ptr, "%3.4f,", bat_volt);
    resp_ptr += bytes_written;
    return resp_ptr;
}

void hover(_Atomic(CommandInfo) *cmd_info){
    config = get_config();
    FusionEuler est_attitude = get_attitude();
    FusionEuler targ_attitude = { 0.0f, 0.0f, est_attitude.angle.pitch };
    double est_alt = get_est_alt();
    double thr = config.base_thr;
    double goal_alt = 2; // set goal altitude to 1 meter
    PIDContainer pid_container = config.pid_container;
    set_global_throttle(0.05);
    while(*cmd_info == NO_COMMANDS_QUEUED){
        est_attitude = get_attitude();
        LOG_CTRL("pre motor vals: %3.2f,%3.2f,%3.2f,%3.2f\n", motor_thr.d[0], motor_thr.d[1], motor_thr.d[2], motor_thr.d[3]);
        run_pid_loops(&motor_thr, pid_container, est_attitude, targ_attitude, thr);
        LOG_CTRL("post motor vals: %3.2f,%3.2f,%3.2f,%3.2f\n", motor_thr.d[0], motor_thr.d[1], motor_thr.d[2], motor_thr.d[3]);
        write_to_motors(0);
        rc_usleep(1000000/LOOP_HZ);
    }
}

void idle(_Atomic(CommandInfo) *cmd_info){
    int min_timeout = 3 * LOOP_HZ;
    int counter = 0;
    LOG_CTRL("Executing idle command.\n");
    while(*cmd_info == NO_COMMANDS_QUEUED || counter < min_timeout){
        set_global_throttle(0.05);
        write_to_motors(0);
        rc_usleep(1000000/LOOP_HZ);
        if (counter < min_timeout){
            counter++;
        }
    }
    LOG_CTRL("Idle command finished.\n");
}

void set_global_throttle(double thr){
    //LOG_CTRL("Global throttle set to %f\n", thr);
    motor_thr.d[M_BL] = thr;
    motor_thr.d[M_BR] = thr;
    motor_thr.d[M_FL] = thr;
    motor_thr.d[M_FR] = thr;
    //LOG_CTRL("Successfully set throttle for all motors.\n");
}

int write_to_motors(int wakeup){
    int stat = 0;
    // ensure values are not out of bounds
    if(!wakeup){
        for(int i = 0; i < 4; i++){
            if(motor_thr.d[i] <= 0.05){
                motor_thr.d[i] = 0.05;
            }else if(motor_thr.d[i] > MOTOR_THROT_MAX){
                motor_thr.d[i] = MOTOR_THROT_MAX;
            }
        }
    }
    stat |= rc_servo_send_esc_pulse_normalized(M_BL+PORT_OFFSET, motor_thr.d[M_BL]);
    stat |= rc_servo_send_esc_pulse_normalized(M_BR+PORT_OFFSET, motor_thr.d[M_BR]);
    stat |= rc_servo_send_esc_pulse_normalized(M_FL+PORT_OFFSET, motor_thr.d[M_FL]);
    stat |= rc_servo_send_esc_pulse_normalized(M_FR+PORT_OFFSET, motor_thr.d[M_FR]);
    return stat;
}
