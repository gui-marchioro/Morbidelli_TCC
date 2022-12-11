#include "KMotionDef.h"
#include "Spindle.c"
#include "Drill.c"

// Routine to be executed when M3 is called in a G program.
main()
{
    // Turn on drill box if it is setted.
    if (GetDrillBoxStatus() == 1)
    {
        StopSpindle();
        SetBit(DRILL_MOTOR_OUTPUT);
        return; 
    }

    // Turn off enable CCW
    ClearBit(CCW_ENABLE_OUTPUT);

    // If spindle was on in the opposite direction blocks the execution of M3 command until
    // the spindle turns off.
    if (GetSpindleStatus() == SPINDLE_ON_CCW)
    {
        // Blocks the execution before the velocity is zero
        // Use Exec/Wait on KMotionCNC for this behavior
        while (ReadBit(STOP_CONFIRMATION_INPUT))
        {
            WaitNextTimeSlice();
        }
    }

    SetBit(PISTONS_ENABLE_OUTPUT);
    SetBit(PISTONL_ENABLE_OUTPUT);
    SetBit(CW_ENABLE_OUTPUT);
    // Blocks the execution before the velocity is reached
    // Use Exec/Wait on KMotionCNC for this behavior
    while (!ReadBit(VELOCITY_CONFIRMATION_INPUT)) //&& !ReadBit(PISTONS_SENSOR_INPUT) && !ReadBit(PISTONL_SENSOR_INPUT))
    {
        WaitNextTimeSlice();
    }
    
    SetSpindleStatus(SPINDLE_ON_CW);
    Delay_sec(DELAY_BEFORE_MACHINING);
}