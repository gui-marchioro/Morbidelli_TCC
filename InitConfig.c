#include "InitConfig.h"
#include "KMotionDef.h"

// Initial configuration for Axis and IOBoards
void InitConfig()
{
	AxisConfig();
	IOboardsConfig();
	// Sign that the initialization was executed
	SetInitExecuted();
}

// Configure axes and its controller according to the setup made in the StepScreen from KMotion software
// In this particular configuration it was inverted the X axis for the desired system coordinates of the machine
// In front of the machine X positive axis represents the tool going to the right direction
//                         Y positive axis represents the tool going backward
//                         Z positive axis represents the tool going upwards
void AxisConfig()
{
    SetBit(152);  // enable AMPS
	SetBit(157);
	SetBit(158);
	SetBit(ENABLE_ALL_AXIS_PIN);
	
	ch0->InputMode=ENCODER_MODE;
	ch0->OutputMode=DAC_SERVO_MODE;
	ch0->Vel=6000;
	ch0->Accel=40000;
	ch0->Jerk=400000;
	ch0->P=3;
	ch0->I=1e-05;
	ch0->D=15;
	ch0->FFAccel=0.0001;
	ch0->FFVel=0.025;
	ch0->MaxI=200;
	ch0->MaxErr=2000;
	ch0->MaxOutput=2047;
	ch0->DeadBandGain=1;
	ch0->DeadBandRange=0;
	ch0->InputChan0=0;
	ch0->InputChan1=1;
	ch0->OutputChan0=0;
	ch0->OutputChan1=1;
	ch0->MasterAxis=-1;
	ch0->LimitSwitchOptions=0x12f;
	ch0->LimitSwitchNegBit=139;
	ch0->LimitSwitchPosBit=140;
	ch0->SoftLimitPos=1e+09;
	ch0->SoftLimitNeg=-1e+09;
	ch0->InputGain0=-1;
	ch0->InputGain1=1;
	ch0->InputOffset0=0;
	ch0->InputOffset1=0;
	ch0->OutputGain=-1;
	ch0->OutputOffset=0;
	ch0->SlaveGain=1;
	ch0->BacklashMode=BACKLASH_OFF;
	ch0->BacklashAmount=0;
	ch0->BacklashRate=0;
	ch0->invDistPerCycle=1;
	ch0->Lead=0;
	ch0->MaxFollowingError=200;
	ch0->StepperAmplitude=250;

	ch0->iir[0].B0=1;
	ch0->iir[0].B1=0;
	ch0->iir[0].B2=0;
	ch0->iir[0].A1=0;
	ch0->iir[0].A2=0;

	ch0->iir[1].B0=1;
	ch0->iir[1].B1=0;
	ch0->iir[1].B2=0;
	ch0->iir[1].A1=0;
	ch0->iir[1].A2=0;

	ch0->iir[2].B0=1;
	ch0->iir[2].B1=0;
	ch0->iir[2].B2=0;
	ch0->iir[2].A1=0;
	ch0->iir[2].A2=0;

	ch1->InputMode=ENCODER_MODE;
	ch1->OutputMode=DAC_SERVO_MODE;
	ch1->Vel=6000;
	ch1->Accel=40000;
	ch1->Jerk=400000;
	ch1->P=3;
	ch1->I=1e-05;
	ch1->D=15;
	ch1->FFAccel=0.0001;
	ch1->FFVel=0.025;
	ch1->MaxI=200;
	ch1->MaxErr=2000;
	ch1->MaxOutput=2047;
	ch1->DeadBandGain=1;
	ch1->DeadBandRange=0;
	ch1->InputChan0=1;
	ch1->InputChan1=1;
	ch1->OutputChan0=1;
	ch1->OutputChan1=1;
	ch1->MasterAxis=-1;
	ch1->LimitSwitchOptions=0x12f;
	ch1->LimitSwitchNegBit=141;
	ch1->LimitSwitchPosBit=142;
	ch1->SoftLimitPos=1e+09;
	ch1->SoftLimitNeg=-1e+09;
	ch1->InputGain0=1;
	ch1->InputGain1=1;
	ch1->InputOffset0=0;
	ch1->InputOffset1=0;
	ch1->OutputGain=1;
	ch1->OutputOffset=0;
	ch1->SlaveGain=1;
	ch1->BacklashMode=BACKLASH_OFF;
	ch1->BacklashAmount=0;
	ch1->BacklashRate=0;
	ch1->invDistPerCycle=1;
	ch1->Lead=0;
	ch1->MaxFollowingError=200;
	ch1->StepperAmplitude=250;

	ch1->iir[0].B0=1;
	ch1->iir[0].B1=0;
	ch1->iir[0].B2=0;
	ch1->iir[0].A1=0;
	ch1->iir[0].A2=0;

	ch1->iir[1].B0=1;
	ch1->iir[1].B1=0;
	ch1->iir[1].B2=0;
	ch1->iir[1].A1=0;
	ch1->iir[1].A2=0;

	ch1->iir[2].B0=1;
	ch1->iir[2].B1=0;
	ch1->iir[2].B2=0;
	ch1->iir[2].A1=0;
	ch1->iir[2].A2=0;

	ch2->InputMode=ENCODER_MODE;
	ch2->OutputMode=DAC_SERVO_MODE;
	ch2->Vel=6000;
	ch2->Accel=40000;
	ch2->Jerk=400000;
	ch2->P=3;
	ch2->I=1e-05;
	ch2->D=15;
	ch2->FFAccel=0.0001;
	ch2->FFVel=0.025;
	ch2->MaxI=200;
	ch2->MaxErr=2000;
	ch2->MaxOutput=2047;
	ch2->DeadBandGain=1;
	ch2->DeadBandRange=0;
	ch2->InputChan0=2;
	ch2->InputChan1=1;
	ch2->OutputChan0=2;
	ch2->OutputChan1=1;
	ch2->MasterAxis=-1;
	ch2->LimitSwitchOptions=0x12f;
	ch2->LimitSwitchNegBit=143;
	ch2->LimitSwitchPosBit=1088;
	ch2->SoftLimitPos=1e+09;
	ch2->SoftLimitNeg=-1e+09;
	ch2->InputGain0=1;
	ch2->InputGain1=1;
	ch2->InputOffset0=0;
	ch2->InputOffset1=0;
	ch2->OutputGain=1;
	ch2->OutputOffset=0;
	ch2->SlaveGain=1;
	ch2->BacklashMode=BACKLASH_OFF;
	ch2->BacklashAmount=0;
	ch2->BacklashRate=0;
	ch2->invDistPerCycle=1;
	ch2->Lead=0;
	ch2->MaxFollowingError=200;
	ch2->StepperAmplitude=250;

	ch2->iir[0].B0=1;
	ch2->iir[0].B1=0;
	ch2->iir[0].B2=0;
	ch2->iir[0].A1=0;
	ch2->iir[0].A2=0;

	ch2->iir[1].B0=1;
	ch2->iir[1].B1=0;
	ch2->iir[1].B2=0;
	ch2->iir[1].A1=0;
	ch2->iir[1].A2=0;

	ch2->iir[2].B0=1;
	ch2->iir[2].B1=0;
	ch2->iir[2].B2=0;
	ch2->iir[2].A1=0;
	ch2->iir[2].A2=0;

    EnableAxis(0);
	EnableAxis(1);
	EnableAxis(2);

    DefineCoordSystem(0,1,2,-1);
}

// Configure the Konnect IO boards connected with the KFLOP main board
void IOboardsConfig()
{
    InitAux();
	AddKonnect(0,&VirtualBits,VirtualBitsEx);
	AddKonnect(1,VirtualBitsEx+1,VirtualBitsEx+2);
}

// Sign that the initialization was executed to serve as a condition for other programs.
// Ex. not execute Homing before Init
void SetInitExecuted()
{
	persist.UserData[INIT_EXECUTED_VAR] = 1;
}

// Reset the flag that sign that the initialization was executed.
void ResetInitExecuted()
{
	persist.UserData[INIT_EXECUTED_VAR] = 0;
}

// Get a value that indicates wheter the initialization program was executed.
// Returns 1 if it was executed
int GetInitExecuted()
{
	return persist.UserData[INIT_EXECUTED_VAR];
}