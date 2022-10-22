#include "InitConfig.h"
#include "MPGWatch.h"
#include "EmergencyWatch.h"
#include "ManualToolExchangeWatch.h"
#include "Homing.h"
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
	}
    // Initializes, monitor MPG and Emergency variables
    else
    {
        InitConfig();
        WaitNextTimeSlice();

        for(;;)
        {
            ManualToolExchangeWatch();
            BasicServiceMPG();
            LoopEmergencyMonitoring();
            
            WaitNextTimeSlice();
        }
    }
}