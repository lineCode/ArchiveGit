#pragma once
/******************************************************************************
 **																			 **
 ** 	Module:		PictRead.h												 **
 ** 																		 **
 ** 																		 **
 ** 																		 **
 ** 	Purpose: 	protos for PICT to TEX routines							 **
 ** 																		 **
 ** 																		 **
 ** 																		 **
 ** 	Copyright (C) 1992-1995 Apple Computer, Inc.  All rights reserved.	 **
 ** 																		 **
 ** 																		 **
 *****************************************************************************/
#ifndef PictRead_h
#define PictRead_h

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */


PicHandle OpenPICTFile( 
	short 	vRefNum, 
	Str255 	fName);

PicHandle GetPICTFile( 
	void);
	
short LoadMapPICT(
	PicHandle			pict,
	unsigned long 		mapId,
	unsigned long 		mapSizeX, 
	unsigned long 		mapSizeY, 
	TQ3StoragePixmap 	*bMap);
	
	
#ifdef __cplusplus
}
#endif	/* __cplusplus */

	
#endif  /*  PictRead_h  */