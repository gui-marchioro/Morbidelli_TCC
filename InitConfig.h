#ifndef InitConfig_h    /* This is an "include guard" */
#define InitConfig_h    /* prevents the file from being included twice. */
                        /* Including a header file twice causes all kinds */
                        /* of interesting problems.*/

#define ENABLE_ALL_AXIS_PIN 1120 // Brake enabled
#define INIT_EXECUTED_VAR 180

// Initial configuration for Axis and IOBoards
void InitConfig();

// Configure axes and its controller according to the setup made in the StepScreen from KMotion software
// In this particular configuration it was inverted the X axis for the desired system coordinates of the machine
// In front of the machine X positive axis represents the tool going to the right direction
//                         Y positive axis represents the tool going backward
//                         Z positive axis represents the tool going upwards
void AxisConfig();

// Configure the Konnect IO boards connected with the KFLOP main board
void IOboardsConfig();

// Sign that the initialization was executed to serve as a condition for other programs.
// Ex. not execute Homing before Init
void SetInitExecuted();

// Sign that the initialization was not executed.
// Ex. not execute Homing before Init
void ResetInitExecuted();

// Get a value that indicates wheter the initialization program was executed.
// Returns 1 if it was executed
int GetInitExecuted();

#endif /* InitConfig_h */