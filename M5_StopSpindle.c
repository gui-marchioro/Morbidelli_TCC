#include "KMotionDef.h"
#include "Spindle.c"

// Routine to be executed when M5 is called in a G program.
main()
{
    ClearBit(PISTONS_ENABLE_OUTPUT);
    ClearBit(PISTONL_ENABLE_OUTPUT);
    ClearBit(CCW_ENABLE_OUTPUT);
    ClearBit(CW_ENABLE_OUTPUT);
    // Blocks the execution before the velocity is zero
    // Use Exec/Wait on KMotionCNC for this behavior
    while (ReadBit(STOP_CONFIRMATION_INPUT))
    {
        WaitNextTimeSlice();
    }

    SetSpindleStatus(SPINDLE_OFF);
}