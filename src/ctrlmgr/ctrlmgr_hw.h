#ifndef CTRLMGR_HW_H
#define CTRLMGR_HW_H

/* #undef HW_BUILD */

#include <unistd.h>

#ifdef HW_BUILD
#include <rc/time.h>
#include <rc/servo.h>
#endif

void init_hardware();
void enable_leds();

#endif
