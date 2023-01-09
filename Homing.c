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

// Posible states for homing axis
enum homeAxisStates
{
    Initial,
    JogUntilLimit,
    ReverseJog,
    ZeroAxis,
    SafeMove,
    End
};

void HomeHorizontalAxis(int Vel, double Position)
{
    enum homeAxisStates XState = Initial;
    enum homeAxisStates YState = Initial;

    while (XState != End || YState != End)
    {
        switch (XState)
        {
        case Initial:
            Jog(X_AXIS, -Vel / FACTOR_X);
            XState = JogUntilLimit;
            break;

        case JogUntilLimit:
            if (ReadBit(X_HOME_INPUT_PIN))
            {
                Jog(X_AXIS, (Vel/10.0) / FACTOR_X);
                XState = ReverseJog;
            }

            break;
        
        case ReverseJog:
            if (!ReadBit(X_HOME_INPUT_PIN))
            {
                Jog(X_AXIS, 0);
                XState = ZeroAxis;
            }

            break;

        case ZeroAxis:
            if (CheckDone(X_AXIS)) // loop until motion completes
            {
                DisableAxis(X_AXIS);			// disable the axis
                Zero(X_AXIS);				// Zero the position
                EnableAxis(X_AXIS);
                MoveAtVel(X_AXIS, Position / FACTOR_X, Vel / FACTOR_X);
                XState = SafeMove;
            }
                
            break;

        case SafeMove:
            if (CheckDone(X_AXIS))
            {
                XState = End;
            }

            break;

        default:
            break;
        }

        switch (YState)
        {
        case Initial:
            Jog(Y_AXIS, Vel / FACTOR_Y);
            YState = JogUntilLimit;
            break;

        case JogUntilLimit:
            if (ReadBit(Y_HOME_INPUT_PIN))
            {
                Jog(Y_AXIS, -(Vel/10.0) / FACTOR_Y);
                YState = ReverseJog;
            }

            break;
        
        case ReverseJog:
            if (!ReadBit(Y_HOME_INPUT_PIN))
            {
                Jog(Y_AXIS, 0);
                YState = ZeroAxis;
            }

            break;

        case ZeroAxis:
            if (CheckDone(Y_AXIS)) // loop until motion completes
            {
                DisableAxis(Y_AXIS);			// disable the axis
                Zero(Y_AXIS);				// Zero the position
                EnableAxis(Y_AXIS);
                MoveAtVel(Y_AXIS, -Position / FACTOR_Y, Vel / FACTOR_Y);
                YState = SafeMove;
            }
                
            break;

        case SafeMove:
            if (CheckDone(Y_AXIS))
            {
                YState = End;
            }
            
            break;

        default:
            break;
        }

        WaitNextTimeSlice();
    }
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
    HomeHorizontalAxis(50.0, 20.0);
    ch0->LimitSwitchOptions = SaveXLimits;  // restore limit settings
    ch1->LimitSwitchOptions = SaveYLimits;  // restore limit settings

    // After homing axes adjust soft limits
    ch0->SoftLimitPos=3.2e+03 / FACTOR_X;
	ch0->SoftLimitNeg=0.0 / FACTOR_X;

    ch1->SoftLimitPos=0.0 / FACTOR_Y;
	ch1->SoftLimitNeg=-1.08e+03 / FACTOR_Y;

    ch2->SoftLimitPos=2.0 / FACTOR_Z;
	ch2->SoftLimitNeg=-1.08e+02 / FACTOR_Z;
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