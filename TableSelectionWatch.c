//#include "Homing.c"
//#include "InitConfig.c"
#include "MillChanger.h"
#include "KMotionDef.h"

#ifndef TMP
#define TMP 10 // which spare persist to use to transfer data
#include "KflopToKMotionCNCFunctions.c"
#endif

#define TABLE_EXECUTING_VAR 193
#define TABLE1_EXECUTING_VAR 194
#define TABLE2_EXECUTING_VAR 195

// Inputs to be monitored for manual tool exchange
#define TABLE1_BUTTON_INPUT 1100
#define TABLE2_BUTTON_INPUT 1101
#define VACUUM1_BUTTON_INPUT 1103
#define VACUUM2_BUTTON_INPUT 1105
#define VACUUM1_CONFIRMATION_INPUT 1107
#define VACUUM2_CONFIRMATION_INPUT 1108

// Outputs to be controled for tool exchange
#define AUXILIARY_POSITIONING1_OUTPUT 1070
#define AUXILIARY_POSITIONING2_OUTPUT 1071
#define VACUUM1_OUTPUT 48//1134
#define VACUUM2_OUTPUT 49//1135

// Posible states for the table selection
enum tableSelectionStates
{
    InitialState,
    TableSelected,
    ReadyToExecute,
    GoingToOrigin,
    ProgramExecution
};

int tableExecuting = 0;

// Move Axis XY at specified Speed and wait until complete
// return 0 = success, 1 if axis disabled
int MoveXY(float x, float y, float Speed)
{
	MoveAtVel(X_AXIS, x / FACTOR_X, Speed / FACTOR_X);
	MoveAtVel(Y_AXIS, y / FACTOR_Y, Speed / FACTOR_Y);
	
	return 0;  //success
}

#define DBTIME 300

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

Table1SelectionWatch()
{
    static enum tableSelectionStates states = InitialState; // Initializes the state machine
    static int tableButtonChangeState=-1,tlast=0,tlastsolid=-1,tcount=0; // initialize the table selection button state variables for switch debouncing
    static int vacuumButtonChangeState=-1,vlast=0,vlastsolid=-1,vcount=0; // initialize the vacuum button state variables for switch debouncing

    tableExecuting = persist.UserData[TABLE_EXECUTING_VAR];

    tableButtonChangeState = Debounce(ReadBit(TABLE1_BUTTON_INPUT),&tcount,&tlast,&tlastsolid);
    int tableButtonState = tlast;

    vacuumButtonChangeState = Debounce(ReadBit(VACUUM1_BUTTON_INPUT),&vcount,&vlast,&vlastsolid);
    int vacuumButtonState = vlast;

    switch (states)
    {
    case InitialState:
        if (tableButtonChangeState == 1)
        {
            SetBit(AUXILIARY_POSITIONING1_OUTPUT);
            states = TableSelected;
            printf("Going to Table1Selected. TableSelectionWatch.\n");
        }

        break;
    
    case TableSelected:
        if (vacuumButtonChangeState == 1)
        {
            SetBit(VACUUM1_OUTPUT);
        }
        else if (vacuumButtonChangeState == 0)
        {
            ClearBit(VACUUM1_OUTPUT);
        }

        if(tableButtonState == 1 && ReadBit(VACUUM1_CONFIRMATION_INPUT))
        {
            ClearBit(AUXILIARY_POSITIONING1_OUTPUT);
            states = ReadyToExecute;
            printf("Going to ReadyToExecute1. TableSelectionWatch.\n");
        }
        
        break;
    
    case ReadyToExecute:
        if (!JOB_ACTIVE && tableExecuting == 0)
        {
            MDI("G55");
            DoPC(PC_COMM_EXECUTE);
            persist.UserData[TABLE_EXECUTING_VAR] = 1;
            persist.UserData[TABLE1_EXECUTING_VAR] = 1;
            states = ProgramExecution;
            printf("Going to ProgramExecution1. TableSelectionWatch.\n");
        }

        break;

    case ProgramExecution:
        if (!JOB_ACTIVE && persist.UserData[TABLE1_EXECUTING_VAR] == 0)
        {
            ClearBit(VACUUM1_OUTPUT);
            states = InitialState;
            printf("Going to InitialState1. TableSelectionWatch.\n");
            persist.UserData[TABLE_EXECUTING_VAR] = 0;
        }

        break;

    default:
        states = InitialState;
        break;
    }
}

