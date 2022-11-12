#include "Homing.h"
#include "MovementDefs.h"
#include "KMotionDef.h"

// Executes homing routine
void Homing()
{
    SetIsExecutingHoming();
    HomeAllAxis();
    SetHomingExecuted();
    ResetIsExecutingHoming();
}

// Home one desired axis. It reaches the sensor, reverse the movement applying less velocity,
// and zero when the sensor ends the reading
// Receives the velocity in mm/s and the position in mm.
void HomeAxis(int Axis, int HomePin, int Vel, double Position, double mmFactor)
{
	// Home Axis - jog until it sees the limit

    Jog(Axis, Vel / mmFactor);  			// jog slowly negative
    while (!ReadBit(HomePin)) ;  	// loop until IO bit goes high
    Jog(Axis, -(Vel/10.0) / mmFactor);
    while (ReadBit(HomePin)) ;  	// loop until IO bit goes low
    Jog(Axis,0);
    while (!CheckDone(Axis)) ; // loop until motion completes
	DisableAxis(Axis);			// disable the axis
    Zero(Axis);				// Zero the position
	EnableAxis(Axis);			// re-enable the ServoTick
	Move(Axis, Position / mmFactor);			// move some amount inside the limits
    while (!CheckDone(Axis)) ; // loop until motion completes
}

// Home all axis
void HomeAllAxis()
{
    int SaveXLimits,SaveYLimits,SaveZLimits;  //place to save limit switch settings

    // disable the limits (first save how they were set) 
    SaveXLimits = ch0->LimitSwitchOptions;
    SaveYLimits = ch1->LimitSwitchOptions;
    SaveZLimits = ch2->LimitSwitchOptions;
    ch0->LimitSwitchOptions = 0;
    ch1->LimitSwitchOptions = 0;
    ch2->LimitSwitchOptions = 0;

    HomeAxis(Z_AXIS, Z_HOME_INPUT_PIN, 5.0, -7.5, FACTOR_Z);
    ch2->LimitSwitchOptions = SaveZLimits;  // restore limit settings
    HomeAxis(X_AXIS, X_HOME_INPUT_PIN, -50.0, 20.0, FACTOR_X);
    ch0->LimitSwitchOptions = SaveXLimits;  // restore limit settings
    HomeAxis(Y_AXIS, Y_HOME_INPUT_PIN, 50.0, -20.0, FACTOR_Y);
    ch1->LimitSwitchOptions = SaveYLimits;  // restore limit settings
}

// Sign that homing was executed to serve as a condition for other programs.
// Ex. not execute a G program before Homing.
void SetHomingExecuted()
{
	persist.UserData[HOMING_EXECUTED_VAR] = 1;
}

// Reset the flag that sign that homing was executed
void ResetHomingExecuted()
{
	persist.UserData[HOMING_EXECUTED_VAR] = 0;
}

// Get a value that indicates wheter the homing program was executed.
// Returns 1 if it was executed
int GetHomingExecuted()
{
	return persist.UserData[HOMING_EXECUTED_VAR];
}

// Sign that homing is executing to serve as a condition for other programs.
void SetIsExecutingHoming()
{
    persist.UserData[HOMING_EXECUTING_VAR] = 1;
}

// Reset the flag that sign that homing is executing
void ResetIsExecutingHoming()
{
    persist.UserData[HOMING_EXECUTING_VAR] = 0;
}

// Get a value that indicates wheter the homing program is executing.
// Returns 1 if it is executing
int GetIsExecutingHoming()
{
	return persist.UserData[HOMING_EXECUTING_VAR];
}