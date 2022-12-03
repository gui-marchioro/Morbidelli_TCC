#include "Drill.h"
#include "KMotionDef.h"

void SetDrillBoxStatus()
{
    persist.UserData[DRILL_STATUS_VAR] = 1;
}

void ResetDrillBoxStatus()
{
    persist.UserData[DRILL_STATUS_VAR] = 0;
}

int GetDrillBoxStatus()
{
    return persist.UserData[DRILL_STATUS_VAR];
}

// Clear all outputs related to the drill operation
void ClearDrillOutputs()
{
    ResetDrillBoxStatus();
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
    SetDrillBoxStatus();
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