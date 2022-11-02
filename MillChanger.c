#include "MillChanger.h"
#include "MillChangerDefs.h"
#include "Spindle.h"
#include "KMotionDef.h"

// Routine responsible for exchange mill tool.
// Return 0=Success, 1=Failure
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

// Opens the tool magazine.
void OpenMagazine()
{
    SetBit(OPEN_MAGAZINE_OUTPUT);
    Delay_sec(2.0);
    ClearBit(OPEN_MAGAZINE_OUTPUT);
}

// Closes the tool magazine.
void CloseMagazine()
{
    SetBit(CLOSE_MAGAZINE_OUTPUT);
    Delay_sec(2.0);
    ClearBit(CLOSE_MAGAZINE_OUTPUT);
}

// save the tool number to KFLOP global Variable and to PC Disk file in case power is lost.
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

// Remove tool in spindle by going to holder of current tool.
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

// Load new Tool (Spindle must be empty).
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

// Return x position of tool holder as a function of the tool in mm.
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

// Return y position of tool holder as a function of the tool in mm.
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

// Return z position of tool holder in mm.
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

// Activate piston actuators to approach tool holder in z axis.
void EnablePistons()
{
    SetBit(PISTONS_ENABLE_OUTPUT);
    SetBit(PISTONL_ENABLE_OUTPUT);
    SetBit(PISTON_TOOL_OUTPUT);

    Delay_sec(2.0);
}

// Disable piston actuators.
void DisablePistons()
{
    ClearBit(PISTONS_ENABLE_OUTPUT);
    ClearBit(PISTONL_ENABLE_OUTPUT);
    ClearBit(PISTON_TOOL_OUTPUT);

    Delay_sec(2.0);
}

// Eject milling tool.
// return 0 = success, 1 if sensor points that the tool is still grabbed.
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

// Grab milling tool.
// return 0 = success, 1 if sensor points that the tool is not grabbed.
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