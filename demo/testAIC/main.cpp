/*  This file is part of CORE-CAM Driver.
	Copyright 2018-2019 Manuele Turini

	CORE-CAM Driver is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	CORE-CAM Driver is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with CORE-CAM Driver.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <windows.h>
#include <conio.h>
#include <direct.h>
#include <math.h>
#include "aicam.h"
#include "_string.h"
#include "remote.h"

extern void StartTimer();
extern unsigned long long EndTimer();

#define ARGV(a) _str_field(Argv, a)
#define ST	st = GetTickCount()
#define ET	et = GetTickCount()

char 	cmdLine[1024];

// Path to image display application
char	* appPath = "..\\..\\MDI\\bin\\mdi.exe";

AiCam	cam = 0;

ImageHandle * img = 0;
roi * r = 0;

void _waiting()
{
	static int  c = 0;
	char        s[5] = {"|/-\\"};
	printf("%c\r", s[c % 4]);
	c++;
}

void printStatistics(roi * r)
{
	printf("Image resolution  : %dx%d (HxV)\n", r->dim.w, r->dim.h);
	printf("ROI start         : %d,%d\n", r->sta.x, r->sta.y);
	printf("ROI end           : %d,%d\n", r->end.x, r->end.y);
	printf("Number of pixel   : %.0lf\n", r->area);
	printf("Total value       : %.0lf\n", r->sum);
	printf("Mean              : %lf\n", r->media);
	printf("Mean absolute dev : %lf\n", r->adev);
	printf("Standard Deviation: %lf\n", r->sigma);
	printf("Variance          : %lf\n", r->varia);
	printf("Kurtosis          : %lf\n", r->curt);
	printf("Skew              : %lf\n", r->skew);
	printf("Minimum           : %.0lf\n", r->min);
	printf("Maximum           : %.0lf\n", r->max);
	printf("Contrast          : %lf\n", r->ctrst);
	printf("Range             : %.0lf\n", r->range);
	printf("Entropy sum       : %lf\n", r->entropy);
	printf("Maximum entropy   : %lf\n", r->maxent);
	printf("Number of tokens  : %lf\n", r->ntokens);
	printf("Ratio of entropy  : %lf\n", r->eratio);
}

int _info()
{
	int		ok;
	printf("Camera identifier : %d\n", cam->camera.platform);
	printf("Firmware revision : %.2lf\n", (double) AIC_FirmwareRevision(cam, &ok) / 100.0);
	printf("Sensor Identifier : %d\n", cam->camera.sensor.sensor);
	printf("Sensor Resolution : %dx%d (HxV)\n", cam->camera.sensor.hor, cam->camera.sensor.ver);
	printf("Sensor Revision   : %X\n", cam->camera.sensor.sensorRev);
	return 0;
}

int _statistics()
{
	if(!r)
		r = AIC_RoiCreate(img);
	// Image evaluate
	AIC_RoiStatistics(SALL, r, img);
	printStatistics(r);
	return 0;
}

int _show()
{
// 	char	com[1024];
// 	if(Argc == 2)
// 	{
// 		sprintf(com, "%s %s", appPath, (char *) ARGV(1));
// 		system(com);
// 	} else printf("Syntax error: SHOWE imageFileName.raw\n");
 	return 0;
}

int _showWin()
{
	AIC_ImageShow("IMG", img);
	return 0;
}

int _imageShow()
{
	char	* path, com[1024];
	path = _getcwd(NULL, 0);
	if(AIC_ImageSaveRaw("_IMG_.RAW", img))
	{
		sprintf(com, "%s %s\\_IMG_.RAW\n", appPath, path);
		system(com);
	}
	return 0;
}

int _focusLensControl()
{
	bool		ok;
	U16			ret;
	char		ch = 0;
	unsigned	lc = 0;
	int			k = 0;
	unsigned long long et;
	if(!img)
		img = AIC_ImageCreate(cam);
	if(img)
	{
		while(ch != '.')
		{
			if(_kbhit())
			{
				ok = false;
				ch = _getch();
				switch(ch)
				{
				case '+':	// Inc zoom, inc/dec, steps, step delay (ms)
							AIC_CommandPipe(cam, AIC_PIPE_LENS_ZOOM, 0, 100, 5);
							ok = true;
							break;
				case '-':	// Dec zoom, inc/dec, steps, step delay (ms)
							AIC_CommandPipe(cam, AIC_PIPE_LENS_ZOOM, 1, 100, 5);
							ok = true;
							break;
				case 'f':	// Inc focus, inc/dec, steps, step delay (ms)
							AIC_CommandPipe(cam, AIC_PIPE_LENS_FOCUS, 0, 100, 5);
							ok = true;
							break;
				case 'F':	// Dec focus, inc/dec, steps, step delay (ms)
							AIC_CommandPipe(cam, AIC_PIPE_LENS_FOCUS, 1, 100, 5);
							ok = true;
							break;
				case 'i':	// IR cut off
							AIC_CommandPipe(cam, AIC_PIPE_LENS_IR, 0);
							ok = true;
							break;
				case 'I':	// IR cut on
							AIC_CommandPipe(cam, AIC_PIPE_LENS_IR, 1);
							ok = true;
							break;
				}
				if(ok)
				{
					ret = AIC_CommandPipeRetCode();
					if(ret != AIC_DSP_ACK)
					{
						printf("Command fail!\n");
						return 0;
					}
				}
			}
			StartTimer();
			if(AIC_CameraGrabWait(cam))
			{
				AIC_CameraTransfer(0, cam, img);
				et = EndTimer();
				lc++;
				AIC_ImageShow("FOCUS", img);
				if((lc % 5) == 0)
					printf("[%6u] Elapsed time: %7.1lf mS @ %.1lf fps\n", lc, (double) et / 1000.0, 1E6 / (double) et);
			}
		}
		AIC_ImageShowClose("FOCUS");
	}
	return 0;
}

int _focus(bool avg = false, bool not = false)
{
	unsigned	lc = 0;
	int			k = 0;
	unsigned long long et;
	if(!img)
		img = AIC_ImageCreate(cam);
	if(img)
	{
		//SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS); 
		//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL); 
		while(!_kbhit())
		{
			StartTimer();
			if(AIC_CameraGrabWait(cam))
			{
				if(not)
					AIC_ImageNot(cam);
				if(avg)
					AIC_CameraTransferAverage(cam, img);
				else
					AIC_CameraTransfer(0, cam, img);
				et = EndTimer();
				lc++;
				k = AIC_ImageShow("FOCUS", img);
				if((lc % 5) == 0)
					printf("[%6u] Elapsed time: %7.1lf mS @ %.1lf fps\n", lc, (double) et / 1000.0, 1E6 / (double) et);
			}
			if(k != 255) break;
		}
		//SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);          
		//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
		AIC_ImageShowClose("FOCUS");
	}
	return 0;
}

int _splitFocusAction()
{
	unsigned	lc = 0;
	int			k = 0;
	unsigned long long et;

	if(!img)
		img = AIC_ImageCreate(cam);
	if(img)
	{
		StartTimer();
		AIC_CameraGrab(cam);
		while(!_kbhit())
		{
			AIC_CameraTransferGrab(0, cam, img);
			et = EndTimer();
			StartTimer();
			lc++;
			k = AIC_ImageShow("FOCUS", img);
			if((lc % 5) == 0)
				printf("[%6u] Elapsed time: %7.1lf mS @ %.1lf fps\n", lc, (double) et / 1000.0, 1E6 / (double) et);
			if(k != 255) break;
		}
		AIC_ImageShowClose("FOCUS");
	}
	return 0;
}

void testTimer()
{
	unsigned long long et;
	StartTimer();
	Sleep(1000);
	et = EndTimer();
	printf("Timer test (1000mS): %7.3lf mS\n", (double) et / 1000.0);
}

void commandList()
{
	printf(" 0 - 752x480 Bin 1x1 @ 16bit\n");
	printf(" 1 - 376x240 Bin 2x2 @ 16bit\n");
	printf(" 2 - 188x120 Bin 4x4 @ 16bit\n");
	printf(" 3 - 752x480 Bin 1x1 @  8bit Low nibble\n");
	printf(" 4 - 376x240 Bin 2x2 @  8bit Low nibble\n");
	printf(" 5 - 188x120 Bin 4x4 @  8bit Low nibble\n");
	printf(" 6 - 128x128 Bin 1x1 @ 16bit ROI\n");
	printf(" 7 - 128x120 Bin 2x2 @ 16bit ROI\n");
	printf(" 8 - 128x128 Bin 1x1 @  8bit ROI\n");
	printf(" 9 - 752x480 Bin 1x1 @  8bit High nibble\n");
	printf("10 - 752x480 Avg by 10 frames\n");
	printf("11 - Grab 1000 frames 188x120 Bin 4x4 @ 16bit\n");
	printf("12 - Show SD contents\n");
	printf("13 - Copy SD contents\n");
	printf("14 - Split action in focus\n");
	printf("15 - Image negate\n");
	printf("16 - Create a file on SD\n");
	printf("17 - Lens controls\n");
	printf("18 - Remote command\n");
	printf("> ");
}

//#define LOOP_BACK_TEST	1
#define LOOP_BACK_NUM	(65536 * 4)

int main(int argc, char * argv[])
{
	printf("CORE-CAM USB 2 Library tester. Rev. 1.0\n");
	printf("Copyright 2018-2019 Manuele Turini\n");

	// Find available cameras on all USB available channels
	int		usb = AIC_AvailableCameras(), n, sc, OK;
	
	// Simple program to check timer
	// testTimer();

	if(usb > 0)
	{
		// List available cameras
		for(n = 0; n < usb; n++)
		{
			printf("LINK [%d] = ID   [%16d]\n", n, AIC_CameraLinkIdentifier(n));
			printf("LINK [%d] = SN   [%16s]\n", n, AIC_CameraLinkSerial(n));
			printf("LINK [%d] = DESC [%16s]\n", n, AIC_CameraLinkDescription(n));
		}
		printf("\nSelect camera LINK index [0:%d] =  ", usb - 1);
		scanf("%d", &sc);				// Wait user selection
		cam = AIC_CameraLinkOpen(sc);	// Open the USB channel for selected camera
		if(cam)
		{
#ifdef LOOP_BACK_TEST
			char		ch;
			unsigned long long et, tot = 0;
			U32			* mem = (U32 *) malloc(LOOP_BACK_NUM * sizeof(U32));
			unsigned	loop, i;
			printf("\nTest for USB connection, run a test for values of 10000 or higher.\n");
			printf("At each test 1Mbytes are transferred and checked.\n");
			printf("Test to execute: ");
			scanf("%u", &loop);
			printf("\n");
			for(unsigned l = 0; l < loop; l++)
			{
				StartTimer();
				if(AIC_LoopBackTest(mem, LOOP_BACK_NUM, cam))
				{
					et = EndTimer();
					// Check received data
					for(i = 0; i < LOOP_BACK_NUM; i++)
						if(i != mem[i])
						{
							printf("\n[%u] fail at %u: found %X instead of %X\n", l, i, mem[i], i);
							break;
						}
					if(i == LOOP_BACK_NUM)
					{
						tot += (long long) LOOP_BACK_NUM * sizeof(U32);
						printf("%3.1lf%% %4llu mS %6llu byte/s %5llu Mbyte\r", (double) (l + 1) * 100.0 / (double) loop, et / 1000ll, (long long) LOOP_BACK_NUM * sizeof(U32) * 1000000ll / et, tot / (1024ll * 1024ll));
					}
				}
				else
				{
					printf("\nLoop back command fail\n");
					break;
				}
				if(i < LOOP_BACK_NUM)
					break;
			}
			free(mem);
			printf("\nEnd of test, press any key to exit.\n");
			ch = getch();
			return 0;
#endif
			if(AIC_CameraStart(cam))	// Open connection with selected camera
			{
				bool	avg = false, not = false;
				// Stop automatic gain and exposure
				OK = AIC_CameraModes(false, true, cam);
				// Set 0.5 ms exposure time
				AIC_CameraSetExposureMs(0.5, cam);

				commandList(); scanf("%d", &sc);		// Wait user command selection

				// Demos, choose your test
				switch(sc)
				{
				case 0:	// 752x480 16bit						[ 6.4 fps]
						break;
				case 1:	// Binning 2x2							[  23 fps]
						OK = AIC_CameraBinning(2, 2, cam);
						break;
				case 2:	// Binning 4x4							[  48 fps]
						OK = AIC_CameraBinning(4, 4, cam);
						break;
				case 3:	// Try 8 bit transfer					[   9 fps]
						OK = AIC_CameraDataType(IMG_U8, cam);
						// Set a low gain
						OK = AIC_CameraSetGain(1, cam);
						// Set ADC reference to 2.1 to prevent 8 bit overflow
						OK = AIC_CameraAdcVref(7, cam);
						break;
				case 4:	// Try 8 bit transfer @ bin 2x2			[  23 fps]
						OK = AIC_CameraDataType(IMG_U8, cam);
						// Set ADC reference to 2.1 to prevent 8 bit overflow
						OK = AIC_CameraAdcVref(7, cam);
						OK = AIC_CameraBinning(2, 2, cam);
						break;
				case 5:	// Try 8 bit transfer @ bin 4x4			[  36 fps]
						OK = AIC_CameraDataType(IMG_U8, cam);
						// Set ADC reference to 2.1 to prevent 8 bit overflow
						OK = AIC_CameraAdcVref(7, cam);
						OK = AIC_CameraBinning(4, 4, cam);
						break;
				case 6:	// Create a ROI of 128x128				[  53 fps]
						OK = AIC_CameraROI(312, 176, 128, 128, cam);
						break;
				case 7:	// Create a ROI of 128x120 @ bin 2x2	[  53 fps]
						OK = AIC_CameraBinning(2, 2, cam);
						OK = AIC_CameraROI(124, 0, 128, 120, cam);
						break;
				case 8:	// Create a ROI of 128x128 @ 8bit		[  60 fps]
						OK = AIC_CameraDataType(IMG_U8, cam);
						// Set ADC reference to 2.1 to prevent 8 bit overflow
						OK = AIC_CameraAdcVref(7, cam);
						OK = AIC_CameraROI(312, 56, 128, 128, cam);
						break;
				case 9:	// 752x480 8bit high nibble				[   9 fps]
						// Locally set x an 8bit image
						cam->imgCamSet.type = IMG_U8;
						cam->imgCamSet.dataSize = AIC_DataSize(IMG_U8);
						img = AIC_ImageCreate(cam);
						// Will transfer high 8 bit
						img->imgSet.nibble = 1;
						break;
				case 10:// 752x480 16bit average from 5 frames
						{
							OK = AIC_CameraDataType(IMG_U16, cam);
							OK = AIC_CameraSetFrames(5, cam);
							OK = AIC_CameraDataUpdate(cam);
							OK = AIC_CameraAverageOpen(cam);
							_focus(true);
							OK = AIC_CameraAverageClose(cam);
							AIC_CameraLinkClose(cam);	// Close USB channel
							return 0;
						}
						break;
				case 11:// Grab 1000 frames with binning 4x4 on CORE-CAM (max fps)
						//	transfer all and save in RAW format
						{
							U16		f;
							unsigned long long et;
							OK = AIC_CameraBinning(4, 4, cam);
							OK = AIC_CameraDataType(IMG_U16, cam);
							// RAM usage: 188x120x1000x2 = 45120000 bytes
							OK = AIC_CameraSetFrames(1000, cam);
							OK = AIC_CameraDataUpdate(cam);
							// Create local image stack
							img = AIC_ImageCreate(cam);
							printf("Started ... ");
							StartTimer();
							// Start CORE-CAM grab x 1000 frames
							OK = AIC_CameraGrabWait(cam);
							et = EndTimer();
							printf("End\n");
							printf("CORE-CAM elapsed time: %7.3lf mS @ %.1lf fps\n", (double) et / 1000.0, 1E9 / (double) et);
							StartTimer();
							// Transfer all data
							for(f = 0; f < 1000; f++)
							{
								AIC_CameraTransfer(f, cam, img);
								if(((f + 1) % 100) == 0)
									printf("[%4u]\n", f + 1);
							}
							et = EndTimer();
							printf("Transfer elapsed time: %7.3lf mS @ %.1lf fps\n", (double) et / 1000.0, 1E9 / (double) et);
							AIC_ImageSaveRaw("1000frames.raw", img);
							AIC_CameraLinkClose(cam);
							getchar();
							return 0;
						}
						break;
				case 12:// Show SD contents
						{
							if(AIC_DiskMount(true, cam, 0))
							{
								unsigned	nf, i;
								FILINFO		** fileList;
								fileList = (FILEINFO **) malloc(sizeof(FILEINFO *) * 1024);
								printf("SD mount OK\n");
								printf("CWD: %s\n", AIC_DiskPath(cam, 0));
								nf = AIC_DiskFileList("/", fileList, cam);
								printf("Files: %d\n", nf);
								for(i = 0; i < nf; i++)
									printf("%c%c%c%c%c %u/%02u/%02u %02u:%02u %10u  %s\n",
											(fileList[i]->fattrib & AM_DIR) ? 'D' : '-',
											(fileList[i]->fattrib & AM_RDO) ? 'R' : '-',
											(fileList[i]->fattrib & AM_HID) ? 'H' : '-',
											(fileList[i]->fattrib & AM_SYS) ? 'S' : '-',
											(fileList[i]->fattrib & AM_ARC) ? 'A' : '-',
											(fileList[i]->fdate >> 9) + 1980, 
											(fileList[i]->fdate >> 5) & 15, 
											 fileList[i]->fdate & 31,
											(fileList[i]->ftime >> 11), 
											(fileList[i]->ftime >> 5) & 63,
											 fileList[i]->fsize,
											&(fileList[i]->fname[0]));
								AIC_DiskMount(false, cam, 0);
							}
							AIC_CameraLinkClose(cam);	// Close USB channel
							return 0;
						}
						break;
				case 13:// Copy SD file contents
						{
							if(AIC_DiskMount(true, cam, 0))
							{
								unsigned long long	amount = 0;
								unsigned	nf, i, et;
								FILINFO		** fileList;
								char		dest[1024];
								fileList = (FILEINFO **) malloc(sizeof(FILEINFO *) * 1024);
								printf("SD mount OK\n");
								nf = AIC_DiskFileList("/", fileList, cam);
								printf("Files: %d\n", nf);
								et = GetTickCount();
								for(i = 0; i < nf; i++)
								{
									if(fileList[i]->fattrib & AM_DIR)
									{
										// Do nothing
									}
									else
									{
										// Check if this directory exists!
										strcpy((char *) dest, "C:\\core-cam\\");
										strcat(dest, (const char *) &(fileList[i]->fname[0]));
										printf("[%4u]:[%9u] -> %s\n", i + 1, fileList[i]->fsize, &(fileList[i]->fname[0]));
										AIC_GetFile((const char *) &(fileList[i]->fname[0]), dest, cam);
										amount += fileList[i]->fsize;
									}
								}
								et = GetTickCount() - et;
								AIC_DiskMount(false, cam, 0);
								printf("Transferred %u files, total amount %llu Kb, elapsed time %.1lf\n", nf, amount / 1024, (double) et / 1000.0);
							}
							AIC_CameraLinkClose(cam);	// Close USB channel
							return 0;
						}
						break;
				case 14:// Split focus action
						break;
				case 15:// Not
						not = true;
						break;
				case 16:// Create a file on CORE-CAM SD
						if(AIC_DiskMount(true, cam, 0))
						{
							FILE	* fd = fopen("C:\\hello.txt", "wb");
							for(int i = 0; i < 32768; i++)
								fprintf(fd, "Hello world\n");
							fclose(fd);
							AIC_PutFile("C:\\hello.txt", "/hello.txt", cam);
						}
						return 0;
						break;
				case 17:// Lens control
						{
							AIC_CameraModes(true, true, cam);
							AIC_CameraDataUpdate(cam);
							_focusLensControl();
							AIC_CameraLinkClose(cam);
							return 0;
						}
						break;
				case 18:// Remote command
						{
							U16	pRev = HRC_Rev(cam);
							if(pRev < 100)
								printf("Hercules board not connected!\n");
							else
							{
								dspinSettings	mot[3];
								printf("Hercules board connected!\n");
								printf("Set power output 1 ON for 5s\n");
								HRC_PowerOutput(cam, 1, true);
								Sleep(5000);
								HRC_PowerOutput(cam, 1, false);
								printf("Set power output 1 OFF\n");
								// Read and dump motor settings
								for(int i = 0; i < 4; i++)
								{
									if(HRC_GetConfig(cam, i, &mot[i]))
									{
										printf("[%d] Acceleration              Register = %10.2lf steps/s/s\n", i, (double) mot[i].accel / 0.137438);
										printf("[%d] Deceleration              Register = %10.2lf steps/s/s\n", i, (double) mot[i].decel / 0.137438);
										printf("[%d] MaximumSpeed              Register = %10.2lf steps/s\n", i, (double) mot[i].maxSpeed / 0.065536);
										printf("[%d] MinimumSpeed              Register = %10.2lf steps/s\n", i, (double) mot[i].minSpeed / 0.238);
										printf("[%d] HoldingKval               Register = %10d\n", i, mot[i].hldKVAL);
										printf("[%d] ConstantSpeedKval         Register = %10d\n", i, mot[i].runKVAL);
										printf("[%d] AccelerationStartingKval  Register = %10d\n", i, mot[i].accKVAL);
										printf("[%d] DecelerationStartingKval  Register = %10d\n", i, mot[i].decKVAL);
										printf("[%d] IntersectSpeed            Register = %10.2lf steps/s\n", i, (double) mot[i].intSpeed / 4.1943);
										printf("[%d] StartSlope                Register = %10d\n", i, mot[i].startSlope);
										printf("[%d] AccelerationFinalSlope    Register = %10d\n", i, mot[i].finalSlopeAcc);
										printf("[%d] DecelerationFinalSlope    Register = %10d\n", i, mot[i].finalSlopeDec);
										printf("[%d] ThermalCompensationFactor Register = %10.2lf\n", i, (double) mot[i].kTherm * 0.03125 + 1.0);
										printf("[%d] OcdThreshold              Register = %10.2lf A\n", i, (double) (mot[i].current + 1) * 0.375);
										printf("[%d] StallThreshold            Register = %10.2lf A\n", i, (double) (mot[i].stallThreshold + 1) * 0.03125);
										printf("[%d] FullStepSpeed             Register = %10.2lf steps/s\n", i, ((double) mot[i].fullStepSpeed + 0.5) / 0.065536);
										printf("[%d] StepMode                  Register = %10d s/r\n", i, (int) pow(2.0, (double) (mot[i].stepMode + 1)) * 100);
										printf("[%d] Config                    Register = %10X\n", i, mot[i].conf);
									}
								}
							}
							return 0;
						}
						break;
				}
				// Always send this command when image size or type is changed!
				//	also the first time you want to grab and transfer an image
				AIC_CameraDataUpdate(cam);
				if(img)
					AIC_ImageReAlloc(cam, img);
				if(sc == 14)
					_splitFocusAction();
				else
					_focus(avg, not);

				AIC_CameraLinkClose(cam);	// Close USB channel
			}
			else printf("Camera processor not answer.\n");
		}
		else printf("Can't open selected camera.\n");
	}
	else printf("Nothing AiCam compatible camera is connected.\n");
	return 0;
}