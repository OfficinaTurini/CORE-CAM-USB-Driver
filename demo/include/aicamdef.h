/*****************************************************************************
 *****************************************************************************
 **                                                                         **
 **        AiCam Types & Constants definitions for camera control           **
 **                     for Windows DLL or Linux LIB                        **
 **          (C) 2016-2019 Officina Turini, All Rights Reserved             **
 **                                                                         **
 **                                                                         **
 **   Develope contributes:    Manuele Turini                               **
 **                                                                         **
 *****************************************************************************
 *****************************************************************************/

#ifndef AICAM_DEFINITIONS
#define AICAM_DEFINITIONS

/*!	\mainpage Main Index
	- \ref intro \n
		- \ref instal \n
			- \ref files \n
 	
	\section intro Introduction
  	This series of libraries allows for the complete management of AiCam OT cameras.\n
	They have been made using the classic DLL technique, or the library for LINUX;\n
	With this modality all you need to do is declare the include aicam.h file within your own C program to obtain complete access to the library.\n
	To become familiar with the procedures available you should first of all examine the testAIC.sln\n
	example project, which can be opened with the Visual 2010 or higher.\n
	This library runs on Windows 32-bit platforms such as XP, VISTA, Windows 7, Windows 8/10 or LINUX.\n

	\section instal Installation
	The installation of the library is of course always linked to the installation of the camera  :\n
		-# If requested by your camera model you must install the requested driver for USB interface. For more informations always refer to the camera manual.\n
		-# Copy the \ref files into your working directory.\n

	\section files Library Files
	When you copy the files of the library into the work directory you have to be well aware of the necessary ones:\n
		-# aicamV2.dll and aicam.lib for Windows or aicamV2.a for Linux : this is the library !\n
		-# aicam.h and aicamdef.h are definition files for C/C++ language; they are necessary to compile and link the library to its own C/C++ code.\n
		-# ftd2xx.dll this library file must always present, the library use it to access at USB 1.1/2.0 port.\n
 */

/*! \page histo AiCAM Version History.
	\verbatim
	1.0.0 27/11/2016 First release!
    2.0.0 20/04/2019 CORE-CAM update
	\endverbatim
*/

/*!	\file aicamdef.h 
	\brief Types, structures and defines used in AiCam libraries.
	\version 2.0.0
	\author Manuele Turini
 */

#define PROC_CALL __stdcall

/*
 *  Use for both Win16 & Win32.
 *  For Win32 compilers supporting the "declspec" keyword.
 */
#if defined(_WIN32) || defined(__WIN32__)
	#if defined(_MSC_VER)
		#ifdef STATIC_LIBRARY
			#define DLL
			#define DLL_P *
			#define DLL_V
		#else
			#ifdef AICAM_SOURCE_CODE
				#define DLL __declspec(dllexport) PROC_CALL
				#define DLL_P __declspec(dllexport) * PROC_CALL
				#define DLL_V __declspec(dllexport)
			#else
				#define DLL __declspec(dllimport) PROC_CALL
				#define DLL_P __declspec(dllimport) * PROC_CALL
				#define DLL_V __declspec(dllimport)
			#endif
		#endif
	#else
		#if defined(__BORLANDC__)
			#ifdef AICAM_SOURCE_CODE
				#define DLL _export PROC_CALL
				#define DLL_P _export * PROC_CALL
				#define DLL_V _export
			#else
				#define DLL _import PROC_CALL
				#define DLL_P _import * PROC_CALL
				#define DLL_V _import
			#endif
		#endif
	#endif
#else
	// UNIX
	#define DLL
	#define DLL_P *
	#define DLL_V
	#define PROC_CALL
#endif

// UNIX/Windows C++ compatibility
#ifndef CTYPET
	#define CTYPET
	#ifdef __cplusplus
		#define CTYPE extern "C"
	#else
		#define CTYPE
	#endif
#endif

