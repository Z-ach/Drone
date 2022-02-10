#include "ctrlmgr_hw.h"

#define I2C_BUS 2

rc_mpu_data_t mpu_data;

void init_hardware(){
    #ifdef HW_BUILD
    // initialize PRU and make sure power rail is OFF
    int min_us = RC_ESC_DEFAULT_MIN_US;
    int max_us = RC_ESC_DEFAULT_MAX_US;

    // init servos
    rc_servo_init();
    rc_servo_set_esc_range(min_us,max_us);
    rc_servo_power_rail_en(0);

    // init mpu
    rc_mpu_config_t conf = rc_mpu_default_config();
    conf.i2c_bus = I2C_BUS;
    conf.show_warnings = 0;
    if(rc_mpu_initialize(&mpu_data, conf)){
        LOG_CTRL("MPU INIT FAIL.\n")
    }
    //read_mpu(&mpu_data);
    #endif
}

void enable_leds(){
    #ifdef HW_BUILD
    #include <rc/led.h>
    #include <rc/time.h>

    #define WAIT_US 500000 // time to light each LED in microseconds

    rc_led_set(RC_LED_GREEN,1);
    rc_usleep(WAIT_US);
    rc_led_set(RC_LED_GREEN,0);

    rc_led_set(RC_LED_RED,1);
    rc_usleep(WAIT_US);
    rc_led_set(RC_LED_RED,0);

    rc_led_cleanup();
    #endif
    #ifndef HW_BUILD
    sleep(2);
    #endif
}

void read_mpu(){
    if(rc_mpu_read_accel(&mpu_data) < 0){
        LOG_CTRL("MPU ACCEL READ FAIL\n");
    }else{
        LOG_CTRL("%f\t%f\t%f\n", mpu_data.accel[0], mpu_data.accel[1], mpu_data.accel[2]);
    }
    if(rc_mpu_read_gyro(&mpu_data) < 0){
        LOG_CTRL("MPU GYRO READ FAIL\n");
    }else{
        LOG_CTRL("%f\t%f\t%f\n", mpu_data.gyro[0], mpu_data.gyro[1], mpu_data.gyro[2]);
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
    double esc_vals[4] = {
        telem.esc_data.front_left,
        telem.esc_data.front_right,
        telem.esc_data.back_left,
        telem.esc_data.back_right
    };

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
    for(int i = 0; i < 3; i++){
        bytes_written = snprintf(resp_buf+resp_ptr, buf_size-resp_ptr, "%3.4f,", esc_vals[i]);
        resp_ptr += bytes_written;
    }
}