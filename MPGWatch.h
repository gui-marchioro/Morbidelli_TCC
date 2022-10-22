#ifndef MPGWatch_h
#define MPGWatch_h

#define X_SELECTED_PIN 1026
#define Y_SELECTED_PIN 1027
#define Z_SELECTED_PIN 1028
#define X1_MULTIPLIER_PIN 1029
#define X10_MULTIPLIER_PIN 1030
#define X100_MULTIPLIER_PIN 1031

#define MPG_INPUT_AXIS 7

#define TAU 0.08 // smoothness factor (Low Pass Time constant seconds)
#define FINAL_TIME 1.0 // Set final dest after this amount of time with no change

// Global variables to hold the state of the MPG inputs
extern int isXSelected;
extern int isYSelected;
extern int isZSelected;
extern int isX1Selected;
extern int isX10Selected;
extern int isX100Selected;
extern int isManualSelected;

// Read the inputs related to the MPG controller
void ReadMPGInputs();

// Evaluates if manual mode is selected based on the inputs
void EvaluateIsManualMPG();

// Monitoring for buttons of the MPG
// The emergency button is wired externally (off the controller)
// This method has to be called after initializing the infinite loop
void BasicServiceMPG(void);

#endif /* MPGWatch_h */