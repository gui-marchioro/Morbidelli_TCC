#include "KMotionDef.h"
#include "Spindle.h"

// Routine to be executed when M4 is called in a G program.
main()
{
    ClearBit(CW_ENABLE_OUTPUT);
    SetBit(PISTONS_ENABLE_OUTPUT);
    SetBit(PISTONL_ENABLE_OUTPUT);
    SetBit(CCW_ENABLE_OUTPUT);
    // Blocks the execution before the velocity is reached
    // Use Exec/Wait on KMotionCNC for this behavior
    while (!ReadBit(VELOCITY_CONFIRMATION_INPUT) && !ReadBit(PISTONS_SENSOR_INPUT) && !ReadBit(PISTONL_SENSOR_INPUT))
    {
        WaitNextTimeSlice();
    }
    
    Delay_sec(DELAY_BEFORE_MACHINING);
}