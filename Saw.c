#include "KMotionDef.h"
#include "Saw.h"

// Clear all outputs related to the saw operation
void ClearSawOutputs();

// Select saw
void DrillRoutine(int sawSlot);

// 0: Saw off
// 1: X Saw selected
// 2: Y Saw selected
void SetSawStatus(int sawSlot);
void ResetSawStatus();
int GetSawStatus();

void SetSawStatus(int sawSlot)
{
    if (sawSlot == SAW_X_SLOT)
    {
        persist.UserData[SAW_STATUS_VAR] = 1;
    }
    else
    {
        persist.UserData[SAW_STATUS_VAR] = 2;
    }
}

void ResetSawStatus()
{
    persist.UserData[SAW_STATUS_VAR] = 0;
}

int GetSawStatus()
{
    return persist.UserData[SAW_STATUS_VAR];
}

// Clear all outputs related to the saw operation
void ClearSawOutputs()
{
    ResetSawStatus();

    ClearBit(SAW_X_OUTPUT);
    ClearBit(SAW_Y_OUTPUT);
}

// Select saw
void SawRoutine(int sawSlot)
{
    SetSawStatus(sawSlot);
}