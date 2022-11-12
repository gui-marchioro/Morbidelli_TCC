#include "InitConfig.c"
#include "MPGWatch.c"
#include "EmergencyWatch.c"
#include "ManualToolExchangeWatch.c"
#include "Homing.c"
#include "Spindle.c"
#include "MillChanger.c"
#include "TableSelectionWatch.c"
#include "KMotionDef.h"

// Initialization program that runs when the Init button is pressed inside KMotionCNC
// It initializes the boards, monitors alarms and listen to the IOs of the machine
main()
{
    IOboardsConfig();
    InitialEmergencyMonitoring();
    // Confirms if there is an emergency to be solved before init.
    if (GetEmergencyState() == 1)
	{
		MsgBox("Solve the emergency problem before Initialize", MB_ICONEXCLAMATION);
        return;
	}

    // Initializes KFlop and Kanalog board
    InitConfig();
    // Updates current milling tool in use
    int currentTool;
    GetCurrentTool(&currentTool);

    // monitor external buttons, MPG and Emergency variables
    for(;;)
    {
        ManualToolExchangeWatch();
        BasicServiceMPG();
        LoopEmergencyMonitoring();
        Table1SelectionWatch();
        Table2SelectionWatch();
        Table1And2SelectionWatch();
        
        WaitNextTimeSlice();
    }
    
}