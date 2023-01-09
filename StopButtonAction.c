#include "KMotionDef.h"

#ifndef TMP
#define TMP 10 // which spare persist to use to transfer data
#include "KflopToKMotionCNCFunctions.c"
#endif

#include "InitConfig.c"
#include "Homing.c"
#include "Drill.c"
#include "Spindle.c"
#include "Saw.c"
#include "Magazine.c"
#include "TableSelectionWatch.c"

// Program to be executed when the Stop button is pressed
// DoPC(PC_COMM_ESTOP);
void main()
{
    // For safety reasons, turn off spindle, drills and saw
    ClearDrillOutputs();
    ClearSawOutputs();
    StopSpindle();
    // Disable Axis
    ClearBit(ENABLE_ALL_AXIS_PIN);
    // Reset Init and Homing variables, requiring the user do it again to keep using the machine
    ResetHomingExecuted();
    ResetIsExecutingHoming();
    ResetInitExecuted();
    SetMagazineStatus(0);
    // Reset table states
    persist.UserData[TABLE_EXECUTING_VAR] = 0;
    persist.UserData[TABLE1_EXECUTING_VAR] = 0;
    persist.UserData[TABLE2_EXECUTING_VAR] = 0;
    persist.UserData[TABLE12_EXECUTING_VAR] = 0;
    // Reset table outputs
    ClearTableOutputs();
}