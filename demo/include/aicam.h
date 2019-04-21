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

#include	<stdio.h>
#include	"aicamdef.h"

/*!	\file aicam.h 
	\brief Function prototypes.
	\author Manuele Turini
 */

/*!	\fn unsigned AIC_AvailableCameras(void)
 *	\brief Return the number of available connected cameras.
 */
CTYPE unsigned DLL AIC_AvailableCameras(void);
CTYPE unsigned DLL AIC_CameraLinkIdentifier(unsigned camera);
CTYPE const char DLL_P AIC_CameraLinkDescription(unsigned camera);
CTYPE const char DLL_P AIC_CameraLinkSerial(unsigned camera);
CTYPE AiCamHandle DLL_P AIC_CameraLinkOpen(unsigned camera);
CTYPE void DLL AIC_CameraLinkClose(AiCam handle);

// Camera init
CTYPE U16 DLL AIC_CameraInit(AiCam handle);
CTYPE U16 DLL AIC_CameraId(AiCam handle, int * ok);
CTYPE U16 DLL AIC_CameraStart(AiCam handle);

// Camera grab and transfers
CTYPE U16 DLL AIC_CameraGrab(AiCam handle);
CTYPE U16 DLL AIC_CameraGrabWait(AiCam handle);
CTYPE U16 DLL AIC_CameraGrabRef(AiCam handle);
CTYPE U16 DLL AIC_CameraReady(AiCam handle, int * ok);
CTYPE U16 DLL AIC_CameraTransfer(U16 frame, AiCam handle, ImageHandle * img);
CTYPE U16 DLL AIC_CameraTransferAverage(AiCam handle, ImageHandle * img);
CTYPE U16 DLL AIC_CameraTransferGrab(AiCam handle, ImageHandle * img);
CTYPE U16 DLL AIC_CameraTransferAverageGrab(AiCam handle, ImageHandle * img);
CTYPE U16 DLL AIC_CameraDataUpdate(AiCam handle);
CTYPE U16 DLL AIC_CameraDataType(U16 imgType, AiCam handle);
CTYPE U16 DLL AIC_CameraSetFrames(U16 frames, AiCam handle);

// Camera set
CTYPE U16 DLL AIC_CameraBinning(U16 hBin, U16 vBin, AiCam handle);
CTYPE U16 DLL AIC_CameraROI(U16 xoff, U16 yoff, U16 xSize, U16 ySize, AiCam handle);
CTYPE U16 DLL AIC_CameraAdc(U16 adcMode, AiCam handle);
CTYPE U16 DLL AIC_CameraAdcVref(U16 adcVref, AiCam handle);
CTYPE U16 DLL AIC_CameraFlip(U16 xFlip, U16 yFlip, AiCam handle);
CTYPE U16 DLL AIC_CameraModes(U16 aec, U16 agc, AiCam handle);
CTYPE U16 DLL AIC_CameraSetGain(U16 gain, AiCam handle);
CTYPE U16 DLL AIC_CameraGetGain(AiCam handle, int * ok);
CTYPE F64 DLL AIC_CameraGetGainVV(int gain);
CTYPE F64 DLL AIC_CameraGetExposureMs(AiCam handle, int * ok);
CTYPE F64 DLL AIC_CameraGetMinExposureMs(AiCam handle, int * ok);
CTYPE F64 DLL AIC_CameraGetMaxExposureMs(AiCam handle, int * ok);
CTYPE U16 DLL AIC_CameraSetExposure(U16 expo, AiCam handle);
CTYPE U16 DLL AIC_CameraSetExposureMs(F64 expo, AiCam handle);
CTYPE U16 DLL AIC_CameraAverageOpen(AiCam handle);
CTYPE U16 DLL AIC_CameraAverageClose(AiCam handle);
CTYPE U16 DLL AIC_CameraLedEnable(U16 enable, AiCam handle);
CTYPE U16 DLL AIC_CameraLedSource(U16 source, AiCam handle);

// Image functions (locally)
CTYPE ImageHandle DLL_P AIC_ImageCreate(AiCam handle);
CTYPE ImageHandle DLL_P AIC_ImageCreateBySet(U32 cols, U32 rows, U32 frames, U32 dataType);
CTYPE ImageHandle DLL_P AIC_ImageReAlloc(AiCam handle, ImageHandle * i);
CTYPE void DLL AIC_ImageClose(ImageHandle * img);
CTYPE U16 DLL AIC_ImageSaveRaw(const char * filePathName, ImageHandle * img);
CTYPE FILE DLL_P AIC_ImageSaveRawOpen(const char * filePathName, ImageHandle * img);
CTYPE U16 DLL AIC_ImageSaveRawAppend(FILE * fd, ImageHandle * img);
CTYPE U16 DLL AIC_ImageSaveRawClose(FILE * fd);
CTYPE int DLL AIC_ImageShow(const char * title, ImageHandle * img);
CTYPE int DLL AIC_ImageFrameShow(const char * title, ImageHandle * img, int frame);
CTYPE void DLL AIC_ImageShowClose(const char * title);
CTYPE U16 DLL AIC_ImageSave(const char * fileName, ImageHandle * img, int quality);
CTYPE unsigned DLL AIC_DataSize(int dt);

CTYPE void DLL AIC_ColorCodingMatrixClose(matrix3D * mg);

