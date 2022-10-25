#include "Spindle.h"
#include "KMotionDef.h"

void SetSpindleStatus(int Status)
{
    persist.UserData[SPINDLE_STATUS_VAR] = Status;
}

int GetSpindleStatus()
{
    return persist.UserData[SPINDLE_STATUS_VAR];
}