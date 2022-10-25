#include "InitConfig.c"
#include "Homing.c"
#include "KMotionDef.h"

#ifndef TMP
#define TMP 10 // which spare persist to use to transfer data
#include "KflopToKMotionCNCFunctions.c"
#endif

// Program to be executed when the Homing button is pressed
main()
{
	if (GetInitExecuted() == 0)
	{
		MsgBox("Execute the Init function before Home", MB_ICONEXCLAMATION);
	}
	else
	{
		Homing();
	}
}