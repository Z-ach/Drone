#include "ctrlmgr_hw.h"

void init_hardware(){
    #ifdef HW_BUILD
    // initialize PRU and make sure power rail is OFF
    int min_us = RC_ESC_DEFAULT_MIN_US;
    int max_us = RC_ESC_DEFAULT_MAX_US;
    if(rc_servo_init()) return;
    if(rc_servo_set_esc_range(min_us,max_us));
    rc_servo_power_rail_en(0);
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
