#include "MPGWatch.h"
#include "KMotionDef.h"
//#include "Spindle.c"

int isXSelected = 0;
int isYSelected = 0;
int isZSelected = 0;
int isX1Selected = 0;
int isX10Selected = 0;
int isX100Selected = 0;
int isManualSelected = 0;

void ReadMPGInputs()
{
    if (ReadBit(X_SELECTED_PIN))
    {
        isXSelected = TRUE;
        isYSelected = FALSE;
        isZSelected = FALSE;
    }
    else if (ReadBit(Y_SELECTED_PIN))
    {
        isXSelected = FALSE;
        isYSelected = TRUE;
        isZSelected = FALSE;
    }
    else if (ReadBit(Z_SELECTED_PIN))
    {
        isXSelected = FALSE;
        isYSelected = FALSE;
        isZSelected = TRUE;
    }
    else
    {
        isXSelected = FALSE;
        isYSelected = FALSE;
        isZSelected = FALSE;
    }

    if (ReadBit(X1_MULTIPLIER_PIN))
    {
        isX1Selected = TRUE;
        isX10Selected = FALSE;
        isX100Selected = FALSE;
    }
    else if (ReadBit(X10_MULTIPLIER_PIN))
    {
        isX1Selected = FALSE;
        isX10Selected = TRUE;
        isX100Selected = FALSE;
    }
    else if (ReadBit(X100_MULTIPLIER_PIN))
    {
        isX1Selected = FALSE;
        isX10Selected = FALSE;
        isX100Selected = TRUE;
    }
    else
    {
        isX1Selected = FALSE;
        isX10Selected = FALSE;
        isX100Selected = FALSE;
    }
}

// Evaluates if manual mode is selected based on the inputs
void EvaluateIsManualMPG()
{
    if ((isXSelected == 1) || (isYSelected == 1) || (isZSelected == 1) ||
        (isX1Selected == 1) || (isX10Selected == 1) || (isX100Selected == 1))
    {
        isManualSelected = 1;
    }
    else
    {
        isManualSelected = 0;
    }
}

// Monitoring for buttons of the MPG
// The emergency button is wired externally (off the controller)
// This method has to be called after initializing the infinite loop
void BasicServiceMPG(void)
{
	static int Position, FirstTime=TRUE;
	static int InMotion=FALSE,Axis,LastAxis=-1;
	static double LastChangeTime=0,Target,Factor=0,FactorZ=0,FeedRateInc=0,CurrentFeedRate=1,NextFeedRate=0;
	int DiffPosition, NewPos;
    
    // Only monitors the next methods if no Homing is running
    if (GetIsExecutingHoming() == 1)
    {
        return;
    }
	
    // Initializes the position of the MPG encoder in the first execution.
	if (FirstTime)
	{
		Position = chan[MPG_INPUT_AXIS].Position;
		FirstTime=FALSE;
	}
    // Evaluates the difference between the last and the current position
	NewPos = chan[MPG_INPUT_AXIS].Position;
	DiffPosition = NewPos - Position;
	Position = NewPos;

    ReadMPGInputs();

    EvaluateIsManualMPG();

    // Halt when doing G code and Manual is pressed
    if (isManualSelected && JOB_ACTIVE)
    {
        DoPC(PC_COMM_HALT);
        Delay_sec(0.6);
        StopSpindle();
        Delay_sec(0.6);
        DoPC(PC_COMM_RESTART);
    }

	if ((isManualSelected == 0) || JOB_ACTIVE) // if not button pressed or Job Active (can be used to determine if GCode is running.)  ignore the encoder. 
	{
        DiffPosition = 0;
    }

    // Evaluates the multiplier factor based in the mpg X1xx selector
    if (isX1Selected == 1)  // is X1 selected?
    {
        Factor = 1.25; // Factors to acquire 0.05 mm per increment
        FactorZ = 3.3333334;
    }
    else if (isX10Selected == 1)  // is X10 selected?
    {
        Factor = 2.5; // Factors to acquire 0.1 mm per increment
        FactorZ = 6.6666667;
    }
    else if (isX100Selected == 1)  // is X100 selected?
    {
        Factor = 5; // Factors to acquire 0.2 mm per increment
        FactorZ = 13.3333332;
    }
    else
    {                 
        Factor = 0.0;
        FactorZ = 0.0;
    }

    // Evaluates which axis is selected
    if (isXSelected == 1)  // is x selected?
        Axis=0;
    else if (isYSelected == 1)  // is y selected?
        Axis=1;
    else if (isZSelected == 1)  // is z selected?
        Axis=2;

    // check if the Axis just changed or we have been 
    // converging to the target for a long time
    if (Axis != LastAxis || 
        (InMotion && Time_sec() > LastChangeTime+FINAL_TIME))
    {
        if (InMotion)
            Move(LastAxis,Target);  //finalize any motion

        LastAxis = Axis;
        InMotion = FALSE;
    }
    
    // If Manual is selected, there is encoder movement, an axis is selected, no G code running
    // Accepts jog commands
    if (DiffPosition && (isXSelected || isYSelected || isZSelected)) // did we move?
    {
        if (!InMotion) Target = chan[Axis].Dest;
        if (isZSelected)
        {
            Target += DiffPosition * FactorZ;
        }
        else
        {
            Target += DiffPosition * Factor;
        }
        MoveExp(Axis,Target,TAU);  // note: contains a WaitNextTimeSlice
        LastChangeTime = Time_sec();
        InMotion=TRUE;
    }
}