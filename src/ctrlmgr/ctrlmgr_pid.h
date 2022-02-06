#ifndef DRONE_CTRLMGR_PID_H
#define DRONE_CTRLMGR_PID_H

typedef struct PIDRoll{
    double kP;
    double kI;
    double kD;
} PIDRoll;

typedef struct PIDPitch{
    double kP;
    double kI;
    double kD;
} PIDPitch;

typedef struct PIDYaw{
    double kP;
    double kI;
    double kD;
} PIDYaw;

typedef enum PIDAxisType{
    ROLL,
    PITCH,
    YAW
} PIDAxisType;

void write_to_motors();

#endif //DRONE_CTRLMGR_PID_H
