#ifndef MillChanger_h
#define MillChanger_h

#define MILL_TOOL_SLOT_MIN 1
#define MILL_TOOL_SLOT_MAX 8
#define TOOL_PRESENT_INPUT 1049
#define CLOSE_MAGAZINE_OUTPUT 1125
#define MAGAZINE_CLOSED_INPUT 1040
#define OPEN_MAGAZINE_OUTPUT 1126
#define MAGAZINE_OPENED_INPUT 1041
#define PISTON_TOOL_OUTPUT 1132
#define SPHERE_GRIPPER_OUTPUT 1129
#define PANTOGRAPH_OFF_SENSOR_INPUT 1042 //Pantografo recuado/alto
#define PANTOGRAPH_MOVING_SENSOR_INPUT 1043 //Pantografo baixo
#define PANTOGRAPH_ON_SENSOR_INPUT 1044 //Pantografo baixo

#define OFFSET_X 20
#define OFFSET_Y -20
#define OFFSET_Z -7.5

#define SLOW_SPEED 60 // mm/sec

// Routine responsible for exchange mill tool.
// Return 0=Success, 1=Failure
int MillExchangeRoutine(int millSlot);

// Opens the tool magazine.
void OpenMagazine();

// Closes the tool magazine.
void CloseMagazine();

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
int LoadNewTool(int newTool);

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

#endif /* MillChanger_h */