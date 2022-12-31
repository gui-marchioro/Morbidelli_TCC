#include "Spindle.h"
#include "KMotionDef.h"

void SetSpindleStatus(int Status);
int GetSpindleStatus();
void StopSpindle();

void SetSpindleStatus(int Status)
{
    persist.UserData[SPINDLE_STATUS_VAR] = Status;
}

int GetSpindleStatus()
{
    return persist.UserData[SPINDLE_STATUS_VAR];
}

void StopSpindle()
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