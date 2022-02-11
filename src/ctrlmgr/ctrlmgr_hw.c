#include "ctrlmgr_hw.h"

#define I2C_BUS 2
#ifdef HW_BUILD

rc_mpu_data_t mpu_data;
_Atomic(rc_vector_t) motor_thr = RC_VECTOR_INITIALIZER;
rc_vector_t goal_gyro = RC_VECTOR_INITIALIZER;
rc_vector_t goal_accel = RC_VECTOR_INITIALIZER;


void handle_shutdown(){
    rc_led_cleanup();
    rc_mpu_power_off();
    rc_servo_cleanup();
    LOG_CTRL("Hardware shutdown successful.\n");
}

// interrupt handler to process shutdown request
static void __signal_handler(__attribute__ ((unused)) int dummy){
    handle_shutdown();
    exit(0);
}

void init_hardware(){
    signal(SIGINT, __signal_handler);
    // initialize PRU and make sure power rail is OFF
    int min_us = RC_ESC_DEFAULT_MIN_US;
    int max_us = RC_ESC_DEFAULT_MAX_US;

    // init throttle vector
    rc_vector_zeros(&motor_thr, 4);

    // init servos
    rc_servo_init();
    rc_servo_set_esc_range(min_us,max_us);
    rc_servo_power_rail_en(0);
    LOG_CTRL("Performing ESC wakeup\n");
    esc_wakeup();

    // init mpu
    rc_mpu_config_t conf = rc_mpu_default_config();
    conf.i2c_bus = I2C_BUS;
    conf.show_warnings = 0;
    LOG_CTRL("Initialzing mpu\n");
    if(rc_mpu_initialize(&mpu_data, conf)){
        LOG_CTRL("MPU INIT FAIL.\n")
    }
    LOG_CTRL("Hardware init success.\n");
}

int esc_wakeup(){
    double wakeup_s_req = 3.0;
    set_global_throttle(-0.1);
    // throttle requires -0.1 for wakeup
    for(int i = 0; i <= LOOP_HZ*wakeup_s_req; i++){
        write_to_motors();
        rc_usleep(1000000/LOOP_HZ);
    }
    set_global_throttle(0.0);
    write_to_motors();
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

void read_mpu(){
    if(rc_mpu_read_accel(&mpu_data) < 0){
        LOG_CTRL("MPU ACCEL READ FAIL\n");
    }else{
        //LOG_CTRL("%f\t%f\t%f\n", mpu_data.accel[0], mpu_data.accel[1], mpu_data.accel[2]);
    }
    if(rc_mpu_read_gyro(&mpu_data) < 0){
        LOG_CTRL("MPU GYRO READ FAIL\n");
    }else{
        //LOG_CTRL("%f\t%f\t%f\n", mpu_data.gyro[0], mpu_data.gyro[1], mpu_data.gyro[2]);
    }
}

void get_telemetry(char *resp_buf, int buf_size){
    int stat;
    Telemetry *telem = malloc(sizeof(*telem));

    read_mpu();
    telem->esc_data.front_left = 0;
    telem->esc_data.front_right = 0;
    telem->esc_data.back_left = 0;
    telem->esc_data.back_right = 0;
    telem_to_resp(*telem, resp_buf, buf_size);
    free(telem);
}

void telem_to_resp(Telemetry telem, char *resp_buf, int buf_size){
    int resp_ptr = 0;
    int bytes_written;
    memset(resp_buf, 0, buf_size);

    // Record accel data
    for(int i = 0; i < 3; i++){
        bytes_written = snprintf(resp_buf+resp_ptr, buf_size-resp_ptr, "%3.4f,", mpu_data.accel[i]);
        resp_ptr += bytes_written;
    }

    // Record accel data
    for(int i = 0; i < 3; i++){
        bytes_written = snprintf(resp_buf+resp_ptr, buf_size-resp_ptr, "%3.4f,", mpu_data.gyro[i]);
        resp_ptr += bytes_written;
    }

    // Record ESC values
    for(int i = 0; i < 4; i++){
        bytes_written = snprintf(resp_buf+resp_ptr, buf_size-resp_ptr, "%3.4f,", motor_thr.d[i]);
        resp_ptr += bytes_written;
    }
}

void hover(_Atomic(CommandInfo) *cmd_info){
    rc_vector_zeros(&goal_gyro, 3);
    rc_vector_zeros(&goal_accel, 3);
    kPID_t pitch_pid = { 0.1, 0.0, 0.001 };
    kPID_t roll_pid = { 0.1, 0.0, 0.001 };
    kPID_t yaw_pid = { 0.1, 0.0, 0.001 };
    // account for g
    goal_accel.d[2] = -9.8;
    while(*cmd_info == NO_COMMANDS_QUEUED){
        read_mpu();
        LOG_CTRL("pre motor vals: %3.2f,%3.2f,%3.2f,%3.2f\n", motor_thr.d[0], motor_thr.d[1], motor_thr.d[2], motor_thr.d[3]);
        run_pid_loop(&motor_thr, pitch_pid, roll_pid, yaw_pid, mpu_data, goal_gyro, goal_accel);
        LOG_CTRL("post motor vals: %3.2f,%3.2f,%3.2f,%3.2f\n", motor_thr.d[0], motor_thr.d[1], motor_thr.d[2], motor_thr.d[3]);
        write_to_motors();
        rc_usleep(1000000/LOOP_HZ);
    }
}

void set_global_throttle(double thr){
    LOG_CTRL("Global throttle set to %f\n", thr);
    motor_thr.d[M_BL] = thr;
    motor_thr.d[M_BR] = thr;
    motor_thr.d[M_FL] = thr;
    motor_thr.d[M_FR] = thr;
    LOG_CTRL("Successfully set throttle for all motors.\n");
}

int write_to_motors(){
    int stat = 0;
    stat |= rc_servo_send_esc_pulse_normalized(M_BL+1, motor_thr.d[M_BL]);
    stat |= rc_servo_send_esc_pulse_normalized(M_BR+1, motor_thr.d[M_BR]);
    stat |= rc_servo_send_esc_pulse_normalized(M_FL+1, motor_thr.d[M_FL]);
    stat |= rc_servo_send_esc_pulse_normalized(M_FR+1, motor_thr.d[M_FR]);
    return stat;
}

#endif

#ifndef HW_BUILD
void hover(_Atomic(CommandInfo) *cmd_info){}
void init_hardware(){}
void handle_shutdown(){}
void get_telemetry(char *resp_buf, int buf_size){}
void enable_leds(){
    sleep(2);
}
#endif