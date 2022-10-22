#ifndef EmergencyWatch_h
#define EmergencyWatch_h

#define THERMAL_PROTECTION_PIN 1089
#define GENERAL_EMERGENCY_PIN 1090 // Axis limit, indicates emergency when low
#define HAS_PRESSURE_PIN 1091
#define X_AXIS_NEGATIVE_LIMIT_PIN 139
#define X_AXIS_POSITIVE_LIMIT_PIN 140
#define Y_AXIS_NEGATIVE_LIMIT_PIN 141
#define Y_AXIS_POSITIVE_LIMIT_PIN 142
#define Z_AXIS_NEGATIVE_LIMIT_PIN 143
#define Z_AXIS_POSITIVE_LIMIT_PIN 1088

#define EMERGENCY_STATE_VAR 183

// Monitor emergency safety variables
// If one or more of them are true, signalizes and set the emergency state
// If none of them are true, resets the emergency state
void InitialEmergencyMonitoring();

// Monitor emergency safety variables
// If one or more of them are true, signalizes and set the emergency state
// If none of them are true, resets the emergency state
void LoopEmergencyMonitoring();

// Receives a number of input to verify and a message to be shown if the input is true.
int WatchInputLowLogic(int input, char *message);

// Receives a number of input to verify and a message to be shown if the input is true.
int WatchInputHighLogic(int input, char *message);

// Sign that the emergency is raised to serve as a condition for other programs.
// Ex. not execute Init before clear emergency
void SetEmergencyState();

// Reset the emergency raised flag.
void ResetEmergencyState();

// Get a value that indicates wheter the emergency state is active or not.
// Returns 1 if it was executed
int GetEmergencyState();

#endif /* EmergencyWatch_h */