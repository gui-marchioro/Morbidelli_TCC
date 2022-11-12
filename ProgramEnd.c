#include "TableSelectionWatch.h"
#include "KMotionDef.h"

main()
{
    // Reset executing state from table 1 and 2.
    if (persist.UserData[TABLE1_EXECUTING_VAR] == 1)
    {
        persist.UserData[TABLE1_EXECUTING_VAR] = 0;
    }

    if (persist.UserData[TABLE2_EXECUTING_VAR] == 1)
    {
        persist.UserData[TABLE2_EXECUTING_VAR] = 0;
    }

    if (persist.UserData[TABLE12_EXECUTING_VAR] == 1)
    {
        persist.UserData[TABLE12_EXECUTING_VAR] = 0;
    }
}