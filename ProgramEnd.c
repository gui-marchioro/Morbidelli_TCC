#include "TableSelectionWatch.c"
#include "KMotionDef.h"

main()
{
    if (persist.UserData[TABLE1_EXECUTING_VAR] == 1)
    {
        persist.UserData[TABLE1_EXECUTING_VAR] = 0;
    }

    if (persist.UserData[TABLE2_EXECUTING_VAR] == 1)
    {
        persist.UserData[TABLE2_EXECUTING_VAR] = 0;
    }
}