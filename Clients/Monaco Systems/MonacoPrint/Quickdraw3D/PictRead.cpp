/******************************************************************************
 **																			 **
 ** 	Module:		PictRead.c												 **
 ** 																		 **
 ** 																		 **
 ** 	Purpose: 									 						 **
 ** 																		 **
 ** 																		 **
 ** 																		 **
 ** 	Copyright (C) 1992-1995 Apple Computer, Inc.  All rights reserved.	 **
 ** 																		 **
 ** 																		 **
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <StandardFile.h>
#include <Memory.h>

#include <QDOffscreen.h>
#include "QD3D.h"
#include "QD3DStorage.h"
#include "PictRead.h"


/*===========================================================================*\
 *
 *	Routine:	OpenPICTFile()
 *
 *	Comments:	open a PICT file and read it into a PICT handle
 *
\*===========================================================================*/

PicHandle OpenPICTFile(
	short 	vRefNum, 
	Str255 	fName)
{
	short		fRefNum;
	OSErr		err;
	long		curEOF;
	PicHandle	myPic;
	long 		count;
	Ptr 		buffer;
	
	/* open PICT file */
	err = FSOpen(fName, vRefNum, &fRefNum);
	if (err != 0) {
		/*printf("Error - cannot open file\n"); */
		exit(-1);
	}
	
	/* get size of file */
	err = GetEOF(fRefNum, &curEOF);
	if (err != 0) {
		/*printf("Error - cannot get EOF\n"); */
		exit(-2);
	}
	
	/* move the file mark to 512 */
	err = SetFPos(fRefNum, SEEK_SET, 512L);
	if (err != 0) {
		/*printf("Error - cannot seek 512\n"); */
		exit(-3);
	}

	/* size of data to read */
	count = curEOF - 512;
	
	/* create the PicHandle */
	myPic = (PicHandle)NewHandle(count);
	HLock((Handle)myPic);
	
	/* read the PICT info */
	buffer = (Ptr)(*myPic);
	err = FSRead(fRefNum, &count, buffer);
	if (err != 0) {
		/*printf("Error - cannot read\n");*/
		exit(-4);
	}
	HUnlock((Handle)myPic);
	
	/* release the file */
	err = FSClose(fRefNum);
	if (err != 0) {
		/*printf("Error - cannot close file \n"); */
		exit(-5);
	}
	
	return (myPic);
}


/*===========================================================================*\
 *
 *	Routine:	GetPICTFile()
 *
 *	Comments:	Query user for PICT File
 *
\*===========================================================================*/

PicHandle GetPICTFile (
	void)
{
	SFReply			reply;
	static	Point	where = { 80, 80 };
	PicHandle 		picHandle;
	
	SFGetFile(where, NULL, NULL, -1, NULL, NULL, &reply);
	
	if (reply.good) {
		picHandle = OpenPICTFile(reply.vRefNum, reply.fName);
		return (picHandle);
	}
	
	return (0);
}


/*===========================================================================*\
 *
 *	Routine:	LoadMapPICT()
 *
 *	Comments:	take a PICT handle and loads it into a bitmap structure
 *
\*===========================================================================*/

short LoadMapPICT(
	PicHandle 			pict,
	unsigned long 		mapID,
	unsigned long 		mapSizeX,
	unsigned long 		mapSizeY,
	TQ3StoragePixmap 	*bMap)
{
	unsigned long 			*textureMap;
	unsigned long			*textureMapAddr;
	unsigned long 			*pictMap;
	unsigned long			pictMapAddr;
	register unsigned long 	row;
	register unsigned long 	col;
	Rect 					rectGW;
	GWorldPtr 				pGWorld;
	PixMapHandle 			hPixMap;
	unsigned long 			pictRowBytes;
	QDErr					err;
	GDHandle				oldGD;
	GWorldPtr				oldGW;
	short					success;
	
	mapID;		/* unused argument */
	
	/* save current port */
	GetGWorld(&oldGW, &oldGD);

	/* create the GWorld */
	SetRect(&rectGW, 0, 0, (unsigned short)mapSizeX, (unsigned short)mapSizeY);

	err = NewGWorld(&pGWorld, 32, &rectGW, 0, 0, useTempMem);
	if (err != noErr)
		return 0;

	success = 1;
	
	hPixMap = GetGWorldPixMap(pGWorld);
	pictMapAddr = (unsigned long)GetPixBaseAddr (hPixMap);
	pictRowBytes = (unsigned long)(**hPixMap).rowBytes & 0x3fff;
	
	/* put the PICT into the window */
	SetGWorld(pGWorld, nil);
	
	LockPixels(hPixMap);
	EraseRect(&rectGW);
	DrawPicture(pict, &rectGW);
		
	/* allocate an area of memory for the texture */
	textureMap = (unsigned long *)malloc(mapSizeX * mapSizeY * sizeof(unsigned long));
	if (textureMap == NULL) {
		success = 0;
		goto bail;
	}
	/* bMap->image = (char *)textureMap; */

	/* copy the PICT into the texture */
	textureMapAddr = textureMap;
	for (row = 0L; row < mapSizeY; row++) {
		pictMap = (unsigned long *)(pictMapAddr + (pictRowBytes * row));
		for (col = 0L; col < mapSizeX; col++) {
			*textureMap++ = (*pictMap++ | 0xff000000L);
		}
	}
		
	bMap->image = Q3MemoryStorage_New((const unsigned char *)textureMapAddr, 
								  mapSizeX * mapSizeY * sizeof(unsigned long));
								  
	if (bMap->image == NULL) {
		/* error */
		success = 0;
		goto bail;
	}

	UnlockPixels(hPixMap);
	
	bMap->width 	= mapSizeX;
	bMap->height	= mapSizeY;
	bMap->rowBytes 	= bMap->width * 4;
	bMap->pixelSize = 32;
	bMap->pixelType	= kQ3PixelTypeRGB32;
	bMap->bitOrder	= kQ3EndianBig;
	bMap->byteOrder	= kQ3EndianBig;
	
	/* Free junk */
bail:

	SetGWorld(oldGW, oldGD);
	
	DisposeGWorld(pGWorld);
	if (textureMapAddr != NULL)
		free(textureMapAddr);
	
	return success;
}
