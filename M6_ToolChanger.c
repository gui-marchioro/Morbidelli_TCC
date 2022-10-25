#include "Drill.c"
#include "Spindle.c"
#include "KMotionDef.h"

#ifndef TMP
#define TMP 10 // which spare persist to use to transfer data
#include "KflopToKMotionCNCFunctions.c"
#endif

#define TOOL_VAR 9

#define PREVIOUS_TOOL_VAR 191

#define MILL_TOOL_SLOT_MIN 1
#define MILL_TOOL_SLOT_MAX 10
#define TOOL_PRESENT_INPUT 1049
#define CLOSE_MAGAZINE_OUTPUT 1061
#define MAGAZINE_CLOSED_INPUT 1040
#define OPEN_MAGAZINE_OUTPUT 1062
#define MAGAZINE_OPENED_INPUT 1041
#define PISTON_TOOL_OUTPUT 1068
#define SPHERE_GRIPPER_OUTPUT 1065
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

// Routine to be executed when M6 TX is called in a G program. (X is the tool number)
void main ()
{
    int slot = persist.UserData[TOOL_VAR];  // value stored is an int 
	int id = persist.UserData[TOOL_VAR+1];  // value stored is an int 
	printf("Tool Set to slot %d id %d\n",slot,id);  // print the slot and id

    if (GetSpindleStatus != SPINDLE_OFF)
    {
        MDI("M5");
    }

    ClearDrillOutputs();

    if(slot >= MILL_TOOL_SLOT_MIN && slot <= MILL_TOOL_SLOT_MAX)
    {
        MillExchangeRoutine(slot);
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

// Exchange the milling tool
void MillExchangeRoutine(int millSlot)
{
    double xOffset = 0;
    double yOffset = 0;

    if (millSlot%2 == 0)
    {
        xOffset = 120;
    }

    if (millSlot > 6)
    {
        yOffset = 160*3;
    }
    else if (millSlot > 4)
    {
        yOffset = 160*2;
    }
    else if (millSlot > 2)
    {
        yOffset = 160*1;
    }

    double xPosition = (double) (23.6 + OFFSET_X + xOffset)/FACTOR_X;
    double yPosition = (double) (-876.2 + OFFSET_Y + yOffset)/FACTOR_X;
    double zPosition = (double) (3.15 + OFFSET_Z)/FACTOR_Z;
    SetBit(OPEN_MAGAZINE_OUTPUT);
    Move(X_AXIS, xPosition);
    Move(Y_AXIS, yPosition);
    while (!(CheckDone(X_AXIS) && CheckDone(Y_AXIS)));
    Move(Z_AXIS, zPosition);
    while (!CheckDone(Z_AXIS));
    SetBit(PISTONS_ENABLE_OUTPUT);
    SetBit(PISTONL_ENABLE_OUTPUT);
    SetBit(PISTON_TOOL_OUTPUT);
}

void SetPreviousTool(int millSlot)
{
    persist.UserData[PREVIOUS_TOOL_VAR] = millSlot;
}

int GetPreviousTool()
{
    return persist.UserData[PREVIOUS_TOOL_VAR];
}