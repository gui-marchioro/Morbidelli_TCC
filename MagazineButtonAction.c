#include "KMotionDef.h"

#ifndef TMP
#define TMP 10 // which spare persist to use to transfer data
#include "KflopToKMotionCNCFunctions.c"
#endif

#include "MillChanger.c"
#include "Magazine.c"
#include "InitConfig.c"
#include "Homing.c"

void UpdateMillingToolInUse();

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
        OpenMagazineNoWait();
        ClearBit(CLOSE_MAGAZINE_OUTPUT);
    }
    else
    {
        printf("Close Magazine request. MagazineButtonAction.\n");
        CloseMagazineNoWait();
        ClearBit(OPEN_MAGAZINE_OUTPUT);
        UpdateMillingToolInUse();
    }
}

void UpdateMillingToolInUse()
{
    float value = 0.0f;
    int Answer = InputBox("Tool in Spindle or -1",&value);
    int tool = (int) value;
    if (Answer)
    {
        printf("Operator Canceled\n");
        return 1;
    }
    else if (!ToolNumberValid(tool))  // check if invalid
    {
        char s[80];
        sprintf(s,"Invalid Current Tool Number %d\n",tool);
        printf(s);
        MsgBox(s, MB_ICONHAND | MB_OK);
        return 1;
    }
    else
    {
        persist.UserData[PREVIOUS_TOOL_VAR]=tool;
        FILE *f=fopen(TOOL_DISK_FILE,"wt");
        fprintf(f,"%d\n",tool);
        fclose(f);
        UpdateCurrentToolLabel(tool);
        printf("Operator Entered Value of %d\n",tool);
    }
}