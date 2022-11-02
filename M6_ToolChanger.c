#include "Drill.c"
#include "Spindle.c"
#include "KMotionDef.h"

#ifndef TMP
#define TMP 10 // which spare persist to use to transfer data
#include "KflopToKMotionCNCFunctions.c"
#endif

#define TOOL_VAR 9

#define PREVIOUS_TOOL_VAR 191
#define TOOL_DISK_FILE "c:\\Kmotion434\\KMotion\\Data\\ToolChangerData.txt"

#define MILL_TOOL_SLOT_MIN 1
#define MILL_TOOL_SLOT_MAX 10
#define TOOL_PRESENT_INPUT 1049
#define CLOSE_MAGAZINE_OUTPUT 1061
#define MAGAZINE_CLOSED_INPUT 1040
#define OPEN_MAGAZINE_OUTPUT 1062
#define MAGAZINE_OPENED_INPUT 1041
#define PISTON_TOOL_OUTPUT 1068
#define SPHERE_GRIPPER_OUTPUT 1065
#define EXTRACT_OUTPUT 1066
#define OPEN_TOOL_GRIPPER_OUTPUT 1067
#define PANTOGRAPH_OFF_SENSOR_INPUT 1042 //Pantografo recuado/alto
#define PANTOGRAPH_MOVING_SENSOR_INPUT 1043 //Pantografo baixo
#define PANTOGRAPH_ON_SENSOR_INPUT 1044 //Pantografo baixo


// Each axis have 2000 counts per revolution
// Resolution of X axis = 20 mm per revolution
// Resolution of Y axis = 20 mm per revolution
// Resolution of Z axis = 7.5 mm per revolution
// FACTOR_X = 20 /2000 = 0.01000 mm/counts
// FACTOR_Y = 20 /2000 = 0.01000 mm/counts
// FACTOR_Z = 7.5/2000 = 0.00375 mm/counts
#define FACTOR_X 0.01000
#define FACTOR_Y 0.01000
#define FACTOR_Z 0.00375
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2
#define OFFSET_X 20
#define OFFSET_Y -20
#define OFFSET_Z -7.5

#define SLOW_SPEED 500

// Routine to be executed when M6 TX is called in a G program. (X is the tool number)
void main ()
{
    // int previousSlot = persist.UserData[PREVIOUS_TOOL_VAR];
    int slot = persist.UserData[TOOL_VAR];  // Requested tool slot. Value stored is an int 
	// int id = persist.UserData[TOOL_VAR+1];  // value stored is an int 
	// printf("Tool Set to slot %d id %d\n",slot,id);  // print the slot and id

    ClearDrillOutputs();

    // Turn off spindle if on.
    if (GetSpindleStatus() != SPINDLE_OFF)
    {
        MDI("M5");
    }

    if(slot >= MILL_TOOL_SLOT_MIN && slot <= MILL_TOOL_SLOT_MAX)
    {
        if (MillExchangeRoutine(slot))
        {
            // error, Halt Job
		    DoPC(PC_COMM_HALT);
        }
    }
    // Drill box routine
    else if (slot >= DRILL_BOX_SLOT_MIN && slot <= DRILL_BOX_SLOT_MAX)
    {
        DrillRoutine(slot);
    }
    else
    {
        MsgBox("The selected tool is out of the range. Please select one that is defined inside the tool table.", MB_ICONEXCLAMATION);
    }
}

// Exchange the milling tool. Return 0=Success, 1=Failure
int MillExchangeRoutine(int millSlot)
{
    int currentTool;

    //  -1=Spindle empty, 0=unknown, 1-4 Tool Slot loaded into Spindle
    if (GetCurrentTool(&currentTool))
    {
        return 1;
    }

    printf("Load Tool Slot %d requested, Current Tool %d\n",millSlot, currentTool);
	
	if (!ToolNumberValid(millSlot))  // check if invalid
	{
		char s[80];
		sprintf(s,"Invalid Tool Change Number %d\n", millSlot);
		printf(s);
		MsgBox(s, MB_ICONHAND | MB_OK);
		return 1;
	}

    OpenMagazine();
	
	if (currentTool != -1) // is there a tool in the Spindle??
    {
        if (UnloadTool(currentTool)) // yes, unload it
        {
            return 1;
        }
    }
		
	// Now Spindle is empty, load requested tool
	if (LoadNewTool(millSlot))
    {
        return 1;
    }

    CloseMagazine();
	
	SaveCurrentTool(millSlot);  // save the one that has been loaded
	return 0;  // success
}

