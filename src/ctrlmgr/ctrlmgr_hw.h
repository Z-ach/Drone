#ifndef CTRLMGR_HW_H
#define CTRLMGR_HW_H

#define HW_BUILD

#ifdef HW_BUILD
    #include <rc/time.h>
    #include <rc/servo.h>
#endif

void init_hardware();

#endif