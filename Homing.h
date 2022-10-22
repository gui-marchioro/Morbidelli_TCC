#ifndef Homing_h
#define Homing_h

#define X_HOME_INPUT_PIN 136
#define Y_HOME_INPUT_PIN 137
#define Z_HOME_INPUT_PIN 138
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2
#define HOMING_EXECUTED_VAR 181
#define HOMING_EXECUTING_VAR 182

// Executes homing routine
void Homing();

// Home one desired axis
void HomeAxis(int Axis, int HomePin, int Vel, double Position);

// Home all axis
void HomeAllAxis();

// Sign that homing was executed to serve as a condition for other programs.
// Ex. not execute a G program before Homing.
void SetHomingExecuted();

// Reset the flag that sign that homing was executed
void ResetHomingExecuted();

// Get a value that indicates wheter the homing program was executed.
// Returns 1 if it was executed
int GetHomingExecuted();

// Sign that homing is executing to serve as a condition for other programs.
void SetIsExecutingHoming();

// Reset the flag that sign that homing is executing
void ResetIsExecutingHoming();

// Get a value that indicates wheter the homing program is executing.
// Returns 1 if it is executing
int GetIsExecutingHoming();

#endif /* Homing_h */