CTYPE void DLL AIC_RectCenter(aic_rect * r, double * xc, double * yc);

// Camera embedded
CTYPE void DLL AIC_MotionApplyToImage(int searchMode, ImageHandle * src, ImageHandle * rgbDst, matrix2D * horVector, matrix2D * verVector, matrix3D * gain);
CTYPE void DLL AIC_MotionApplyArrowToImage(ImageHandle * src, matrix2D * horVector, matrix2D * verVector);

// Memory allocation functions
CTYPE matrix2D DLL_P AIC_MatrixCreate(MatrixTypes mt, U32 horSize, U32 verSize);
CTYPE void DLL AIC_MatrixClose(matrix2D * m);

// Image functions 
CTYPE void DLL_P AIC_ImagePtr(U16 frame, ImageHandle * src);
CTYPE void DLL AIC_ImageCopy(ImageHandle * dst, ImageHandle * src);
CTYPE void DLL AIC_ImageConvert(ImageHandle * dst, ImageHandle * src, int convMode);
CTYPE void DLL AIC_ImageMonoToRGB(ImageHandle * mono, ImageHandle * rgbDst);
CTYPE void DLL AIC_ImageSetPixel(int x, int y, int rCol, int gCol, int bCol, int writeMode, ImageHandle * dst);
CTYPE void DLL AIC_ImageGetPixel(int x, int y, int * rCol, int * gCol, int * bCol, ImageHandle * dst);
CTYPE void DLL AIC_ImageSetLine(int xs, int ys, int xd, int yd, int rCol, int gCol, int bCol, int writeMode, ImageHandle * dst);
CTYPE void DLL AIC_ImageSetVector(int x, int y, int u, int v, int size, int rCol, int gCol, int bCol, int writeMode, ImageHandle * dst);
CTYPE void DLL AIC_ImageSetBox(int sx, int sy, int dx, int dy, int rCol, int gCol, int bCol, int writeMode, ImageHandle * dst);
CTYPE void DLL AIC_ImageSetValue(int value, ImageHandle * dst);
CTYPE void DLL AIC_ImageDivideByValue(int value, ImageHandle * dst);
CTYPE void DLL AIC_ImageCentroid(ImageHandle * src, int xSta, int xEnd, int ySta, int yEnd, float * outX, float * outY);

// Image functions on DSP
CTYPE U16 DLL AIC_ImageSub(AiCam handle);
CTYPE U16 DLL AIC_ImageXor(AiCam handle);
CTYPE U16 DLL AIC_ImageNot(AiCam handle);
CTYPE U32 DLL AIC_ImageBlobs(U16 threshold, AiCam handle, int * ok);
CTYPE U16 DLL AIC_ImageBlobsData(U16 index, aic_rect * r, AiCam handle);
CTYPE U16 DLL AIC_ImageMotionVector(U16 searchFactor, AiCam handle);
CTYPE U16 DLL AIC_ImageMotionVectorReady(AiCam handle);
CTYPE U16 DLL AIC_ImageMotionGetVectorData(matrix2D * horVector, matrix2D * verVector, AiCam handle);


// Sensor related
CTYPE U16 DLL AIC_SensorHorPixel(AiCam handle, int * ok);
CTYPE U16 DLL AIC_SensorVerPixel(AiCam handle, int * ok);
CTYPE U16 DLL AIC_SensorId(AiCam handle, int * ok);
CTYPE U16 DLL AIC_SensorVer(AiCam handle, int * ok);
CTYPE U16 DLL AIC_SensorRdReg(U16 reg, AiCam handle, int * ok);
CTYPE U16 DLL AIC_SensorWrReg(U16 reg, U16 val, AiCam handle);

// 
CTYPE U16 DLL AIC_NopCommand(AiCam handle);
CTYPE U16 DLL AIC_FirmwareRevision(AiCam handle, int * ok);
CTYPE U32 DLL AIC_ElapsedTime(AiCam handle, int * ok);

// ROI and statistics
CTYPE roi DLL_P AIC_RoiCreate(ImageHandle * i);
CTYPE roi DLL_P AIC_RoiSet(roi * r, ImageHandle * i);
CTYPE void DLL AIC_RoiStatistics(int op, roi * r, ImageHandle * i);

// Disk access
CTYPE U16 DLL AIC_DiskMount(int enable, AiCam handle, int * ok);
CTYPE U16 DLL AIC_DiskFileList(const char * path, FILINFO ** fileList, AiCam handle);
CTYPE U16 DLL AIC_ReadFileFrom(const char * srcPath, const char * dstPath, AiCam handle);	// PC <- AiCam
CTYPE U16 DLL AIC_DiskCopyFilesTo(const char * srcPath, const char * dstPath, AiCam handle);	// PC -> AiCam
// Low level I/O
CTYPE U16 DLL AIC_BufferAlloc(unsigned kBytes, AiCam handle);
CTYPE U16 DLL AIC_BufferFree(AiCam handle);
CTYPE U16 DLL AIC_BufferRead(char * dst, U16 offset, U16 len, AiCam handle);
CTYPE U16 DLL AIC_BufferWrite(const char * src, U16 offset, U16 len, AiCam handle);

// Library init
CTYPE void DLL AIC_SetProgressCallback(AiCamProgress func);