#ifndef ManualToolExchangeWatch_h
#define ManualToolExchangeWatch_h

// Outputs to be controled for manual tool exchange
#define EXTRACT_OUTPUT 1066
#define OPEN_TOOL_GRIPPER_OUTPUT 1067

// Inputs to be monitored for manual tool exchange
#define EXCHANGE_TOOL_BUTTON_INPUT 1050

// The time that the open tool gripper should wait before turn on 
#define GRIPPER_ON_STATE_TIME 0.5

// Debounce a bit
//
// return 1 one time when first debounced high 
// return 0 one time when first debounced low 
// return -1 otherwise 
#define DBTIME 300

int Debounce(int n, int *cnt, int *last, int *lastsolid);

// Posible states for the tool exchange cycle
extern enum toolExchangeStates;

// Routine that monitors the inputs related to the manual tool exchange and runs the machine state
// that opens the gripper and the extractor
void ManualToolExchangeWatch(void);

#endif /* ManualToolExchangeWatch_h */