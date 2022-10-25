#ifndef Spindle_h
#define Spindle_h

// Outputs to be controled for spindle interaction
#define CW_ENABLE_OUTPUT 1057
#define CCW_ENABLE_OUTPUT 1058
#define PISTONS_ENABLE_OUTPUT 1063
#define PISTONL_ENABLE_OUTPUT 1064
// Inputs to be monitored for spindle interaction
#define VELOCITY_CONFIRMATION_INPUT 1094
#define STOP_CONFIRMATION_INPUT 1095
//#define PISTONS_SENSOR_INPUT 1042
//#define PISTONL_SENSOR_INPUT 1044
// seconds of delay before execute other G code
#define DELAY_BEFORE_MACHINING 1
// Persist variable to sign the status of the spindle
#define SPINDLE_STATUS_VAR 190 // 0 indicates that spindle is off, 1 CW, 2 CCW
#define SPINDLE_OFF 0
#define SPINDLE_ON_CW 1
#define SPINDLE_ON_CCW 2

void SetSpindleStatus(int Status);

int GetSpindleStatus();

#endif /* Spindle_h */