void OpenMagazine()
{
    SetBit(OPEN_MAGAZINE_OUTPUT);
    Delay_sec(2.0);
    ClearBit(OPEN_MAGAZINE_OUTPUT);
}

void CloseMagazine()
{
    SetBit(CLOSE_MAGAZINE_OUTPUT);
    Delay_sec(2.0);
    ClearBit(CLOSE_MAGAZINE_OUTPUT);
}

void SaveCurrentTool(int millSlot)
{
    persist.UserData[PREVIOUS_TOOL_VAR] = millSlot;
	FILE *f=fopen(TOOL_DISK_FILE,"wt");
	fprintf(f,"%d\n", millSlot);
	fclose(f);
	return 0;
}

// Get the last loaded tool.  Parameter points to where to return tool
// First try to get from KFLOP memory
// if memory is invalid, try to read from disk
// if can't read disk then ask Operator
// returns 0 on success, 1 on fail or Operator asked to abort
int GetCurrentTool(int *ptool)
{
	int success, Answer, result, tool;
	float value;

	tool = persist.UserData[PREVIOUS_TOOL_VAR];
	success = ToolNumberValid(tool);  // check if valid

	if (!success)   // invalid after power up, try to read from PC Disk File
	{
		// Try to open file
		FILE *f = fopen(TOOL_DISK_FILE, "rt");
		if (f)  // did file open?
		{
			// read a line and convert it
			result = fscanf(f, "%d", &tool);
			fclose(f);
			
			if (result == 1 && ToolNumberValid(tool))
			{
				printf("Read Disk File Value of %d\n", tool);
				success = TRUE; // success if one value converted
			}
		}
		
		if (!success) printf("Unable to open/read file:%s\n",TOOL_DISK_FILE);  
	}

	if (!success)   // if still no success ask Operator
	{
		Answer = InputBox("Tool in Spindle or -1",&value);
		if (Answer)
		{
			printf("Operator Canceled\n");
			return 1;
		}
		else
		{
			tool = value;
			printf("Operator Entered Value of %d\n",tool);
		}
	}

	if (!ToolNumberValid(tool))  // check if invalid
	{
		char s[80];
		sprintf(s,"Invalid Current Tool Number %d\n",tool);
		printf(s);
		MsgBox(s, MB_ICONHAND | MB_OK);
		return 1;
	}
	
	printf("Current tool = %d\n", tool);
	*ptool = tool;  // return result to caller
	return 0;  //success
}

// check if Current Tool number Valid
// -1 = no tool loaded
// 1-10 = valid tool
BOOL ToolNumberValid(int tool)
{
	return tool == -1 || (tool>=1 && tool<=10);
}

int UnloadTool(int currentTool)
{
    // - Rapid to Z Home to clear any work that may be on the table
	if (MoveZ(0.0, SLOW_SPEED))
    {
        return 1;
    }

    // - Rapid to current tool position to execute Z move
	if (MoveXY(ToolPositionX(currentTool), ToolPositionY(currentTool), SLOW_SPEED))
    {
        return 1;
    }

	// - Approach tool holder by matching Z height of tool flange currently in spindle with tool holder                            claw
	if (MoveZ(ToolPositionZ(), SLOW_SPEED))
    {
        return 1;
    }

    // Activate piston actuators to approach tool in z axis.
    EnablePistons();

    // - Eject tool
	if (EjectTool())
    {
        return 1;
    }

    DisablePistons();

    // - Rapid to Z Home
	if (MoveZ(0.0, SLOW_SPEED))
    {
        return 1;
    }

	return 0; //success
}

int LoadNewTool(int newTool)
{
    // - Rapid to Z Home to clear any work that may be on the table
	if (MoveZ(0.0, SLOW_SPEED))
    {
        return 1;
    }

    // - Rapid to new tool position to execute Z move
	if (MoveXY(ToolPositionX(newTool), ToolPositionY(newTool), SLOW_SPEED))
    {
        return 1;
    }

	// - Approach tool holder by matching Z + safe height of tool flange currently in spindle with tool holder                            claw
	if (MoveZ(ToolPositionZ() + 5.0, SLOW_SPEED))
    {
        return 1;
    }

    // Activate piston actuators to approach tool in z axis.
    EnablePistons();

    ClearBit(SPHERE_GRIPPER_OUTPUT);
    ClearBit(OPEN_TOOL_GRIPPER_OUTPUT);

    if (MoveZ(ToolPositionZ(), SLOW_SPEED))
    {
        return 1;
    }

    // - Grab tool
	if (GrabTool())
    {
        return 1;
    }
    
    DisablePistons();
    // - Rapid to Z Home
	if (MoveZ(0.0, SLOW_SPEED))
    {
        return 1;
    }

	return 0; //success
}

