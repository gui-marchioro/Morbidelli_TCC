#include "Homing.c"
#include "InitConfig.c"
#include "MillChanger.h"
#include "KMotionDef.h"

#define TMP 10 // which spare persist to use to transfer data
#include "KflopToKMotionCNCFunctions.c"

// Program to be executed when the Cycle Start button is pressed
// DoPC(PC_COMM_EXECUTE);
void main()
{
    if (GetInitExecuted() == 0 || GetHomingExecuted() == 0 || GetIsExecutingHoming() == 1)
    {
        HaltAndWarn("Execute the Init and Homing function before run a program");
    }
    else if (ReadBit(MAGAZINE_OPENED_INPUT))
    {
        HaltAndWarn("Close the tool magazine before run a program");
    }
}

void HaltAndWarn(char *message)
{
    DoPC(PC_COMM_HALT);
    Delay_sec(1);
    MsgBoxNoWait(message, MB_ICONEXCLAMATION);
    Delay_sec(1);
    DoPC(PC_COMM_RESTART);
}