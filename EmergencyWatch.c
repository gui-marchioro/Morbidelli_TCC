#include "EmergencyWatch.h"
#include "KMotionDef.h"

#ifndef TMP
#define TMP 10 // which spare persist to use to transfer data
#include "KflopToKMotionCNCFunctions.c"
#endif

// Monitor emergency safety variables
// If one or more of them are true, signalizes and set the emergency state
// If none of them are true, resets the emergency state
void InitialEmergencyMonitoring()
{
    if ((WatchInputHighLogic(THERMAL_PROTECTION_PIN, "Thermal protection warning active") == 1) ||
        (WatchInputLowLogic(GENERAL_EMERGENCY_PIN, "General emergency active") == 1) ||
        (WatchInputLowLogic(HAS_PRESSURE_PIN, "The pressure is low than required") == 1))
    {
        SetEmergencyState();
    }
    else
    {
        WatchInputLowLogic(X_AXIS_NEGATIVE_LIMIT_PIN, "The X axis reach its negative limit");
        WatchInputLowLogic(X_AXIS_POSITIVE_LIMIT_PIN, "The X axis reach its positive limit");
        WatchInputLowLogic(Y_AXIS_NEGATIVE_LIMIT_PIN, "The Y axis reach its negative limit");
        WatchInputLowLogic(Y_AXIS_POSITIVE_LIMIT_PIN, "The Y axis reach its positive limit");
        WatchInputLowLogic(Z_AXIS_NEGATIVE_LIMIT_PIN, "The Z axis reach its negative limit");
        WatchInputLowLogic(Z_AXIS_POSITIVE_LIMIT_PIN, "The Z axis reach its positive limit");
        ResetEmergencyState();
    }
}

// Monitor emergency safety variables
// If one or more of them are true, signalizes and set the emergency state
// If none of them are true, resets the emergency state
void LoopEmergencyMonitoring()
{
    if ((WatchInputHighLogic(THERMAL_PROTECTION_PIN, "Thermal protection warning active") == 1) ||
        (WatchInputLowLogic(GENERAL_EMERGENCY_PIN, "General emergency active") == 1) ||
        (WatchInputLowLogic(HAS_PRESSURE_PIN, "The pressure is low than required") == 1))
    {
        MDI("M5");
        SetEmergencyState();
    }
    else
    {
        ResetEmergencyState();
    }
}

// Receives a number of input to verify and a message to be shown if the input is true.
int WatchInputLowLogic(int input, char *message)
{
    if (!ReadBit(input))
    {
        MsgBoxNoWait(message, MB_ICONEXCLAMATION);
        return 1;
    }
    
    return 0;
}

// Receives a number of input to verify and a message to be shown if the input is true.
int WatchInputHighLogic(int input, char *message)
{
    if (ReadBit(input))
    {
        MsgBoxNoWait(message, MB_ICONEXCLAMATION);
        return 1;
    }
    
    return 0;
}

// Sign that the emergency is raised to serve as a condition for other programs.
// Ex. not execute Init before clear emergency
void SetEmergencyState()
{
	persist.UserData[EMERGENCY_STATE_VAR] = 1;
}

// Reset the emergency raised flag.
void ResetEmergencyState()
{
	persist.UserData[EMERGENCY_STATE_VAR] = 0;
}

// Get a value that indicates wheter the emergency state is active or not.
// Returns 1 if it was executed
int GetEmergencyState()
{
	return persist.UserData[EMERGENCY_STATE_VAR];
}