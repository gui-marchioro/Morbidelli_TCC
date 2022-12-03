//#include "Homing.c"
//#include "InitConfig.c"
#include "MovementDefs.h"
#include "TableSelectionWatch.h"
#include "KMotionDef.h"

#ifndef TMP
#define TMP 10 // which spare persist to use to transfer data
#include "KflopToKMotionCNCFunctions.c"
#endif

// Posible states for the table selection
enum tableSelectionStates
{
    InitialState,
    TableSelected,
    ReadyToExecute,
    SafeZBegin,
    ProgramExecution,
    SafeZEnd
};

int tableExecuting = 0;

void ClearTableOutputs()
{
    // Table 1
    ClearBit(AUXILIARY_POSITIONING1_OUTPUT);
    ClearBit(VACUUM1_OUTPUT);
    // Table 2
    ClearBit(AUXILIARY_POSITIONING2_OUTPUT);
    ClearBit(AUXILIARY_POSITIONING_MIDDLE_OUTPUT);
    ClearBit(VACUUM2_OUTPUT);
}

// Move Axis X at specified Speed
// return 0 = success, 1 if axis disabled
int MoveX(float x, float Speed)
{
	MoveAtVel(X_AXIS, x / FACTOR_X, Speed / FACTOR_X);
	
	return 0;  //success
}

// Move Axis Z at specified Speed
// return 0 = success, 1 if axis disabled
int MoveZ(float z, float Speed)
{
	MoveAtVel(Z_AXIS, z / FACTOR_Z, Speed / FACTOR_Z);
	
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

    // To reset outputs when an init is performed
    static int firstTime = 1;
    if (firstTime == 1)
    {
        ClearBit(AUXILIARY_POSITIONING1_OUTPUT);
        ClearBit(VACUUM1_OUTPUT);
        firstTime = 0;
    }

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
            if (ReadBit(VACUUM1_OUTPUT) == 0)
            {
                SetBit(VACUUM1_OUTPUT);
            }
            else
            {
                ClearBit(VACUUM1_OUTPUT);
            }
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
            persist.UserData[TABLE_EXECUTING_VAR] = 1;
            persist.UserData[TABLE1_EXECUTING_VAR] = 1;
            MoveZ(0, 10);
            states = SafeZBegin;
            printf("Going to SafeZBegin1. TableSelectionWatch.\n");
        }

        break;

    case SafeZBegin:
        if(CheckDone(Z_AXIS))
        {
            MDI("G55");
            DoPC(PC_COMM_EXECUTE);
            states = ProgramExecution;
            printf("Going to ProgramExecution1. TableSelectionWatch.\n");
        }

        break;

    case ProgramExecution:
        if (!JOB_ACTIVE && persist.UserData[TABLE1_EXECUTING_VAR] == 0)
        {
            MoveZ(0, 10);
            states = SafeZEnd;
            printf("Going to SafeZEnd1. TableSelectionWatch.\n");
        }

        break;

    case SafeZEnd:
        if(CheckDone(Z_AXIS))
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

    // To reset outputs when an init is performed
    static int firstTime = 1;
    if (firstTime == 1)
    {
        ClearBit(AUXILIARY_POSITIONING2_OUTPUT);
        ClearBit(AUXILIARY_POSITIONING_MIDDLE_OUTPUT);
        ClearBit(VACUUM2_OUTPUT);
        firstTime = 0;
    }

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
            SetBit(AUXILIARY_POSITIONING_MIDDLE_OUTPUT);
            states = TableSelected;
            printf("Going to Table2Selected. TableSelectionWatch.\n");
        }

        break;
    
    case TableSelected:
        if (vacuumButtonChangeState == 1)
        {
            if (ReadBit(VACUUM2_OUTPUT) == 0)
            {
                SetBit(VACUUM2_OUTPUT);
            }
            else
            {
                ClearBit(VACUUM2_OUTPUT);
            }
        }

        if(tableButtonState == 1 && ReadBit(VACUUM2_CONFIRMATION_INPUT))
        {
            ClearBit(AUXILIARY_POSITIONING2_OUTPUT);
            ClearBit(AUXILIARY_POSITIONING_MIDDLE_OUTPUT);
            states = ReadyToExecute;
            printf("Going to ReadyToExecute2. TableSelectionWatch.\n");
        }
        
        break;
    
    case ReadyToExecute:
        if (!JOB_ACTIVE && tableExecuting == 0)
        {
            persist.UserData[TABLE_EXECUTING_VAR] = 2;
            persist.UserData[TABLE2_EXECUTING_VAR] = 1;
            MoveZ(0, 10);
            states = SafeZBegin;
            printf("Going to SafeZBegin2. TableSelectionWatch.\n");
        }

        break;

    case SafeZBegin:
        if(CheckDone(Z_AXIS))
        {
            MDI("G56");
            DoPC(PC_COMM_EXECUTE);
            states = ProgramExecution;
            printf("Going to ProgramExecution2. TableSelectionWatch.\n");
        }

        break;

    case ProgramExecution:
        if (!JOB_ACTIVE && persist.UserData[TABLE2_EXECUTING_VAR] == 0)
        {
            MoveZ(0, 10);
            states = SafeZEnd;
            printf("Going to SafeZEnd2. TableSelectionWatch.\n");
        }

        break;

    case SafeZEnd:
        if(CheckDone(Z_AXIS))
        {
            ClearBit(VACUUM2_OUTPUT);
            states = InitialState;
            printf("Going to InitialState2. TableSelectionWatch.\n");
            persist.UserData[TABLE_EXECUTING_VAR] = 0;
        }
        break;

    default:
        states = InitialState;
        break;
    }
}

