#include "InitConfig.c"
#include "Homing.c"
#include "Drill.c"
#include "KMotionDef.h"

#ifndef TMP
#define TMP 10 // which spare persist to use to transfer data
#include "KflopToKMotionCNCFunctions.c"
#endif

// Program to be executed when the Stop button is pressed
// DoPC(PC_COMM_ESTOP);
void main()
{
    ClearDrillOutputs();
    MDI("M5");
    ClearBit(ENABLE_ALL_AXIS_PIN);
    ResetHomingExecuted();
    ResetInitExecuted();
}