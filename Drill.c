#include "Drill.h"
#include "TableSelectionWatch.h"
#include "KMotionDef.h"

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

void ChangeOffsetX(int drillSlot)
{
    if (persist.UserData[TABLE_EXECUTING_VAR] == 1)
    {
        switch (drillSlot)
        {
            case DRILL_1_SLOT:
                    SetToolOffsetX(drillSlot, -DRILL_1_OFFSET_X);
                    printf("Changed OffsetX from drill: %d to %f", drillSlot, -DRILL_1_OFFSET_X);
                break;

            case DRILL_2_SLOT:
                    SetToolOffsetX(drillSlot, -DRILL_2_OFFSET_X);
                    printf("Changed OffsetX from drill: %d to %f", drillSlot, -DRILL_2_OFFSET_X);
                break;
            
            case DRILL_3_SLOT:
                    SetToolOffsetX(drillSlot, -DRILL_3_OFFSET_X);
                    printf("Changed OffsetX from drill: %d to %f", drillSlot, -DRILL_3_OFFSET_X);
                break;

            case DRILL_4_SLOT:
                    SetToolOffsetX(drillSlot, -DRILL_4_OFFSET_X);
                    printf("Changed OffsetX from drill: %d to %f", drillSlot, -DRILL_4_OFFSET_X);
                break;

            case DRILL_5_SLOT:
                    SetToolOffsetX(drillSlot, -DRILL_5_OFFSET_X);
                    printf("Changed OffsetX from drill: %d to %f", drillSlot, -DRILL_5_OFFSET_X);
                break;

            case DRILL_6_SLOT:
                    SetToolOffsetX(drillSlot, -DRILL_6_OFFSET_X);
                    printf("Changed OffsetX from drill: %d to %f", drillSlot, -DRILL_6_OFFSET_X);
                break;

            case DRILL_7_SLOT:
                    SetToolOffsetX(drillSlot, -DRILL_7_OFFSET_X);
                    printf("Changed OffsetX from drill: %d to %f", drillSlot, -DRILL_7_OFFSET_X);
                break;

            case DRILL_8_SLOT:
                    SetToolOffsetX(drillSlot, -DRILL_8_OFFSET_X);
                    printf("Changed OffsetX from drill: %d to %f", drillSlot, -DRILL_8_OFFSET_X);
                break;

            case DRILL_9_SLOT:
                    SetToolOffsetX(drillSlot, -DRILL_9_OFFSET_X);
                    printf("Changed OffsetX from drill: %d to %f", drillSlot, -DRILL_9_OFFSET_X);
                break;

            case DRILL_10_SLOT:
                    SetToolOffsetX(drillSlot, -DRILL_10_OFFSET_X);
                    printf("Changed OffsetX from drill: %d to %f", drillSlot, -DRILL_10_OFFSET_X);
                break;

            /*case DRILL_11_SLOT:
                    SetToolOffsetX(drillSlot, -DRILL_11_OFFSET_X);
                    printf("Changed OffsetX from drill: %d to %f", drillSlot, -DRILL_11_OFFSET_X);
                break;

            case DRILL_12_SLOT:
                    SetToolOffsetX(drillSlot, -DRILL_12_OFFSET_X);
                    printf("Changed OffsetX from drill: %d to %f", drillSlot, -DRILL_12_OFFSET_X);
                break;
            
            case DRILL_13_SLOT:
                    SetToolOffsetX(drillSlot, -DRILL_13_OFFSET_X);
                    printf("Changed OffsetX from drill: %d to %f", drillSlot, -DRILL_13_OFFSET_X);
                break;

            case DRILL_14_SLOT:
                    SetToolOffsetX(drillSlot, -DRILL_14_OFFSET_X);
                    printf("Changed OffsetX from drill: %d to %f", drillSlot, -DRILL_14_OFFSET_X);
                break;

            case DRILL_15_SLOT:
                    SetToolOffsetX(drillSlot, -DRILL_15_OFFSET_X);
                    printf("Changed OffsetX from drill: %d to %f", drillSlot, -DRILL_15_OFFSET_X);
                break;

            case DRILL_16_SLOT:
                    SetToolOffsetX(drillSlot, -DRILL_16_OFFSET_X);
                    printf("Changed OffsetX from drill: %d to %f", drillSlot, -DRILL_16_OFFSET_X);
                break;

            case DRILL_17_SLOT:
                    SetToolOffsetX(drillSlot, -DRILL_17_OFFSET_X);
                    printf("Changed OffsetX from drill: %d to %f", drillSlot, -DRILL_17_OFFSET_X);
                break;

            case DRILL_18_SLOT:
                    SetToolOffsetX(drillSlot, -DRILL_18_OFFSET_X);
                    printf("Changed OffsetX from drill: %d to %f", drillSlot, -DRILL_18_OFFSET_X);
                break;

            case DRILL_19_SLOT:
                    SetToolOffsetX(drillSlot, -DRILL_19_OFFSET_X);
                    printf("Changed OffsetX from drill: %d to %f", drillSlot, -DRILL_19_OFFSET_X);
                break;

            case DRILL_20_SLOT:
                    SetToolOffsetX(drillSlot, -DRILL_20_OFFSET_X);
                    printf("Changed OffsetX from drill: %d to %f", drillSlot, -DRILL_20_OFFSET_X);
                break;

            case DRILL_21_SLOT:
                    SetToolOffsetX(drillSlot, -DRILL_21_OFFSET_X);
                    printf("Changed OffsetX from drill: %d to %f", drillSlot, -DRILL_21_OFFSET_X);
                break;*/

            default:
                break;
        }

        persist.UserData[PREVIOUS_DRILL_VAR] = drillSlot;
    }
}

