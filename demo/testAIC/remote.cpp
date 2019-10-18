#include <windows.h>
#include <conio.h>
#include <direct.h>
#include <math.h>
#include "aicam.h"
#include "_string.h"
#include "remote.h"

U16 HRC_RetCode(AiCam handle)
{
	char		str[16];
	bool		ret = false;
	U16			num = 0xFFFF;
	unsigned	count;

	char		retFields[1024];
	AIC_CommandPipe(handle, AIC_PIPE_SERIAL_RX1_LEN);
	num = AIC_CommandPipeRetCode();
	if(num)
	{
		AIC_CommandPipe(handle, AIC_PIPE_SERIAL_RX1, num, str);
		count = _str_split(str, ',', 8, retFields);
		if(count == 2)
			num = _atoi((char *) _str_field(retFields, 1));
	}
	return num;
}

// 
U16 HRC_Rev(AiCam handle)
{
	U16		ret;
	char	str[1024];
	sprintf(str, "$0,REV\r");
	AIC_CommandPipe(handle, AIC_PIPE_SERIAL_TX1, strlen(str), str);
	ret = AIC_CommandPipeRetCode();
	if(ret == AIC_DSP_ACK)
	{
		Sleep(10);
		ret = HRC_RetCode(handle);
	}
	else ret = 0xFFFF;
	return ret;
}

// output must be in the range 1:4
U16 HRC_PowerOutput(AiCam handle, char output, bool ena)
{
	U16		ret;
	char	str[1024];
	sprintf(str, "$0,WRB,%u,%u\r", output,  ena ? 1 : 0);
	AIC_CommandPipe(handle, AIC_PIPE_SERIAL_TX1, strlen(str), str);
	ret = AIC_CommandPipeRetCode();
	if(ret == AIC_DSP_ACK)
	{
		Sleep(10);
		ret = HRC_RetCode(handle);
	}
	else ret = 0xFFFF;
	return ret;
}

bool HRC_Move(AiCam handle, int axys, int dir, unsigned steps)
{
	U16		ret;
	char	str[1024];
	sprintf(str, "$0,MOV,%d,%d,%d\r", axys, dir, steps);
	AIC_CommandPipe(handle, AIC_PIPE_SERIAL_TX1, strlen(str), str);
	ret = AIC_CommandPipeRetCode();
	return ret == AIC_DSP_ACK;
}

bool HRC_MotorOff(AiCam handle, int axys)
{
	U16		ret;
	char	str[1024];
	sprintf(str, "$0,SFZ,%d\r", axys);
	AIC_CommandPipe(handle, AIC_PIPE_SERIAL_TX1, strlen(str), str);
	ret = AIC_CommandPipeRetCode();
	return ret == AIC_DSP_ACK;
}

bool HRC_GetConfig(AiCam handle, int axys, dspinSettings * set)
{
	U16		ret;
	char	str[1024];

	for(int i = DSPIN_Acc; i <= DSPIN_Config; i++)
	{
		sprintf(str, "$0,RCF,%d,%d\r", axys, i);
		AIC_CommandPipe(handle, AIC_PIPE_SERIAL_TX1, strlen(str), str);
		ret = AIC_CommandPipeRetCode();
		if(ret == AIC_DSP_ACK)
		{
			Sleep(1);
			switch(i)
			{
			case DSPIN_Acc:				set->accel = HRC_RetCode(handle);			break;
			case DSPIN_Dec:				set->decel = HRC_RetCode(handle);			break;
			case DSPIN_MaxSpeed:		set->maxSpeed = HRC_RetCode(handle);		break;
			case DSPIN_MinSpeed:		set->minSpeed = HRC_RetCode(handle);		break;
			case DSPIN_HoldKVAL:		set->hldKVAL = HRC_RetCode(handle);			break;
			case DSPIN_RunKVAL:			set->runKVAL = HRC_RetCode(handle);			break;
			case DSPIN_AccKVAL:			set->accKVAL = HRC_RetCode(handle);			break;
			case DSPIN_DecKVAL:			set->decKVAL = HRC_RetCode(handle);			break;
			case DSPIN_IntersectSpeed:	set->intSpeed = HRC_RetCode(handle);		break;
			case DSPIN_StartSlope:		set->startSlope = HRC_RetCode(handle);		break;
			case DSPIN_SlopeFinalAcc:	set->finalSlopeAcc = HRC_RetCode(handle);	break;
			case DSPIN_SlopeFinalDec:	set->finalSlopeDec = HRC_RetCode(handle);	break;
			case DSPIN_KTherm:			set->kTherm = HRC_RetCode(handle);			break;
			case DSPIN_OcdThreshold:	set->current = HRC_RetCode(handle);			break;
			case DSPIN_StallThreshold:	set->stallThreshold = HRC_RetCode(handle);	break;
			case DSPIN_FullStepSpeed:	set->fullStepSpeed = HRC_RetCode(handle);	break;
			case DSPIN_StepMode:		set->stepMode = HRC_RetCode(handle);		break;
			case DSPIN_Config:			set->conf = HRC_RetCode(handle);			break;
			}
		} else return false;
	}
	return true;
}

bool HRC_PutConfig(AiCam handle, int axys, dspinSettings * set)
{
	U16		ret;
	char	str[1024];
	int		v;

	for(int i = DSPIN_Acc; i <= DSPIN_Config; i++)
	{
		switch(i)
		{
		case DSPIN_Acc:				v = set->accel;			break;
		case DSPIN_Dec:				v = set->decel;			break;
		case DSPIN_MaxSpeed:		v = set->maxSpeed;		break;
		case DSPIN_MinSpeed:		v = set->minSpeed;		break;
		case DSPIN_HoldKVAL:		v = set->hldKVAL;		break;
		case DSPIN_RunKVAL:			v = set->runKVAL;		break;
		case DSPIN_AccKVAL:			v = set->accKVAL;		break;
		case DSPIN_DecKVAL:			v = set->decKVAL;		break;
		case DSPIN_IntersectSpeed:	v = set->intSpeed;		break;
		case DSPIN_StartSlope:		v = set->startSlope;	break;
		case DSPIN_SlopeFinalAcc:	v = set->finalSlopeAcc;	break;
		case DSPIN_SlopeFinalDec:	v = set->finalSlopeDec;	break;
		case DSPIN_KTherm:			v = set->kTherm;		break;
		case DSPIN_OcdThreshold:	v = set->current;		break;
		case DSPIN_StallThreshold:	v = set->stallThreshold;break;
		case DSPIN_FullStepSpeed:	v = set->fullStepSpeed;	break;
		case DSPIN_StepMode:		v = set->stepMode;		break;
		case DSPIN_Config:			v = set->conf;			break;
		}
		sprintf(str, "$0,WCF,%d,%d,%d\r", axys, i, v);
		AIC_CommandPipe(handle, AIC_PIPE_SERIAL_TX1, strlen(str), str);
		ret = AIC_CommandPipeRetCode();
		if(ret != AIC_DSP_ACK)
			return false;
	}
	return true;
}