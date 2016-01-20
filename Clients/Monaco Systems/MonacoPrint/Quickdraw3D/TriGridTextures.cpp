// routines to allow us to put texture maps on a parameterized group
//
//	03/23/95	rdd		added AddResourceTextureToGroup

#include "QD3D.h"
#include "QD3DGroup.h"
#include "QD3DShader.h"

#include <QuickDraw.h>
#include <Resources.h>
#include "PictRead.h"		// this is a library file from QD3D applications folder
#include "Textures2.h"

TQ3Status AddTextureToGroup( TQ3GroupObject	theGroup, TQ3StoragePixmap *textureImage)
{
	TQ3TextureObject	textureObject;
	TQ3GroupPosition position;
	TQ3Object		firstObject;
	
	textureObject = Q3PixmapTexture_New(textureImage);
	
	if( textureObject ) {
		if( Q3Object_IsType(theGroup, kQ3GroupTypeDisplay) == kQ3True) {
			Q3Group_GetFirstPosition(theGroup, &position);
	
			Q3Group_GetPositionObject(theGroup, position,	&firstObject);
	
			if( Q3Object_IsType(firstObject, kQ3SurfaceShaderTypeTexture) == kQ3True) {
				TQ3TextureObject	oldTextureObject;
				TQ3StoragePixmap oldTextureImage;
						
				Q3TextureShader_GetTexture(firstObject, &oldTextureObject);
				Q3PixmapTexture_GetPixmap(oldTextureObject, &oldTextureImage);
				
				Q3Object_Dispose(oldTextureObject);
				Q3TextureShader_SetTexture(firstObject, textureObject);
				Q3Object_Dispose(textureObject);
			} else {
				TQ3ShaderObject textureShader;
				
				textureShader = Q3TextureShader_New(textureObject);
				Q3Object_Dispose(textureObject);
				//Q3Group_SetPositionObject(theGroup, position, theDocument->textureShader);
				Q3Group_AddObjectBefore(theGroup, position, textureShader);
				Q3Object_Dispose(textureShader);
			}
			
			Q3Object_Dispose(firstObject);
		} else if( Q3Object_IsType(theGroup, kQ3DisplayGroupTypeOrdered) == kQ3True) {
			TQ3ShaderObject textureShader;
			
			Q3Group_GetFirstPositionOfType(
				theGroup,
				kQ3ShapeTypeShader, &position);	
			
			if( position ) {
				Q3Group_GetPositionObject(theGroup, position,	&firstObject);
	
				if( Q3Object_IsType(firstObject, kQ3SurfaceShaderTypeTexture) == kQ3True) {
					TQ3TextureObject	oldTextureObject;
					TQ3StoragePixmap oldTextureImage;
					
					Q3TextureShader_GetTexture(firstObject, &oldTextureObject);
					Q3PixmapTexture_GetPixmap(oldTextureObject, &oldTextureImage);
					
					Q3Object_Dispose(oldTextureObject);
					Q3TextureShader_SetTexture(firstObject, textureObject);
					Q3Object_Dispose(textureObject);
				} else {
					textureShader = Q3TextureShader_New(textureObject);
					if( textureShader ) {
						Q3Object_Dispose(textureObject);
						Q3Group_SetPositionObject(theGroup, position, textureShader);
						Q3Object_Dispose(textureShader);
					} else {
						return(kQ3Failure);
					}
				}
			} else {
				textureShader = Q3TextureShader_New(textureObject);
				if( textureShader ) {
					Q3Object_Dispose(textureObject);
					Q3Group_AddObject(theGroup, textureShader);
					Q3Object_Dispose(textureShader);
				} else {
					return(kQ3Failure);
				}
			}
			
		}
	return(kQ3Success);
	} else {
		return(kQ3Failure);
	}
}


void AddResourceTextureToGroup( short pictResID, TQ3GroupObject	theGroup )
{
	TQ3StoragePixmap 	bMap;
	PicHandle 			thePicture = nil;

	if (theGroup == NULL)
		return;

	if((thePicture = (PicHandle) GetResource('PICT', pictResID)) != NULL ) {

		HLock((Handle) thePicture);
		LoadMapPICT( thePicture, 
					 0L,
					 (unsigned long)((**thePicture).picFrame.right - (**thePicture).picFrame.left),
					 (unsigned long)((**thePicture).picFrame.bottom - (**thePicture).picFrame.top),
					 &bMap)	;

		if( bMap.image != NULL )
			AddTextureToGroup( theGroup, &bMap ) ;

		HUnlock((Handle) thePicture);
		ReleaseResource((Handle) thePicture);
	}
}