/// @defgroup VariableTypes Windows/Unix variable types definitions
/// @{
#ifndef AICAM_VARTYPES
#define AICAM_VARTYPES
typedef	unsigned char	U8;		///< 1 byte without sign, range 0-255
typedef char			S8;		///< 1 byte with sign, range -128+127
typedef char  			STR;	///< 1 byte with sign, range -128+127
typedef short int		S16;	///< 2 byte with sign, -32768+32767
typedef short unsigned	U16;	///< 2 byte without sign, range 0-65535
typedef	unsigned long	U32;	///< 4 byte without sign, range 0-4294967295
typedef long			S32;	///< 4 byte with sign, range +2147483647-2147483647
typedef float			F32;	///< 4 byte floating point
typedef double			F64;	///< 8 byte floating point
#endif
/// @}

typedef F64 MATD;	///< Basic data type

#define AIC_AD_MODE_10BIT	0	///< Linear 10 bit
#define AIC_AD_MODE_12BIT	1	///< 12 bit companded
#define AIC_FLIP_OFF		0	///< Disable axis flip
#define AIC_FLIP_ON			1	///< Enable axis flip
#define AIC_AGC_OFF			0	///< Disable Automatic Gain Controls
#define AIC_AGC_ON			1	///< Enable Automatic Gain Controls
#define AIC_AEC_OFF			0	///< Disable Automatic Exposure Controls
#define AIC_AEC_ON			1	///< Enable Automatic Exposure Controls

#define MAX_SPACE	2		///< Max number of dimensions
#define MAX_ROI 	32		///< Max number of ROI on a matrix2D
#define FULL_IMAGE	MAX_ROI
#define MIN_F64		1.7E-308		// Min floating point
#define MAX_F64		1.7E308		// Max floating point

// File attribute bits for directory entry (FILINFO.fattrib)
#define	AM_RDO	0x01	// Read only
#define	AM_HID	0x02	// Hidden
#define	AM_SYS	0x04	// System
#define AM_DIR	0x10	// Directory
#define AM_ARC	0x20	// Archive

typedef struct FILEINFO {
	U32		fsize;			// File size
	U16		fdate;			// Modified date
	U16		ftime;			// Modified time
	U8		fattrib;		// File attribute
	U8		dummy;			// Only for align!
	U16		fnameSize;		// File name length
	U8		fname[128];		// File name
} FILINFO;

enum ImageType {	// With same DIL code
	IMG_S8 = 0,     //
	IMG_U8 = 1,     // Unsigned 8 bit
	IMG_U16 = 2,    // Unsigned 16 bit
	IMG_S16 = 3,
	IMG_U32 = 4,    // Unsigned 32 bit
	IMG_S32 = 5,
	IMG_U64 = 6,    // Unsigned 64 bit
	IMG_S64 = 7,
	IMG_F32 = 8,    // float
	IMG_F64 = 9,    // double
	IMG_RGB8 = IMG_U8 | 0x8000,
	IMG_RGB16 = IMG_U16 | 0x8000
};

/*! \enum Matrix::STATISTICS_OP
 *	Statistics operation on the ROI matrix2D
 */
enum STATISTICS_OP {
	SMEDIA, 	///< Media of the ROI, see \ref roi
	SSIGMA, 	///< Calculate STD of the ROI, see \ref roi
	SMINMAX,	///< Calculate Min,Max,Media of the ROI, see \ref roi
	SALL,		///< Calculate all statistics, see \ref roi
	SDLINE,		///< D-line statistics, \ref roi
	SENTROPY	///< Calculate Entropy
};

enum PIXEL_WRITE_MODE {
	PWM_SET,
	PWM_OR,
	PWM_AND,
	PWM_XOR
};

enum MatrixTypes_ {
	MT_S8,
	MT_F32,
	MT_F64
};

typedef int MatrixTypes;

// shortcuts
#define S8MAT(a)	((S8 **) a->data)
#define F32MAT(a)	((F32 **) a->data)
#define F64MAT(a)	((F64 **) a->data)

typedef struct matrix2D {
	void	** data;
	U32		xSize;
	U32		ySize;
	U16		matType;
}matrix2D;

typedef struct matrix3D {
	F64		*** data;
	U16		xSize;
	U16		ySize;
}matrix3D;

typedef struct aic_rect {
	int     xs;		// Start
	int     ys;
	int     xd;		// Destination
	int     yd;
	int     pixel;
} aic_rect;

