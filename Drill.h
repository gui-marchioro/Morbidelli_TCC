#ifndef Drill_h
#define Drill_h

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

// Clear all outputs related to the drill operation
void ClearDrillOutputs();

// Select and detach the selected drill
void DrillRoutine(int drillSlot);

#endif /* Drill_h */