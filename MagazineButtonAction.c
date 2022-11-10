#include "MillChanger.c"
#include "InitConfig.c"
#include "Homing.c"
#include "KMotionDef.h"

#ifndef TMP
#define TMP 10 // which spare persist to use to transfer data
#include "KflopToKMotionCNCFunctions.c"
#endif

main()
{
    // Conditions to not execute command
    if (GetInitExecuted() == 0 || GetHomingExecuted() == 0 || GetIsExecutingHoming() == 1 || JOB_ACTIVE)
    {
        MsgBoxNoWait("Execute the initialization and homing before execute this command.", MB_ICONASTERISK);
        return;
    }

    if (ReadBit(MAGAZINE_CLOSED_INPUT))
    {
        printf("Open Magazine request. MagazineButtonAction.\n");
        OpenMagazine();
    }
    else
    {
        printf("Close Magazine request. MagazineButtonAction.\n");
        CloseMagazine();
    }
}