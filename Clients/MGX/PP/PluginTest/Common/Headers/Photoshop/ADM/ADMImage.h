/*
 * Name:
 *	ADMImage.h
 *
 * Copyright 1986-1998 Adobe Systems Incorporated.
 * All Rights Reserved.
 *
 * Purpose:
 *	ADM Image Suite.
 *
 * Distribution:
 *	PUBLIC
 *
 * Version history:
 *	1.0.1 11/20/1996	DL	Second version.
 *		Created by Dave Lazarony.
 */

#ifndef __ADMImage__
#define __ADMImage__

/*
 * Includes
 */
 
#ifndef __ADMTypes__
#include "ADMTypes.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma PRAGMA_ALIGN_BEGIN
#pragma PRAGMA_IMPORT_BEGIN

// Problematic Macintosh Headers #define GetItem.
#ifdef GetItem
#undef GetItem
#endif

/*
 * Constants
 */

#define kADMImageSuite				"ADM Image Suite"

#define kADMImageSuiteVersion1		1
#define kADMImageSuiteVersion		kADMImageSuiteVersion1


/*
 *  ADMImage Suite
 */
typedef struct ADMImageSuite1
{
	ADMImageRef ASAPI (*Create)(int width, int height);
	void ASAPI (*Destroy)(ADMImageRef image);

	int ASAPI (*GetWidth)(ADMImageRef image);
	int ASAPI (*GetHeight)(ADMImageRef image);
	int ASAPI (*GetByteWidth)(ADMImageRef image);
	int ASAPI (*GetBitsPerPixel)(ADMImageRef image);
	
	ASBytePtr ASAPI (*BeginBaseAddressAccess)(ADMImageRef image);
	void ASAPI (*EndBaseAddressAccess)(ADMImageRef image);
	
	ADMDrawerRef ASAPI (*BeginADMDrawer)(ADMImageRef image);
	void ASAPI (*EndADMDrawer)(ADMImageRef image);
	
	void ASAPI (*BeginAGMImageAccess)(ADMImageRef image, struct _t_AGMImageRecord *imageRecord);
	void ASAPI (*EndAGMImageAccess)(ADMImageRef image);
	
	ASErr ASAPI (*GetPixel)(ADMImageRef image, ASPoint *point, ASRGBColor *color);
	ASErr ASAPI (*SetPixel)(ADMImageRef image, ASPoint *point, ASRGBColor *color);
	
	ADMImageRef ASAPI (*CreateBitmap)(int width, int height);
	ADMImageRef ASAPI (*CreateOffscreen)(int width, int height);
	
} ADMImageSuite1;

typedef ADMImageSuite1 ADMImageSuite;	// The current version


#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif


#endif








