#ifndef DRONE_CTRLMGR_BAT_H
#define DRONE_CTRLMGR_BAT_H

#include "ctrlmgr_stat.h"
#include "ctrlmgr_hw_build.h"

#include <rc/adc.h>
#include <rc/time.h>

#define BAT_BUF_SIZE 20

void init_bat();
void update_bat();
OperationStatus get_bat(double *res);

#endif // DRONE_CTRLMGR_BAT_H