void RestoreOffsetX()
{
    int previousDrill = persist.UserData[PREVIOUS_DRILL_VAR];
    if (persist.UserData[TABLE_EXECUTING_VAR] == 1)
    {
        switch (previousDrill)
        {
            case DRILL_1_SLOT:
                    SetToolOffsetX(previousDrill, DRILL_1_OFFSET_X);
                    printf("Restored OffsetX from drill: %d to %f", previousDrill, DRILL_1_OFFSET_X);
                break;

            case DRILL_2_SLOT:
                    SetToolOffsetX(previousDrill, DRILL_2_OFFSET_X);
                    printf("Restored OffsetX from drill: %d to %f", previousDrill, DRILL_2_OFFSET_X);
                break;
            
            case DRILL_3_SLOT:
                    SetToolOffsetX(previousDrill, DRILL_3_OFFSET_X);
                    printf("Restored OffsetX from drill: %d to %f", previousDrill, DRILL_3_OFFSET_X);
                break;

            case DRILL_4_SLOT:
                    SetToolOffsetX(previousDrill, DRILL_4_OFFSET_X);
                    printf("Restored OffsetX from drill: %d to %f", previousDrill, DRILL_4_OFFSET_X);
                break;

            case DRILL_5_SLOT:
                    SetToolOffsetX(previousDrill, DRILL_5_OFFSET_X);
                    printf("Restored OffsetX from drill: %d to %f", previousDrill, DRILL_5_OFFSET_X);
                break;

            case DRILL_6_SLOT:
                    SetToolOffsetX(previousDrill, DRILL_6_OFFSET_X);
                    printf("Restored OffsetX from drill: %d to %f", previousDrill, DRILL_6_OFFSET_X);
                break;

            case DRILL_7_SLOT:
                    SetToolOffsetX(previousDrill, DRILL_7_OFFSET_X);
                    printf("Restored OffsetX from drill: %d to %f", previousDrill, DRILL_7_OFFSET_X);
                break;

            case DRILL_8_SLOT:
                    SetToolOffsetX(previousDrill, DRILL_8_OFFSET_X);
                    printf("Restored OffsetX from drill: %d to %f", previousDrill, DRILL_8_OFFSET_X);
                break;

            case DRILL_9_SLOT:
                    SetToolOffsetX(previousDrill, DRILL_9_OFFSET_X);
                    printf("Restored OffsetX from drill: %d to %f", previousDrill, DRILL_9_OFFSET_X);
                break;

            case DRILL_10_SLOT:
                    SetToolOffsetX(previousDrill, DRILL_10_OFFSET_X);
                    printf("Restored OffsetX from drill: %d to %f", previousDrill, DRILL_10_OFFSET_X);
                break;

            /*case DRILL_11_SLOT:
                    SetToolOffsetX(previousDrill, DRILL_11_OFFSET_X);
                    printf("Restored OffsetX from drill: %d to %f", previousDrill, DRILL_11_OFFSET_X);
                break;

            case DRILL_12_SLOT:
                    SetToolOffsetX(previousDrill, DRILL_12_OFFSET_X);
                    printf("Restored OffsetX from drill: %d to %f", previousDrill, DRILL_12_OFFSET_X);
                break;
            
            case DRILL_13_SLOT:
                    SetToolOffsetX(previousDrill, DRILL_13_OFFSET_X);
                    printf("Restored OffsetX from drill: %d to %f", previousDrill, DRILL_13_OFFSET_X);
                break;

            case DRILL_14_SLOT:
                    SetToolOffsetX(previousDrill, DRILL_14_OFFSET_X);
                    printf("Restored OffsetX from drill: %d to %f", previousDrill, DRILL_14_OFFSET_X);
                break;

            case DRILL_15_SLOT:
                    SetToolOffsetX(previousDrill, DRILL_15_OFFSET_X);
                    printf("Restored OffsetX from drill: %d to %f", previousDrill, DRILL_15_OFFSET_X);
                break;

            case DRILL_16_SLOT:
                    SetToolOffsetX(previousDrill, DRILL_16_OFFSET_X);
                    printf("Restored OffsetX from drill: %d to %f", previousDrill, DRILL_16_OFFSET_X);
                break;

            case DRILL_17_SLOT:
                    SetToolOffsetX(previousDrill, DRILL_17_OFFSET_X);
                    printf("Restored OffsetX from drill: %d to %f", previousDrill, DRILL_17_OFFSET_X);
                break;

            case DRILL_18_SLOT:
                    SetToolOffsetX(previousDrill, DRILL_18_OFFSET_X);
                    printf("Restored OffsetX from drill: %d to %f", previousDrill, DRILL_18_OFFSET_X);
                break;

            case DRILL_19_SLOT:
                    SetToolOffsetX(previousDrill, DRILL_19_OFFSET_X);
                    printf("Restored OffsetX from drill: %d to %f", previousDrill, DRILL_19_OFFSET_X);
                break;

            case DRILL_20_SLOT:
                    SetToolOffsetX(previousDrill, DRILL_20_OFFSET_X);
                    printf("Restored OffsetX from drill: %d to %f", previousDrill, DRILL_20_OFFSET_X);
                break;

            case DRILL_21_SLOT:
                    SetToolOffsetX(previousDrill, DRILL_21_OFFSET_X);
                    printf("Restored OffsetX from drill: %d to %f", previousDrill, DRILL_21_OFFSET_X);
                break;*/

            default:
                break;
        }

        persist.UserData[PREVIOUS_DRILL_VAR] = 0;
    }
}

// Select and detach the selected drill
void DrillRoutine(int drillSlot)
{
    ChangeOffsetX(drillSlot);
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