#include "Drill.c"
#include "MillChanger.c"
#include "Spindle.c"
#include "KMotionDef.h"

#ifndef TMP
#define TMP 10 // which spare persist to use to transfer data
#include "KflopToKMotionCNCFunctions.c"
#endif

#define TOOL_VAR 9

// Routine to be executed when M6 TX is called in a G program. (X is the tool number)
void main ()
{
    // int previousSlot = persist.UserData[PREVIOUS_TOOL_VAR];
    int slot = persist.UserData[TOOL_VAR];  // Requested tool slot. Value stored is an int 
	// int id = persist.UserData[TOOL_VAR+1];  // value stored is an int 
	// printf("Tool Set to slot %d id %d\n",slot,id);  // print the slot and id

    // Turn off drill box if it is on.
    if (GetDrillBoxStatus() == 1)
    {
        ClearDrillOutputs();
    }

    // Turn off spindle if it is on.
    if (GetSpindleStatus() != SPINDLE_OFF)
    {
        StopSpindle();
    }

    if(slot >= MILL_TOOL_SLOT_MIN && slot <= MILL_TOOL_SLOT_MAX)
    {
        if (MillExchangeRoutine(slot))
        {
            // error, Halt Job
		    DoPC(PC_COMM_HALT);
        }
    }
    // Drill box routine
    else if (slot >= DRILL_BOX_SLOT_MIN && slot <= DRILL_BOX_SLOT_MAX)
    {
        DrillRoutine(slot);
    }
    else
    {
        MsgBox("The selected tool is out of the range. Please select one that is defined inside the tool table.", MB_ICONEXCLAMATION);
    }
}