#include "InitConfig.h"
#include "Homing.h"
#include "KMotionDef.h"

#ifndef TMP
#define TMP 10 // which spare persist to use to transfer data
#include "KflopToKMotionCNCFunctions.c"
#endif

// Program to be executed when the Stop button is pressed
// DoPC(PC_COMM_ESTOP);
void main()
{
    ClearBit(ENABLE_ALL_AXIS_PIN);
    ResetHomingExecuted();
    ResetInitExecuted();
}