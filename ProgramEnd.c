#include "TableSelectionWatch.h"
#include "Spindle.c"
#include "Drill.c"
#include "KMotionDef.h"

main()
{
    // Reset executing state from table 1, 2 and both.
    // Required to to the machine state execution from TableSelectionWatch.c
    if (persist.UserData[TABLE1_EXECUTING_VAR] == 1)
    {
        persist.UserData[TABLE1_EXECUTING_VAR] = 0;
    }

    if (persist.UserData[TABLE2_EXECUTING_VAR] == 1)
    {
        persist.UserData[TABLE2_EXECUTING_VAR] = 0;
    }

    if (persist.UserData[TABLE12_EXECUTING_VAR] == 1)
    {
        persist.UserData[TABLE12_EXECUTING_VAR] = 0;
    }

    // For safety reasons, turn off spindle and drills
    if (GetSpindleStatus() != SPINDLE_OFF)
    {
        StopSpindle();
    }

    // Turn off drill box if it is on.
    if (GetDrillBoxStatus() == 1)
    {
        ClearDrillOutputs();
    }
}