/*	\typedef point
 *	\brief Point coordinates
 */
typedef struct point {
	S32 	x;		///< X
	S32 	y;		///< Y
}point;

/*	\typedef size
 *	\brief Sizes
 */
typedef struct Msize {
	S32 	w;		///< Width
	S32 	h;		///< Height
}Msize;

/*	\typedef roi
 *	\brief Data type for ROI input/output parameters
 *	ROI analyze 
 */
typedef struct roi {
	// Parameters
	point	sta;	///< Start coordinates location
	point	end;	///< End coordinate location
	Msize	dim;	///< Region dimension
	// Outputs
	MATD 	area;	///< Number of pixel
	MATD 	sum;	///< Total value of all pixels
	MATD 	media;	///< Media
	MATD 	adev;	///< Mean absolute deviation
	MATD 	sigma;	///< STD
	MATD 	varia;	///< Variance
	MATD 	curt;	///< Kurtosis
	MATD 	skew;	///< Skew
	MATD 	min;	///< Minimum value of pixel
	MATD 	max;	///< Maximum value of pixel
	MATD 	ctrst;	///< Contrast value
	MATD 	range;	///< Range of values
	MATD	entropy;///< Entropy sum
	MATD	maxent; ///< Maximum entropy
	MATD	ntokens;///< Number of tokens
	MATD	eratio; ///< Ratio of entropy to maximum entropy
	MATD 	lmin[MAX_SPACE];///< Location of minimum value [0] = x, [1] = y
	MATD 	lmax[MAX_SPACE];///< Location of maximum value [0] = x, [1] = y
}roi;

typedef struct SensorProperties {
	// Sensor intrinsics
	int			sensor;				///< Sensor identifier
	int			sensorRev;			///< Sensor revision
	unsigned	hor;				///< Number of horizontal pixel
	unsigned	ver;				///< Number of vertical pixel
	double		horPixelSize;		///< Horizontal dimension of the pixel as um
	double		verPixelSize;		///< vertical dimension of the pixel as um
	// Sensor and peripherals set
	int			adc;
	int			xFlip;
	int			yFlip;
	int			aec;
	int			agc;
	int			gain;
	int			led;
	int			ledSrc;
}SensorProperties;

typedef struct ImageProperties {
	unsigned	hOff;				///< Horizontal offset
	unsigned	vOff;				///< Vertical offset
	unsigned	hor;				///< Number of horizontal pixel
	unsigned	ver;				///< Number of vertical pixel
	unsigned	hbin;				///< Horizontal binning
	unsigned	vbin;				///< Vertical binning
	unsigned	dataSize;			///< Pixel data size: 1,2,... byte(s)
	unsigned	pixels;				///< Number of pixels on each frames
	unsigned	type;				///< Data type see ImageType
	unsigned	frames;				///< number of frames
	unsigned	nibble;				///< Set which nibble will transferred on 8bit transfer from 16 sampling. 0: Low, 1: High
}ImageProperties;

typedef struct AiCamHardware {
	int					platform;	///< hardware platform id
	SensorProperties	sensor;
	int					diskMount;
}AiCamHardware;

typedef struct AiCamHandle {
	void				* driver;
	void				* driverInfo;
	AiCamHardware		camera;
	ImageProperties		imgCamSet;		///< Image properties for the camera
}AiCamHandle;

typedef struct ImageHandle {
	void			* data;			///< Data pointer
	ImageProperties	imgSet;			///< Image properties
}ImageHandle;

typedef AiCamHandle *	AiCam;		///< Basic pointer to the AiCam handler

// call-back prototype
typedef void (*AiCamProgress)(const char * title, int value);

/// @defgroup hwPlatform Camera platform definitions
/// @{

#define CORE_CAM	666		///< First AI developed camera, monochrome MT9V032

/// @}

/// @defgroup sensors CCD/CMOS code definitions

	/// @defgroup aptina Aptina sensors
	/// @ingroup sensors
	/// @{
	#define MT9V032		1000	///< 752 x 480    6 x 6    um

	/// @}

/// @}

enum ConversionMode { AIC_16_TO_8 };

#endif