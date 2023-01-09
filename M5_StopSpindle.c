#include "KMotionDef.h"
#include "Spindle.c"
#include "Drill.c"
#include "Saw.c"

// Routine to be executed when M5 is called in a G program.
main()
{
    ClearDrillOutputs();
    ClearSawOutputs();
    StopSpindle();
}