#ifndef __REMOTE_COMMANDS__
#define __REMOTE_COMMANDS__

#include "aicam.h"

// Parameter index access by ReadConfig command
enum DSPIN_PARAMS {
	DSPIN_Acc,
	DSPIN_Dec,
	DSPIN_MaxSpeed,
	DSPIN_MinSpeed,
	DSPIN_HoldKVAL,
	DSPIN_RunKVAL,
	DSPIN_AccKVAL,
	DSPIN_DecKVAL,
	DSPIN_IntersectSpeed,
	DSPIN_StartSlope,
	DSPIN_SlopeFinalAcc,
	DSPIN_SlopeFinalDec,
	DSPIN_KTherm,
	DSPIN_OcdThreshold,
	DSPIN_StallThreshold,
	DSPIN_FullStepSpeed,
	DSPIN_StepMode,
	DSPIN_Config,
};

typedef struct dspinSettings {
	U32     identifier;         // Block identifier
	U16     accel;				// 05: Acceleration Register,				0:4095	[steps/s/s] to value: raw / 0.137438
	U16     decel;				// 06: Deceleration Register,				0:4095	[steps/s/s] to value: raw / 0.137438
	U16     maxSpeed;			// 07: MaximumSpeed Register,				0:1023	[steps/s]   to value: raw / 0.065536
	U16     minSpeed;			// 08: MinimumSpeed Register,				0:4095	[steps/s]   to value: raw / 0.238
	U8      hldKVAL;			// 09: HoldingKval Register,				0:255
	U8      runKVAL;			// 10: ConstantSpeedKval Register,			0:255
	U8      accKVAL;			// 11: AccelerationStartingKval Register,	0:255
	U8      decKVAL;			// 12: DecelerationStartingKval Register,	0:255
	U16		intSpeed;			// 13: IntersectSpeed Register,				0:16383	[steps/s]   to value: raw / 4.1943
	U8		startSlope;			// 14: StartSlope Register,					0:255
	U8		finalSlopeAcc;		// 15: AccelerationFinalSlope Register,		0:255
	U8		finalSlopeDec;		// 16: DecelerationFinalSlope Register,		0:255
	U8		kTherm;				// 17: ThermalCompensationFactor Register,	0:15    1 + n x 0.03125
	U8      current;			// 19: OcdThreshold Register,				0:15	(raw + 1) x 0.375 -> 0.375A, 0.75A, 1.125A, 1.5A, 1.875A, 2.25A, 2.625A, 3A, 3.375A, 3.75A, 4.125A, 4.5A, 4.875A, 5.25A, 5.625A, 6A
	U8      stallThreshold;		// 20: StallThreshold Register,				0:127	(raw + 1) x 31.25 mA
	U16     fullStepSpeed;		// 21: FullStepSpeed Register,				0:1023	steps/s]   to value: (raw + 0.5) / 0.065536
	U8      stepMode;           // 22: StepMode Register,					0:7		200,400,800,1600,3200,6400,12800,25600 step/r
	U16		conf;				// 24: Config Register,
}dspinSettings;

// Remote commands for Hercules board. Connected on UART 1 of CORE-CAM
U16		HRC_Rev(AiCam handle);
bool	HRC_Move(AiCam handle, int axys, int dir, unsigned steps);
bool	HRC_MotorOff(AiCam handle, int axys);
U16		HRC_PowerOutput(AiCam handle, char output, bool ena);
bool	HRC_GetConfig(AiCam handle, int axys, dspinSettings * set);
bool	HRC_PutConfig(AiCam handle, int axys, dspinSettings * set);

#endif