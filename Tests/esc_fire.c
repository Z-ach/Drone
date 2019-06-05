/**
 * @file rc_test_esc.c
 * @example    rc_test_esc
 *
 *
 * Demonstrates use of pru to control servos and ESCs with pulses. This program
 * operates in 4 different modes. See the option list below for how to select an
 * operational mode from the command line.
 *
 * SERVO: uses rc_servo_send_pulse_normalized() to set one or all servo
 * positions to a value from -1.5 to 1.5 corresponding to their extended range.
 * -1 to 1 is considered the "safe" normal range as some servos will not go
 * beyond this. Test your servos incrementally to find their safe range.
 *
 * ESC: For unidirectional brushless motor speed controllers specify a range
 * from 0 to 1 as opposed to the bidirectional servo range. Be sure to run the
 * calibrate_esc example first to make sure the ESCs are calibrated to the right
 * pulse range. This mode uses the rc_servo_send_esc_pulse_normalized() function.
 *
 * WIDTH: You can also specify your own pulse width in microseconds (us).
 * This uses the rc_servo_send_pulse_us() function.
 *
 * SWEEP: This is intended to gently sweep a servo back and forth about the
 * center position. Specify a range limit as a command line argument as
 * described below. This also uses the rc_servo_send_pulse_normalized()
 * function.
 *
 *
 *
 * @author     James Strawson
 * @date       3/20/2018
 */
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h> // for atoi
#include <signal.h>
#include <rc/time.h>
#include <rc/servo.h>
static int running = 0;
typedef enum test_mode_t{
        DISABLED,
        NORM,
        WIDTH,
        SWEEP,
        RADIO
}test_mode_t;
static void __print_usage(void)
{
        printf("\n");
        printf(" Options\n");
        printf(" -h             Print this help messege \n\n");
        printf(" -c {channel}   Specify one channel to be driven from 1-8.\n");
        printf("                Otherwise all channels will be driven equally\n");
        printf(" -f {hz}        Specify pulse frequency, otherwise 50hz is used\n");
        printf(" -t {throttle}  Throttle to send between -0.1 & 1.0\n");
        printf(" -o             Enable One-Shot mode\n");
        printf(" -w {width_us}  Send pulse width in microseconds (us)\n");
        printf(" -s {max}       Gently sweep throttle from 0 to {max} back to 0 again\n");
        printf("                {max} can be between 0 & 1.0\n");
        printf(" -m {min,max}   Set the pulse width range in microseconds, default is 1000,2000\n");
        printf("                if this option is not given. Use -m 1120,1920 for DJI ESCs.\n");
        printf(" -p {period,value} Set the wakeup period (seconds) and value (normalized)\n");
        printf("                default is 3.0,-0.1 if this option is not given.\n");
        printf("                Use -p 3,0.0 for DJI ESCs.\n");
        printf(" -d             Disable the wakeup period for ESCs which do not require it\n");
        printf("\n");
        printf("   rc_test_esc -c 2 -r 1\n\n");
        printf("   rc_test_esc -c 2 -r 1 -m 1120,1920 -p 1.0,0.0\n\n");
        printf("sample use to sweep all ESC channels from 0 to quarter throttle with oneshot mode\n");
        printf("   rc_test_esc -o -s 0.25\n\n");
}
// interrupt handler to catch ctrl-c
static void __signal_handler(__attribute__ ((unused)) int dummy)
{
        running=0;
        return;
}
int main(int argc, char *argv[])
{
        int c,i,ret;            // misc variables
        double thr = 0;         // normalized throttle
        int width_us = 0;       // pulse width in microseconds mode
        int ch = 0;             // channel to test, 0 means all channels
        test_mode_t mode;       // current operating mode
        int frequency_hz = 50;  // default 50hz frequency to send pulses
        int wakeup_en = 1;      // wakeup period enabled by default
        double wakeup_s = 1.5;  // wakeup period in seconds
        double wakeup_val = -0.1;// wakeup value
        int min_us = RC_ESC_DEFAULT_MIN_US;
        int max_us = RC_ESC_DEFAULT_MAX_US;
        // start with mode as disabled
        mode = DISABLED;
        // set signal handler so the loop can exit cleanly
        signal(SIGINT, __signal_handler);
        running=1;
        // initialize PRU and make sure power rail is OFF
        if(rc_servo_init()) return -1;
        if(rc_servo_set_esc_range(min_us,max_us)) return -1;
        rc_servo_power_rail_en(0);
        // if driving an ESC, send throttle of 0 first
        // otherwise it will go into calibration mode
        if(wakeup_en){
                printf("waking ESC up from idle for 3 seconds\n");
                for(i=0;i<=frequency_hz*wakeup_s;i++){
                        if(running==0) return 0;
                        if(rc_servo_send_esc_pulse_normalized(ch,wakeup_val)==-1) return -1;
                        rc_usleep(1000000/frequency_hz);
                }
                printf("done with wakeup period\n");
        }
        // Main loop runs at frequency_hz
        while(running){
                rc_servo_send_esc_pulse_normalized(ch,thr);
                rc_usleep(1000000/frequency_hz);
        }
        // cleanup
        else rc_servo_send_esc_pulse_normalized(ch,-0.1);
        rc_usleep(50000);
        rc_servo_cleanup();
        printf("\n");
        return 0;
}
