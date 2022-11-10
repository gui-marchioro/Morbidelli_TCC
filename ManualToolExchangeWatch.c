#include "ManualToolExchangeWatch.h"
#include "MillChangerDefs.h"
#include "KMotionDef.h"

// Posible states for the tool exchange cycle
enum toolExchangeStates
{
    InitialState,
    GripperOpenState,
    ExtractorOnState
};

// Debounce a bit
//
// return 1 one time when first debounced high 
// return 0 one time when first debounced low 
// return -1 otherwise 
int Debounce(int n, int *cnt, int *last, int *lastsolid)
{
	int v = -1;
	
	if (n == *last)  // same as last time?
	{
		if (*cnt == DBTIME-1)
		{
			if (n != *lastsolid)
			{
				v = *lastsolid = n;  // return debounced value
			}
		}
		if (*cnt < DBTIME)	(*cnt)++;
	}
	else
	{
		*cnt = 0;  // reset count
	}
	*last = n;
	return v;
}

#define PREVIOUS_TOOL_VAR 191
#define PREVIOUS_TOOL_LABEL_VAR 192
#define TOOL_DISK_FILE "c:\\Kmotion434\\KMotion\\Data\\ToolChangerData.txt"

// check if Current Tool number Valid
// -1 = no tool loaded
// 1-10 = valid tool
BOOL ToolNumberValid(int tool)
{
	return tool == -1 || (tool>=1 && tool<=10);
}

void UpdateCurrentToolLabel(int currentTool)
{
    char s[80];
    // Now compute and form result
	sprintf(s,"%d",currentTool);
    // Put it onto the Screen
	DROLabel(1000, PREVIOUS_TOOL_LABEL_VAR, s);
}

// Routine that monitors the inputs related to the manual tool exchange and runs the machine state
// that opens the gripper and the extractor
void ManualToolExchangeWatch(void)
{
    static enum toolExchangeStates states = InitialState; // Initializes the state machine
    static int blast=0,blastsolid=-1,bcount=0; // initialize the button state variables for switch debouncing
    static float value;
    static int Answer, tool;

    static double T0=0.0;  // remember the last time we turned on
    double T=Time_sec(); // get current Time_sec

    // Only monitors the next methods if no G Code or Homing is running
    // If not, clear the states machine and low the outputs
    if ((JOB_ACTIVE == 1) || (GetIsExecutingHoming() == 1))
    {
        //ClearBit(EXTRACT_OUTPUT); // commented because it generates a race condition while M6 command was executing
        //ClearBit(OPEN_TOOL_GRIPPER_OUTPUT);
        states = InitialState;
        return;
    }

    Debounce(ReadBit(EXCHANGE_TOOL_BUTTON_INPUT),&bcount,&blast,&blastsolid);
    int inputButtonState = blast;

    switch (states)
    {
        case InitialState:
            if (inputButtonState == 1)
            {
                SetBit(OPEN_TOOL_GRIPPER_OUTPUT);
                T0=T;  // set start time of cycle
                states = GripperOpenState;
                printf("Going to GripperOpenState. ManualToolExchangeWatch.\n");
            }
            // Keep InitialState, all outputs disabled
            break;
        case GripperOpenState:
            if (inputButtonState == 1)
            {
                if (T > T0 + GRIPPER_ON_STATE_TIME)  // are we out of the TIME_ON section of the cycle?
                {
                    T0 = 0.0;
                    SetBit(EXTRACT_OUTPUT);
                    states = ExtractorOnState;
                    printf("Going to ExtractorOnState. ManualToolExchangeWatch.\n");
                }
            }
            // Go back to InitialState, switch off outputs
            else
            {
                T0 = 0.0;
                ClearBit(OPEN_TOOL_GRIPPER_OUTPUT);
                states = InitialState;
                printf("Going to InitialState. ManualToolExchangeWatch.\n");
            }
            break;

        case ExtractorOnState:
            if (inputButtonState == 0)
            {
                ClearBit(EXTRACT_OUTPUT);
                ClearBit(OPEN_TOOL_GRIPPER_OUTPUT);
                states = InitialState;
                printf("Going to InitialState. ManualToolExchangeWatch.\n");

                Answer = InputBox("Tool in Spindle or -1",&value);
                tool = value;
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
            break;

        default:
            ClearBit(EXTRACT_OUTPUT);
            ClearBit(OPEN_TOOL_GRIPPER_OUTPUT);
            states = InitialState;
            break;
    }
}