Table1And2SelectionWatch()
{
    static enum tableSelectionStates states = InitialState; // Initializes the state machine
    static int tableButtonChangeState1=-1,tlast1=0,tlastsolid1=-1,tcount1=0; // initialize the table selection button state variables for switch debouncing
    static int tableButtonChangeState2=-1,tlast2=0,tlastsolid2=-1,tcount2=0; // initialize the table selection button state variables for switch debouncing
    static int vacuumButtonChangeState1=-1,vlast1=0,vlastsolid1=-1,vcount1=0; // initialize the vacuum button state variables for switch debouncing
    static int vacuumButtonChangeState2=-1,vlast2=0,vlastsolid2=-1,vcount2=0; // initialize the vacuum button state variables for switch debouncing

    tableExecuting = persist.UserData[TABLE_EXECUTING_VAR];

    tableButtonChangeState1 = Debounce(ReadBit(TABLE12_BUTTON_INPUT_1),&tcount1,&tlast1,&tlastsolid1);
    int tableButtonState1 = tlast1;
    tableButtonChangeState2 = Debounce(ReadBit(TABLE12_BUTTON_INPUT_2),&tcount2,&tlast2,&tlastsolid2);
    int tableButtonState2 = tlast2;

    vacuumButtonChangeState1 = Debounce(ReadBit(VACUUM12_BUTTON_INPUT_1),&vcount1,&vlast1,&vlastsolid1);
    int vacuumButtonState1 = vlast1;
    vacuumButtonChangeState2 = Debounce(ReadBit(VACUUM12_BUTTON_INPUT_2),&vcount2,&vlast2,&vlastsolid2);
    int vacuumButtonState2 = vlast2;

    switch (states)
    {
    case InitialState:
        if (tableButtonChangeState1 == 1 || tableButtonChangeState2 == 1)
        {
            SetBit(AUXILIARY_POSITIONING1_OUTPUT);
            SetBit(AUXILIARY_POSITIONING2_OUTPUT);
            states = TableSelected;
            printf("Going to Table12Selected. TableSelectionWatch.\n");
        }

        break;
    
    case TableSelected:
        if (vacuumButtonChangeState1 == 1 || vacuumButtonChangeState2 == 1)
        {
            if (ReadBit(VACUUM1_OUTPUT) == 0 || ReadBit(VACUUM2_OUTPUT) == 0)
            {
                SetBit(VACUUM1_OUTPUT);
                SetBit(VACUUM2_OUTPUT);
            }
            else
            {
                ClearBit(VACUUM1_OUTPUT);
                ClearBit(VACUUM2_OUTPUT);
            }
        }

        if((tableButtonState1 == 1 || tableButtonChangeState2 == 1) && ReadBit(VACUUM1_CONFIRMATION_INPUT) && ReadBit(VACUUM2_CONFIRMATION_INPUT) && tableExecuting == 0)
        {
            ClearBit(AUXILIARY_POSITIONING1_OUTPUT);
            ClearBit(AUXILIARY_POSITIONING2_OUTPUT);
            states = ReadyToExecute;
            printf("Going to ReadyToExecute12. TableSelectionWatch.\n");
        }
        
        break;
    
    case ReadyToExecute:
        if (!JOB_ACTIVE && tableExecuting == 0)
        {
            persist.UserData[TABLE_EXECUTING_VAR] = 12;
            persist.UserData[TABLE12_EXECUTING_VAR] = 1;
            MoveZ(0, 10);
            states = SafeZBegin;
            printf("Going to SafeZBegin12. TableSelectionWatch.\n");
        }

        if (tableExecuting != 0)
        {
            states = InitialState;
            printf("Going to InitialState12. TableSelectionWatch.\n");
        }

        break;

    case SafeZBegin:
        if(CheckDone(Z_AXIS))
        {
            MDI("G55");
            DoPC(PC_COMM_EXECUTE);
            states = ProgramExecution;
            printf("Going to ProgramExecution12. TableSelectionWatch.\n");
        }

        break;

    case ProgramExecution:
        if (!JOB_ACTIVE && persist.UserData[TABLE12_EXECUTING_VAR] == 0)
        {
            MoveZ(0, 10);
            states = SafeZEnd;
            printf("Going to SafeZEnd12. TableSelectionWatch.\n");
        }

        break;

    case SafeZEnd:
        if(CheckDone(Z_AXIS))
        {
            ClearBit(VACUUM1_OUTPUT);
            ClearBit(VACUUM2_OUTPUT);
            states = InitialState;
            printf("Going to InitialState12. TableSelectionWatch.\n");
            persist.UserData[TABLE_EXECUTING_VAR] = 0;
        }
        break;

    default:
        states = InitialState;
        break;
    }
}