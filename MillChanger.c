#include "KMotionDef.h"
#include "MillChanger.h"
#include "MillChangerDefs.h"
#include "MovementDefs.h"
#include "Spindle.h"
#include "Magazine.h"

// check if Current Tool number Valid
// -1 = no tool loaded
// 1-10 = valid tool
BOOL ToolNumberValid(int tool);

// Routine responsible for exchange mill tool.
// Return 0=Success, 1=Failure
int MillExchangeRoutine(int millSlot);

// save the tool number to KFLOP global Variable and to PC Disk file in case power is lost.
void SaveCurrentTool(int millSlot);

// Get the last loaded tool.  Parameter points to where to return tool
// First try to get from KFLOP memory
// if memory is invalid, try to read from disk
// if can't read disk then ask Operator
// returns 0 on success, 1 on fail or Operator asked to abort
int GetCurrentTool(int *ptool);

// Remove tool in spindle by going to holder of current tool.
int UnloadTool(int currentTool);

// Load new Tool (Spindle must be empty).
int LoadNewTool(int newTool, int hasUnloadedTool);

// Return x position of tool holder as a function of the tool in mm.
float ToolPositionX(int tool);

// Return y position of tool holder as a function of the tool in mm.
float ToolPositionY(int tool);

// Return z position of tool holder in mm.
float ToolPositionZ();

// Move Axis XY at specified Speed and wait until complete
// return 0 = success, 1 if axis disabled
int MoveXY(float x, float y, float Speed);

// Move Axis Z at specified Speed and wait until complete
// return 0 = success, 1 if axis disabled
int MoveZ(float z, float Speed);

// Activate piston actuators to approach tool holder in z axis.
void EnablePistons();

// Disable piston actuators.
void DisablePistons();

// Eject milling tool.
// return 0 = success, 1 if sensor points that the tool is still grabbed.
int EjectTool();

// Grab milling tool.
// return 0 = success, 1 if sensor points that the tool is not grabbed.
int GrabTool();

// Routine responsible for exchange mill tool.
// Return 0=Success, 1=Failure
int MillExchangeRoutine(int millSlot)
{
    int hasUnloadedTool = 0;
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

    // If the tool requested is already in the spindle return.
    if (millSlot == currentTool)
    {
        return 0;
    }

    printf("Open Magazine. MillChanger.\n");
    OpenMagazineNoWait();
	
	if (currentTool != -1) // is there a tool in the Spindle??
    {
        printf("Unload tool. MillChanger.\n");
        if (UnloadTool(currentTool)) // yes, unload it
        {
            return 1;
        }

        hasUnloadedTool = 1;
    }
		
	// Now Spindle is empty, load requested tool
    printf("Load new tool. MillChanger.\n");
	if (LoadNewTool(millSlot, hasUnloadedTool))
    {
        return 1;
    }

    printf("Close Magazine. MillChanger.\n");
    CloseMagazineNoWait();
	
	SaveCurrentTool(millSlot);  // save the one that has been loaded
	return 0;  // success
}

// save the tool number to KFLOP global Variable and to PC Disk file in case power is lost.
void SaveCurrentTool(int millSlot)
{
    persist.UserData[PREVIOUS_TOOL_VAR] = millSlot;
	FILE *f=fopen(TOOL_DISK_FILE,"wt");
	fprintf(f,"%d\n", millSlot);
	fclose(f);
    UpdateCurrentToolLabel(millSlot);
}

