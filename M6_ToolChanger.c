#include "KMotionDef.h"

#ifndef TMP
#define TMP 10 // which spare persist to use to transfer data
#include "KflopToKMotionCNCFunctions.c"
#endif

#define TOOL_VAR 9
#define MILL_TOOL_SLOT_MIN 1
#define MILL_TOOL_SLOT_MAX 10
#define DRILL_BOX_SLOT_MIN 11
#define DRILL_BOX_SLOT_MAX 31

#define DRILL_1_SLOT 11
#define DRILL_2_SLOT 12
#define DRILL_3_SLOT 13
#define DRILL_4_SLOT 14
#define DRILL_5_SLOT 15
#define DRILL_6_SLOT 16
#define DRILL_7_SLOT 17
#define DRILL_8_SLOT 18
#define DRILL_9_SLOT 19
#define DRILL_10_SLOT 20
#define DRILL_11_SLOT 21
#define DRILL_12_SLOT 22
#define DRILL_13_SLOT 23
#define DRILL_14_SLOT 24
#define DRILL_15_SLOT 25
#define DRILL_16_SLOT 26
#define DRILL_17_SLOT 27
#define DRILL_18_SLOT 28
#define DRILL_19_SLOT 29
#define DRILL_20_SLOT 30
#define DRILL_21_SLOT 31

#define DRILL_MOTOR_OUTPUT 1069
#define DRILL_1_PISTON_OUTPUT 48
#define DRILL_2_PISTON_OUTPUT 49
#define DRILL_3_PISTON_OUTPUT 50
#define DRILL_4_PISTON_OUTPUT 51
#define DRILL_5_PISTON_OUTPUT 52
#define DRILL_6_PISTON_OUTPUT 53
#define DRILL_7_PISTON_OUTPUT 54
#define DRILL_8_PISTON_OUTPUT 55
#define DRILL_9_PISTON_OUTPUT 56
#define DRILL_10_PISTON_OUTPUT 57
/*#define DRILL_11_PISTON_OUTPUT 
#define DRILL_12_PISTON_OUTPUT 
#define DRILL_13_PISTON_OUTPUT 
#define DRILL_14_PISTON_OUTPUT 
#define DRILL_15_PISTON_OUTPUT 
#define DRILL_16_PISTON_OUTPUT 
#define DRILL_17_PISTON_OUTPUT 
#define DRILL_18_PISTON_OUTPUT 
#define DRILL_19_PISTON_OUTPUT 
#define DRILL_20_PISTON_OUTPUT 
#define DRILL_21_PISTON_OUTPUT */

// Routine to be executed when M6 TX is called in a G program. (X is the tool number)
void main ()
{
    int slot = persist.UserData[TOOL_VAR];  // value stored is an int 
	int id = persist.UserData[TOOL_VAR+1];  // value stored is an int 
	printf("Tool Set to slot %d id %d\n",slot,id);  // print the slot and id

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

// Clear all outputs related to the drill operation
void ClearDrillOutputs()
{
    ClearBit(DRILL_MOTOR_OUTPUT);
    ClearBit(DRILL_1_PISTON_OUTPUT);
    ClearBit(DRILL_2_PISTON_OUTPUT);
    ClearBit(DRILL_3_PISTON_OUTPUT);
    ClearBit(DRILL_4_PISTON_OUTPUT);
    ClearBit(DRILL_5_PISTON_OUTPUT);
    ClearBit(DRILL_6_PISTON_OUTPUT);
    ClearBit(DRILL_7_PISTON_OUTPUT);
    ClearBit(DRILL_8_PISTON_OUTPUT);
    ClearBit(DRILL_9_PISTON_OUTPUT);
    ClearBit(DRILL_10_PISTON_OUTPUT);
    /*ClearBit(DRILL_11_PISTON_OUTPUT);
    ClearBit(DRILL_12_PISTON_OUTPUT);
    ClearBit(DRILL_13_PISTON_OUTPUT);
    ClearBit(DRILL_14_PISTON_OUTPUT);
    ClearBit(DRILL_15_PISTON_OUTPUT);
    ClearBit(DRILL_16_PISTON_OUTPUT);
    ClearBit(DRILL_17_PISTON_OUTPUT);
    ClearBit(DRILL_18_PISTON_OUTPUT);
    ClearBit(DRILL_19_PISTON_OUTPUT);
    ClearBit(DRILL_20_PISTON_OUTPUT);
    ClearBit(DRILL_21_PISTON_OUTPUT);*/
}

// Select and detach the selected drill
void DrillRoutine(int drillSlot)
{
    Delay_sec(0.25);
    SetBit(DRILL_MOTOR_OUTPUT);
    Delay_sec(0.25);
    switch (drillSlot)
    {
        case DRILL_1_SLOT:
                SetBit(DRILL_1_PISTON_OUTPUT);
            break;

        case DRILL_2_SLOT:
                SetBit(DRILL_2_PISTON_OUTPUT);
            break;
        
        case DRILL_3_SLOT:
                SetBit(DRILL_3_PISTON_OUTPUT);
            break;

        case DRILL_4_SLOT:
                SetBit(DRILL_4_PISTON_OUTPUT);
            break;

        case DRILL_5_SLOT:
                SetBit(DRILL_5_PISTON_OUTPUT);
            break;

        case DRILL_6_SLOT:
                SetBit(DRILL_6_PISTON_OUTPUT);
            break;

        case DRILL_7_SLOT:
                SetBit(DRILL_7_PISTON_OUTPUT);
            break;

        case DRILL_8_SLOT:
                SetBit(DRILL_8_PISTON_OUTPUT);
            break;

        case DRILL_9_SLOT:
                SetBit(DRILL_9_PISTON_OUTPUT);
            break;

        case DRILL_10_SLOT:
                SetBit(DRILL_10_PISTON_OUTPUT);
            break;

        /*case DRILL_11_SLOT:
                SetBit(DRILL_11_PISTON_OUTPUT);
            break;

        case DRILL_12_SLOT:
                SetBit(DRILL_12_PISTON_OUTPUT);
            break;
        
        case DRILL_13_SLOT:
                SetBit(DRILL_13_PISTON_OUTPUT);
            break;

        case DRILL_14_SLOT:
                SetBit(DRILL_14_PISTON_OUTPUT);
            break;

        case DRILL_15_SLOT:
                SetBit(DRILL_15_PISTON_OUTPUT);
            break;

        case DRILL_16_SLOT:
                SetBit(DRILL_16_PISTON_OUTPUT);
            break;

        case DRILL_17_SLOT:
                SetBit(DRILL_17_PISTON_OUTPUT);
            break;

        case DRILL_18_SLOT:
                SetBit(DRILL_18_PISTON_OUTPUT);
            break;

        case DRILL_19_SLOT:
                SetBit(DRILL_19_PISTON_OUTPUT);
            break;

        case DRILL_20_SLOT:
                SetBit(DRILL_20_PISTON_OUTPUT);
            break;

        case DRILL_21_SLOT:
                SetBit(DRILL_21_PISTON_OUTPUT);
            break;*/

        default:
            break;
    }
}

// Exchange the milling tool
void MillExchangeRoutine(int millSlot)
{

}