#include "KMotionDef.h"

#ifndef TMP
#define TMP 10 // which spare persist to use to transfer data
#include "KflopToKMotionCNCFunctions.c"
#endif

#include "InitConfig.c"
#include "Homing.c"

// Program to be executed when the Homing button is pressed
main()
{
	if (GetInitExecuted() == 0)
	{
		MsgBox("Execute the Init function before Home", MB_ICONEXCLAMATION);
		return;
	}
	
	if (GetIsExecutingHoming() == 1 || JOB_ACTIVE)
	{
		return;
	}

	// If everything Ok, execute homing
	Homing();
}