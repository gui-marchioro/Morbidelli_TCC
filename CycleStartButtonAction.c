#include "Homing.h"
#include "InitConfig.h"
#include "KMotionDef.h"

#define TMP 10 // which spare persist to use to transfer data
#include "KflopToKMotionCNCFunctions.c"

// Program to be executed when the Cycle Start button is pressed
// DoPC(PC_COMM_EXECUTE);
void main()
{
    if (GetInitExecuted() == 0 || GetHomingExecuted() == 0 || GetIsExecutingHoming == 1)
    {
        DoPC(PC_COMM_HALT);
        MsgBox("Execute the Init and Homing function before run a program", MB_ICONEXCLAMATION);
        DoPC(PC_COMM_RESTART);
    }
}