Table2SelectionWatch()
{
    static enum tableSelectionStates states = InitialState; // Initializes the state machine
    static int tableButtonChangeState=-1,tlast=0,tlastsolid=-1,tcount=0; // initialize the table selection button state variables for switch debouncing
    static int vacuumButtonChangeState=-1,vlast=0,vlastsolid=-1,vcount=0; // initialize the vacuum button state variables for switch debouncing

    tableExecuting = persist.UserData[TABLE_EXECUTING_VAR];

    tableButtonChangeState = Debounce(ReadBit(TABLE2_BUTTON_INPUT),&tcount,&tlast,&tlastsolid);
    int tableButtonState = tlast;

    vacuumButtonChangeState = Debounce(ReadBit(VACUUM2_BUTTON_INPUT),&vcount,&vlast,&vlastsolid);
    int vacuumButtonState = vlast;

    switch (states)
    {
    case InitialState:
        if (tableButtonChangeState == 1)
        {
            SetBit(AUXILIARY_POSITIONING2_OUTPUT);
            states = TableSelected;
            printf("Going to Table2Selected. TableSelectionWatch.\n");
        }

        break;
    
    case TableSelected:
        if (vacuumButtonChangeState == 1)
        {
            SetBit(VACUUM2_OUTPUT);
        }
        else if (vacuumButtonChangeState == 0)
        {
            ClearBit(VACUUM2_OUTPUT);
        }

        if(tableButtonState == 1 && ReadBit(VACUUM2_CONFIRMATION_INPUT))
        {
            ClearBit(AUXILIARY_POSITIONING2_OUTPUT);
            states = ReadyToExecute;
            printf("Going to ReadyToExecute2. TableSelectionWatch.\n");
        }
        
        break;
    
    case ReadyToExecute:
        if (!JOB_ACTIVE && tableExecuting == 0)
        {
            persist.UserData[TABLE_EXECUTING_VAR] = 2;
            persist.UserData[TABLE2_EXECUTING_VAR] = 1;
            MoveXY(3020,0,60);
            states = GoingToOrigin;
            printf("Going to GoingToOrigin2. TableSelectionWatch.\n");
        }

        break;

    case GoingToOrigin:
        if(CheckDone(X_AXIS) && CheckDone(Y_AXIS))
        {
            MDI("G56");
            ch0->InputGain0=1;
            ch0->OutputGain=1;
            DoPC(PC_COMM_EXECUTE);
            states = ProgramExecution;
            printf("Going to ProgramExecution2. TableSelectionWatch.\n");
        }
        break;

    case ProgramExecution:
        if (!JOB_ACTIVE &&  persist.UserData[TABLE2_EXECUTING_VAR] == 0)
        {
            MDI("G55");
            ch0->InputGain0=-1;
            ch0->OutputGain=-1;
            ClearBit(VACUUM2_OUTPUT);
            printf("Going to InitialState2. TableSelectionWatch.\n");
            states = InitialState;
            persist.UserData[TABLE_EXECUTING_VAR] = 0;
        }

        break;

    default:
        states = InitialState;
        break;
    }
}

/* if (GetInitExecuted() == 0 || GetHomingExecuted() == 0 || GetIsExecutingHoming() == 1)
{
    HaltAndWarn("Execute the Init and Homing function before run a program");
    return;
}
else if (ReadBit(MAGAZINE_OPENED_INPUT))
{
    HaltAndWarn("Close the tool magazine before run a program");
    return;
}
else if (JOB_ACTIVE)
{
    return;
}

void HaltAndWarn(char *message)
{
    DoPC(PC_COMM_HALT);
    MsgBox(message, MB_ICONEXCLAMATION);
    DoPC(PC_COMM_RESTART);
} */