float ToolPositionX(int tool)
{
    float xOffset = 0;

    if (tool % 2 == 0)
    {
        xOffset = 120.0;
    }

    float xPosition = 23.6 + OFFSET_X + xOffset;
    return xPosition;
}

float ToolPositionY(int tool)
{
    float yOffset = 0;

    if (tool > 6)
    {
        yOffset = 160*3;
    }
    else if (tool > 4)
    {
        yOffset = 160*2;
    }
    else if (tool > 2)
    {
        yOffset = 160*1;
    }

    float yPosition = -876.2 + OFFSET_Y + yOffset;
    return yPosition;
}

float ToolPositionZ()
{
    float zPosition = 3.15 + OFFSET_Z;
    return zPosition;
}

// Move Axis XY at specified Speed and wait until complete
// return 0 = success, 1 if axis disabled
int MoveXY(float x, float y, float Speed)
{
	MoveAtVel(X_AXIS, x / FACTOR_X, Speed * FACTOR_X);
	MoveAtVel(Y_AXIS, y / FACTOR_Y, Speed * FACTOR_Y);
	
	while (!CheckDone(X_AXIS) || !CheckDone(Y_AXIS))
	{
		if (!chan[X_AXIS].Enable)
		{
			printf("Error X Axis Disabled\n");
			MsgBox("Error X Axis Disabled\n", MB_ICONHAND | MB_OK);
			return 1;
		}
		if (!chan[Y_AXIS].Enable)
		{
			printf("Error Y Axis Disabled\n");
			MsgBox("Error Y Axis Disabled\n", MB_ICONHAND | MB_OK);
			return 1;
		}
	}
	return 0;  //success
}

// Move Axis Z at specified Speed and wait until complete
// return 0 = success, 1 if axis disabled
int MoveZ(float z, float Speed)
{
	MoveAtVel(Z_AXIS, z / FACTOR_Z, Speed / FACTOR_Z);
	
	while (!CheckDone(Z_AXIS))
	{
		if (!chan[Z_AXIS].Enable)
		{
			printf("Error Z Axis Disabled\n");
			MsgBox("Error Z Axis Disabled\n", MB_ICONHAND | MB_OK);
			return 1;
		}
	}
	return 0;  //success
}

void EnablePistons()
{
    SetBit(PISTONS_ENABLE_OUTPUT);
    SetBit(PISTONL_ENABLE_OUTPUT);
    SetBit(PISTON_TOOL_OUTPUT);

    Delay_sec(2.0);
}

void DisablePistons()
{
    ClearBit(PISTONS_ENABLE_OUTPUT);
    ClearBit(PISTONL_ENABLE_OUTPUT);
    ClearBit(PISTON_TOOL_OUTPUT);

    Delay_sec(2.0);
}

int EjectTool()
{
    ClearBit(SPHERE_GRIPPER_OUTPUT);
    ClearBit(OPEN_TOOL_GRIPPER_OUTPUT);

    // - Wait for time in seconds defined by CLAMP_TIME
	Delay_sec(1.0);

    // - Read TOOL_PRESENT_INPUT bit to see whether the tool is loose, to make a safe Z move without  
    //      destroying tool holder
	// - If TOOL_PRESENT_INPUT bit is high, something has gone wrong;
	//		halt everything and display message indicating failure
	if (ReadBit(TOOL_PRESENT_INPUT))
	{
		printf("Claw Loose Error\n");
		MsgBox("Claw Loose Error\n", MB_ICONHAND | MB_OK);
		return 1;
	}

    return 0; // success
}

int GrabTool()
{
    SetrBit(SPHERE_GRIPPER_OUTPUT);
    SetBit(OPEN_TOOL_GRIPPER_OUTPUT);

    // - Wait for time in seconds defined by CLAMP_TIME
	Delay_sec(1.0);

    // - Read TOOL_PRESENT_INPUT bit to see whether the tool is loose, to make a safe Z move without  
    //      destroying tool holder
	// - If TOOL_PRESENT_INPUT bit is high, something has gone wrong;
	//		halt everything and display message indicating failure
	if (!ReadBit(TOOL_PRESENT_INPUT))
	{
		printf("Error while grabbing tool\n");
		MsgBox("Error while grabbing tool\n", MB_ICONHAND | MB_OK);
		return 1;
	}

    return 0; // success
}