void UpdateCurrentToolLabel(int currentTool)
{
    char s[80];
    // Now compute and form result
	sprintf(s,"%d",currentTool);
    // Put it onto the Screen
	DROLabel(1000, PREVIOUS_TOOL_LABEL_VAR, s);
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

// Posible states to unload tool cycle
enum unloadStates
{
    UnloadSafeZ,
    MovePlaneUnloadTool,
    UnloadApproachZ,
    UnloadApproachPistons,
    UnloadEject,
    UnloadEnd,
    UnloadError,
    UnloadErrorEnd
};

int UnloadTool(int currentTool)
{
    enum unloadStates states = UnloadSafeZ;
    printf("Going to UnloadSafeZ. Unload tool. MillChanger.\n");
    double T0=0.0;  // remember the last time we turned on

    do
    {
        double T=Time_sec(); // get current Time_sec
        
        if (!JOB_ACTIVE)
        {
            printf("Going to UnloadError. Unload tool. MillChanger.\n");
            states = UnloadError;
        }

        switch (states)
        {
        case UnloadSafeZ:
            // - Rapid to Z Home to clear any work that may be on the table
            if (MoveZ(0.0, FAST_SPEED))
            {
                printf("Going to UnloadError. Unload tool. MillChanger.\n");
                states = UnloadError;
                break;
            }
            
            printf("Going to MovePlaneUnloadTool. Unload tool. MillChanger.\n");
            states = MovePlaneUnloadTool;
            break;
        
        case MovePlaneUnloadTool:
            // - Rapid to current tool position to execute Z move
            if (MoveXY(ToolPositionX(currentTool), ToolPositionY(currentTool), FAST_SPEED))
            {
                printf("Going to UnloadError. Unload tool. MillChanger.\n");
                states = UnloadError;
                break;
            }

            printf("Going to UnloadApproachZ. Unload tool. MillChanger.\n");
            states = UnloadApproachZ;
            break;

        case UnloadApproachZ:
            // Does not approach in the magazine if not is not opened
            if (!ReadBit(MAGAZINE_OPENED_INPUT))
            {
                break;
            }
            // - Approach tool holder by matching Z height of tool flange currently in spindle with tool holder                            claw
            if (MoveZ(ToolPositionZ(), FAST_SPEED))
            {
                printf("Going to UnloadError. Unload tool. MillChanger.\n");
                states = UnloadError;
                break;
            }

            printf("Going to UnloadApproachPistons. Unload tool. MillChanger.\n");
            T0=T;  // set start time of cycle
            states = UnloadApproachPistons;
            break;

        case UnloadApproachPistons:
            // Activate piston actuators to approach tool in z axis.
            SetBit(PISTONS_ENABLE_OUTPUT);
            SetBit(PISTONL_ENABLE_OUTPUT);
            if (T > T0 + 2.0)
            {
                T0 = 0.0;
                printf("Going to UnloadEject. Unload tool. MillChanger.\n");
                states = UnloadEject;
            }

            break;

        case UnloadEject:
            // - Eject tool
            if (EjectTool())
            {
                printf("Going to UnloadError. Unload tool. MillChanger.\n");
                states = UnloadError;
                break;
            }

            printf("Going to UnloadEnd. Unload tool. MillChanger.\n");
            states = UnloadEnd;
            break;
        
        case UnloadError:
            ClearStopImmediately(); // Clear Stop Condition without resuming
            ClearBit(PISTONS_ENABLE_OUTPUT);
            ClearBit(PISTONL_ENABLE_OUTPUT);
            printf("Going to UnloadErrorEnd. Unload tool. MillChanger.\n");
            states = UnloadErrorEnd;
            break;

        default:
            break;
        }

        WaitNextTimeSlice();
    } while (states != UnloadEnd && states != UnloadErrorEnd);

    if (states == UnloadEnd)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

// Posible states to load tool cycle
enum loadStates
{
    LoadInitial,
    LoadSafeZBegin,
    MovePlaneLoadTool,
    LoadApproachZ,
    LoadApproachPistons,
    LoadGrab,
    LoadRetractPistons,
    LoadSafeZEnd,
    LoadEnd,
    LoadError,
    LoadErrorEnd
};

// Load new Tool (Spindle must be empty).
int LoadNewTool(int newTool, int hasUnloadedTool)
{
    enum loadStates states = LoadInitial;
    printf("Going to LoadInitial. Load new tool. MillChanger.\n");
    double T0=0.0;  // remember the last time we turned on

    do
    {
        double T=Time_sec(); // get current Time_sec
        
        if (!JOB_ACTIVE)
        {
            printf("Going to LoadError. Load new tool. MillChanger.\n");
            states = LoadError;
        }

        switch (states)
        {
        case LoadInitial:
            if (hasUnloadedTool)
            {
                printf("Going to MovePlaneLoadTool. Load new tool. MillChanger.\n");
                states = MovePlaneLoadTool;
            }
            else
            {
                printf("Going to LoadSafeZBegin. Load new tool. MillChanger.\n");
                states = LoadSafeZBegin;
            }
            break;

        case LoadSafeZBegin:
            // - Rapid to Z Home to clear any work that may be on the table
            if (MoveZ(0.0, FAST_SPEED))
            {
                printf("Going to LoadError. Load new tool. MillChanger.\n");
                states = LoadError;
                break;
            }

            printf("Going to MovePlaneLoadTool. Load new tool. MillChanger.\n");
            states = MovePlaneLoadTool;
            break;
        
        case MovePlaneLoadTool:
            // - Rapid to new tool position to execute Z move
            if (MoveXY(ToolPositionX(newTool), ToolPositionY(newTool), FAST_SPEED))
            {
                printf("Going to LoadError. Load new tool. MillChanger.\n");
                states = LoadError;
                break;
            }
            
            printf("Going to LoadApproachZ. Load new tool. MillChanger.\n");
            states = LoadApproachZ;
            break;

        case LoadApproachZ:
            // Does not approach in the magazine if not is not opened
            if (!ReadBit(MAGAZINE_OPENED_INPUT))
            {
                break;
            }

            if (MoveZ(ToolPositionZ(), FAST_SPEED))
            {
                printf("Going to LoadError. Load new tool. MillChanger.\n");
                states = LoadError;
                break;
            }

            if (hasUnloadedTool)
            {
                printf("Going to LoadGrab. Load new tool. MillChanger.\n");
                states = LoadGrab;
            }
            else
            {
                printf("Going to LoadApproachPistons. Load new tool. MillChanger.\n");
                states = LoadApproachPistons;
            }
            break;

        case LoadApproachPistons:
            // Activate piston actuators to approach tool in z axis.
            SetBit(PISTONS_ENABLE_OUTPUT);
            SetBit(PISTONL_ENABLE_OUTPUT);
            if (T > T0 + 2.0)
            {
                T0 = 0.0;
                printf("Going to LoadGrab. Load new tool. MillChanger.\n");
                states = LoadGrab;
            }
            break;

        case LoadGrab:
            if (GrabTool())
            {
                printf("Going to LoadError. Load new tool. MillChanger.\n");
                states = LoadError;
                break;
            }

            printf("Going to LoadRetractPistons. Load new tool. MillChanger.\n");
            states = LoadRetractPistons;
            break;

        case LoadRetractPistons:
            // Activate piston actuators to approach tool in z axis.
            ClearBit(PISTONS_ENABLE_OUTPUT);
            ClearBit(PISTONL_ENABLE_OUTPUT);
            printf("Going to LoadSafeZEnd. Load new tool. MillChanger.\n");
            states = LoadSafeZEnd;
            break;

        case LoadSafeZEnd:
            // - Rapid to Z Home
            if (MoveZ(0.0, FAST_SPEED))
            {
                printf("Going to LoadError. Load new tool. MillChanger.\n");
                states = LoadError;
                break;
            }

            printf("Going to LoadEnd. Load new tool. MillChanger.\n");
            states = LoadEnd;
            break;

        case LoadError:
            ClearStopImmediately(); // Clear Stop Condition without resuming
            ClearBit(PISTONS_ENABLE_OUTPUT);
            ClearBit(PISTONL_ENABLE_OUTPUT);
            printf("Going to LoadErrorEnd. Load new tool. MillChanger.\n");
            states = LoadErrorEnd;
            break;

        default:
            break;
        }

        WaitNextTimeSlice();
    } while (states != LoadEnd && states != LoadErrorEnd);

    if (states == LoadEnd)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

// Return x position of tool holder as a function of the tool in mm.
float ToolPositionX(int tool)
{
    float xOffset = 0.0;

    if (tool % 2 == 0)
    {
        xOffset = 120.0;
    }

    float xPosition = -13.5 + OFFSET_X + xOffset;

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

    float yPosition = -868.3 + OFFSET_Y + yOffset;
    return yPosition;
}

// Return z position of tool holder in mm.
float ToolPositionZ()
{
    float zPosition = -10 + OFFSET_Z;
    return zPosition;
}

// Move Axis XY at specified Speed and wait until complete
// return 0 = success, 1 if axis disabled
int MoveXY(float x, float y, float Speed)
{
	MoveAtVel(X_AXIS, x / FACTOR_X, Speed / FACTOR_X);
	MoveAtVel(Y_AXIS, y / FACTOR_Y, Speed / FACTOR_Y);
	
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
    Delay_sec(2.0);
}

// Disable piston actuators.
void DisablePistons()
{
    ClearBit(PISTONS_ENABLE_OUTPUT);
    ClearBit(PISTONL_ENABLE_OUTPUT);
    Delay_sec(2.0);
}

// Eject milling tool.
// return 0 = success, 1 if sensor points that the tool is still grabbed.
int EjectTool()
{
    SetBit(SPHERE_GRIPPER_OUTPUT);
    Delay_sec(0.5);
    SetBit(OPEN_TOOL_GRIPPER_OUTPUT);
    Delay_sec(0.5);
    SetBit(EXTRACT_OUTPUT);
    Delay_sec(0.5);
    ClearBit(EXTRACT_OUTPUT);
    SetBit(PISTON_TOOL_OUTPUT);
    while (!ReadBit(PISTON_TOOL_ON_INPUT));
    ClearBit(SPHERE_GRIPPER_OUTPUT);

    // - Wait for time in seconds defined by CLAMP_TIME
	Delay_sec(0.5);

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

    SaveCurrentTool(-1);
  
    ClearBit(PISTON_TOOL_OUTPUT);
    while (!ReadBit(PISTON_TOOL_OFF_INPUT));
    ClearBit(OPEN_TOOL_GRIPPER_OUTPUT);
    Delay_sec(0.5);

    return 0; // success
}

// Grab milling tool.
// return 0 = success, 1 if sensor points that the tool is not grabbed.
int GrabTool()
{
    SetBit(OPEN_TOOL_GRIPPER_OUTPUT);
    SetBit(PISTON_TOOL_OUTPUT);
    while (!ReadBit(PISTON_TOOL_ON_INPUT));
    SetBit(SPHERE_GRIPPER_OUTPUT);
    Delay_sec(0.5);
    ClearBit(PISTON_TOOL_OUTPUT);
    while (!ReadBit(PISTON_TOOL_OFF_INPUT));
    ClearBit(OPEN_TOOL_GRIPPER_OUTPUT);
    Delay_sec(0.5);
    ClearBit(SPHERE_GRIPPER_OUTPUT);

    // - Wait for time in seconds defined by CLAMP_TIME
	Delay_sec(0.5);

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