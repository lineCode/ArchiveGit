// Quickdraw 3D sample code
//
// This file illustrates how to set up a pixmap based draw context.
// A metafile is read into and imaged in the pixmap, when the window is
// closed the pixmap is used to generate a PICT of the scene.
//
// Nick Thompson, AppleLink: DEVSUPPORT (devsupport@applelink.apple.com)
//
// �1994-5 Apple Computer Inc., All Rights Reserved


#include <Files.h>
#include <QuickDraw.h>
#include <QDOffScreen.h>
#include <StandardFile.h>

#include "gammutwinSupp.h"

#include "QD3D.h"
#include "QD3DDrawContext.h"
#include "QD3DRenderer.h"
#include "QD3DShader.h"
#include "QD3DCamera.h"
#include "QD3DLight.h"
#include "QD3DGeometry.h"
#include "QD3DGroup.h"
#include "QD3DMath.h"
#include "QD3DTransform.h"
#include "QD3DStorage.h"
#include "QD3DIO.h"
#include "QD3DAcceleration.h"


//-----------------------------------------------------------------------------------------------
// local utility functions

static void GetGroupBBox(
	DocumentPtr			theDocument,
	TQ3BoundingBox 		*viewBBox) ;
												
static	TQ3Status MyAddShaderToGroup( TQ3GroupObject group ) ;

static TQ3Status GetDocumentGroupBoundingBox( 
	DocumentPtr theDocument , 
	TQ3BoundingBox *viewBBox) ;

//-----------------------------------------------------------------------------------------------
// Submit the scene for rendering/fileIO and picking
TQ3Status SubmitScene( DocumentPtr theDocument ) 
{		
	TQ3Vector3D				globalScale;
	TQ3Vector3D				globalTranslate;
	
	globalScale.x = globalScale.y = globalScale.z = theDocument->fGroupScale;
	globalTranslate = *(TQ3Vector3D *)&theDocument->fGroupCenter;
	Q3Vector3D_Scale(&globalTranslate, -1, &globalTranslate);
	Q3Style_Submit(theDocument->fInterpolation, theDocument->fView);
	Q3Style_Submit(theDocument->fBackFacing , theDocument->fView);
	Q3Style_Submit(theDocument->fFillStyle, theDocument->fView);
		
		
	Q3MatrixTransform_Submit( &theDocument->fRotation, theDocument->fView);
		
	Q3ScaleTransform_Submit(&globalScale, theDocument->fView);
	Q3TranslateTransform_Submit(&globalTranslate, theDocument->fView);
		
	Q3DisplayGroup_Submit( theDocument->fModel, theDocument->fView);
		
	return kQ3Success ;
}

//-----------------------------------------------------------------------------------------------

static TQ3Status GetDocumentGroupBoundingBox( 
	DocumentPtr theDocument , 
	TQ3BoundingBox *viewBBox)
{
	TQ3Status		status;
	TQ3ViewStatus	viewStatus ;
	
	status = Q3View_StartBoundingBox( theDocument->fView, kQ3ComputeBoundsApproximate );
	do {
		status = SubmitScene( theDocument ) ;
	} while((viewStatus = Q3View_EndBoundingBox( theDocument->fView, viewBBox )) == kQ3ViewStatusRetraverse );
	return status ;
}
//-----------------------------------------------------------------------------------------------

TQ3ViewObject MyNewView(WindowPtr dp,Rect *winrect,float ambient, float point, float fill,TQ3Point3D *from, TQ3Point3D *to)
{
	TQ3Status				myStatus;
	TQ3ViewObject			myView;
	TQ3DrawContextObject		myDrawContext;
	TQ3RendererObject		myRenderer;
	TQ3CameraObject			myCamera;
	TQ3GroupObject			myLights;
	
	if((myView = Q3View_New()) == NULL)
		goto bail ;	
		
	//	Create and set draw context.
	if ((myDrawContext = MyNewDrawContext(dp,winrect)) == NULL )
		goto bail;
		
	if ((myStatus = Q3View_SetDrawContext(myView, myDrawContext)) == kQ3Failure )
		goto bail;

	Q3Object_Dispose( myDrawContext ) ;
	
	//	Create and set renderer.
	// this would use the interactive software renderer

	if ((myRenderer = Q3Renderer_NewFromType(kQ3RendererTypeInteractive)) != NULL ) {
		if ((myStatus = Q3View_SetRenderer(myView, myRenderer)) == kQ3Failure ) {
			goto bail;
		}
		
		// these two lines set us up to use the best possible renderer,
		// including  hardware if it is installed.
		Q3InteractiveRenderer_SetDoubleBufferBypass(myRenderer, kQ3True);						
		Q3InteractiveRenderer_SetPreferences(myRenderer, kQAVendor_BestChoice, 0);

	}
	else {
		goto bail;
	}

	Q3Object_Dispose( myRenderer ) ;
	
	//	Create and set camera.
	if ( (myCamera = MyNewCamera(winrect,from,to)) == NULL )
		goto bail;
		
	if ((myStatus = Q3View_SetCamera(myView, myCamera)) == kQ3Failure )
		goto bail;

	Q3Object_Dispose( myCamera ) ;
	
	//	Create and set lights.
	if ((myLights = MyNewLights(ambient, point, fill)) == NULL )
		goto bail;
		
	if ((myStatus = Q3View_SetLightGroup(myView, myLights)) == kQ3Failure )
		goto bail;
		
	Q3Object_Dispose(myLights);

	//	Done!!!
	return ( myView );
	
bail:
	//	If any of the above failed, then don't return a view.
	SysBeep(10) ;
	return ( NULL );
}

//----------------------------------------------------------------------------------
/*
TQ3DrawContextObject MyNewDrawContext(GWorldPtr theGWorld)
{
	TQ3PixmapDrawContextData	myDrawContextData;
	TQ3ColorARGB				clearColor ;
	PixMapHandle 				hPixMap ;
	Rect						srcRect ;
	
	//	Set the background color.
	clearColor.a = 1.0;
	clearColor.r = 0.0;
	clearColor.g = 0.0;
	clearColor.b = 0.0;
	
	//	Fill in draw context data.
			
	myDrawContextData.drawContextData.clearImageMethod = kQ3ClearMethodWithColor;
	myDrawContextData.drawContextData.clearImageColor  = clearColor;

	myDrawContextData.drawContextData.paneState = kQ3False;
	myDrawContextData.drawContextData.maskState = kQ3False;
	
	myDrawContextData.drawContextData.doubleBufferState = kQ3True;

	hPixMap = GetGWorldPixMap(theGWorld);
	LockPixels(hPixMap);

	srcRect = theGWorld->portRect;

	myDrawContextData.pixmap.width = srcRect.right  - srcRect.left;
	myDrawContextData.pixmap.height= srcRect.bottom - srcRect.top;
	
	myDrawContextData.pixmap.rowBytes = (**hPixMap).rowBytes & 0x7FFF;
	myDrawContextData.pixmap.pixelType = kQ3PixelTypeRGB32;
	myDrawContextData.pixmap.pixelSize = 32;

	myDrawContextData.pixmap.bitOrder  = kQ3EndianBig;
	myDrawContextData.pixmap.byteOrder = kQ3EndianBig;
	
	myDrawContextData.pixmap.image		= GetPixBaseAddr( hPixMap );
	
	return Q3PixmapDrawContext_New(&myDrawContextData);
} */


TQ3DrawContextObject MyNewDrawContext(WindowPtr theWindow,Rect *winrect)
{
	TQ3DrawContextData		myDrawContextData;
	TQ3MacDrawContextData	myMacDrawContextData;
	TQ3ColorARGB			ClearColor;
	TQ3DrawContextObject	myDrawContext ;
	TQ3Area					pane;
	
	// set the pane
	pane.min.x = winrect->left;
	pane.min.y = winrect->top;
	pane.max.x = winrect->right;
	pane.max.y = winrect->bottom;
	
	//	Set the background color.
	ClearColor.a = 1.0;
	ClearColor.r = 0.0;
	ClearColor.g = 0.0;
	ClearColor.b = 0.0;
	
	//	Fill in draw context data.
	myDrawContextData.clearImageMethod = kQ3ClearMethodWithColor;
	myDrawContextData.clearImageColor = ClearColor;
	myDrawContextData.paneState = kQ3True;
	myDrawContextData.pane = pane;
	myDrawContextData.maskState = kQ3False;
	myDrawContextData.doubleBufferState = kQ3True;
 
	myMacDrawContextData.drawContextData = myDrawContextData;
	
	myMacDrawContextData.window =(CGrafPtr) theWindow;		// this is the window associated with the view
	myMacDrawContextData.library = kQ3Mac2DLibraryNone;
	myMacDrawContextData.viewPort = nil;
	myMacDrawContextData.grafPort = nil;
	
	//	Create draw context and return it, if it�s nil the caller must handle
	myDrawContext = Q3MacDrawContext_New(&myMacDrawContextData);

	return myDrawContext ;
}
//----------------------------------------------------------------------------------

TQ3CameraObject MyNewCamera(Rect *winRect,TQ3Point3D *cameraFrom,TQ3Point3D *cameraTo)
{
	TQ3CameraObject					myCamera;
	TQ3CameraData					myCameraData;
	TQ3ViewAngleAspectCameraData		myViewAngleCameraData;
//	TQ3Point3D						cameraFrom 	= { 0.0, 0.0, 10.0 };
//	TQ3Point3D						cameraTo 	= { 0.0, 0.0, 0.0 };
	TQ3Vector3D						cameraUp 	= { 0.0, 1.0, 0.0 };
	
	float 							fieldOfView = .52359333333;
	float 							hither 		= 0.001;
	float 							yon 		= 1000;
	
	//	Fill in camera data.
	myCameraData.placement.cameraLocation = *cameraFrom;
	myCameraData.placement.pointOfInterest = *cameraTo;
	myCameraData.placement.upVector = cameraUp;
	
	myCameraData.range.hither = hither;
	myCameraData.range.yon = yon;
	
	myCameraData.viewPort.origin.x = -1.0;
	myCameraData.viewPort.origin.y = 1.0;
	myCameraData.viewPort.width = 2.0;
	myCameraData.viewPort.height = 2.0;
	
	myViewAngleCameraData.cameraData = myCameraData;
	myViewAngleCameraData.fov = fieldOfView ;
	
	// set up the aspect ratio based on the window
	myViewAngleCameraData.aspectRatioXToY =  
			(float) (winRect->right - winRect->left) / 
			(float) (winRect->bottom - winRect->top);

	myCamera = Q3ViewAngleAspectCamera_New(&myViewAngleCameraData);	
	
	//	Return the camera.
	return ( myCamera );
}


//----------------------------------------------------------------------------------

TQ3GroupObject MyNewLights(float ambient, float point, float fill)
{
	TQ3GroupPosition			myGroupPosition;
	TQ3GroupObject			myLightList;
	TQ3LightData				myLightData;
	TQ3PointLightData		myPointLightData;
	TQ3DirectionalLightData	myDirectionalLightData;
	TQ3LightObject			myAmbientLight, myPointLight, myFillLight;
	TQ3Point3D				pointLocation = { -10.0, 0.0, 10.0 };
	TQ3Vector3D				fillDirection = { 10.0, 0.0, 10.0 };
	TQ3ColorRGB				WhiteLight = { 1.0, 1.0, 1.0 };
	
	//	Set up light data for ambient light.  This light data will be used for point and fill
	//	light also.

	myLightData.isOn = kQ3True;
	myLightData.color = WhiteLight;
	
	//	Create ambient light.
	myLightData.brightness = ambient;
	myAmbientLight = Q3AmbientLight_New(&myLightData);
	if ( myAmbientLight == NULL )
		goto bail;
	
	//	Create point light.
	myLightData.brightness = point;
	myPointLightData.lightData = myLightData;
	myPointLightData.castsShadows = kQ3False;
	myPointLightData.attenuation = kQ3AttenuationTypeNone;
	myPointLightData.location = pointLocation;
	myPointLight = Q3PointLight_New(&myPointLightData);
	if ( myPointLight == NULL )
		goto bail;

	//	Create fill light.
	myLightData.brightness = fill;
	myDirectionalLightData.lightData = myLightData;
	myDirectionalLightData.castsShadows = kQ3False;
	myDirectionalLightData.direction = fillDirection;
	myFillLight = Q3DirectionalLight_New(&myDirectionalLightData);
	if ( myFillLight == NULL )
		goto bail;

	//	Create light group and add each of the lights into the group.
	myLightList = Q3LightGroup_New();
	if ( myLightList == NULL )
		goto bail;
	myGroupPosition = Q3Group_AddObject(myLightList, myAmbientLight);
	if ( myGroupPosition == 0 )
		goto bail;
	myGroupPosition = Q3Group_AddObject(myLightList, myPointLight);
	if ( myGroupPosition == 0 )
		goto bail;
	myGroupPosition = Q3Group_AddObject(myLightList, myFillLight);
	if ( myGroupPosition == 0 )
		goto bail;

	Q3Object_Dispose( myAmbientLight ) ;
	Q3Object_Dispose( myPointLight ) ;
	Q3Object_Dispose( myFillLight ) ;

	//	Done!
	return ( myLightList );
	
bail:
	//	If any of the above failed, then return nothing!
	return ( NULL );
}



//----------------------------------------------------------------------------------
// attach a shader to the group

TQ3Status MyAddShaderToGroup( TQ3GroupObject group )
{
	TQ3ShaderObject	illuminationShader = Q3PhongIllumination_New();
	//TQ3ShaderObject	illuminationShader = Q3NULLIllumination_New();

	Q3Group_AddObject(group, illuminationShader);
	Q3Object_Dispose(illuminationShader);
	return(kQ3Success);
}


//----------------------------------------------------------------------------------


void GetGroupBBox(
	DocumentPtr			theDocument,
	TQ3BoundingBox 		*viewBBox)
{
	TQ3Point3D 					from 	= { 0.0, 0.0, 1.0 };
	TQ3Point3D 					to 		= { 0.0, 0.0, 0.0 };
	TQ3Vector3D 					up 		= { 0.0, 1.0, 0.0 };
	
	float 						fieldOfView = .52359333333;
	float 						hither 		=  0.5;
	float 						yon 		=  1.5;
	TQ3GroupObject				mainGroup = theDocument->fModel ;

	TQ3Status					status;
	
#ifdef BETA_1_BUILD
	Q3View_StartBounds( theDocument->fView );

	status = Q3DisplayGroup_BoundingBox(mainGroup, 
										viewBBox, 
										kQ3ComputeBoundsApproximate,
								 	    viewObject);

	Q3View_EndBounds( theDocument->fView );
#else
	status = GetDocumentGroupBoundingBox( theDocument , viewBBox) ;
#endif
								        
	//
	//  If we have a point model, then the "viewBBox" would end up
	//  being a "singularity" at the location of the point.  As
	//  this bounding "box" is used in setting up the camera spec,
	//  we get bogus input into Escher.
	
	{
 		float		xSize, ySize, zSize;
		
		xSize = viewBBox->max.x - viewBBox->min.x;
		ySize = viewBBox->max.y - viewBBox->min.y;
		zSize = viewBBox->max.z - viewBBox->min.z;

		if (xSize <= kQ3RealZero &&
		    ySize <= kQ3RealZero &&
			zSize <= kQ3RealZero) {
			
			viewBBox->max.x += 0.0001;
			viewBBox->max.y += 0.0001;
			viewBBox->max.z += 0.0001;
			
			viewBBox->min.x -= 0.0001;
			viewBBox->min.y -= 0.0001;
			viewBBox->min.z -= 0.0001;
		}
	}
}




//------------------------------------------------------------------------
// set the viewing area so that the object is never clipped

TQ3Point3D AdjustCamera(
	DocumentPtr			theDocument,
	short				winWidth,
	short				winHeight)
{
	float 						fieldOfView;
	float 						hither;
	float 						yon;
	TQ3CameraPlacement			placement;
	TQ3CameraRange				range;
	TQ3BoundingBox 				viewBBox;
	long 						fromAxis;	
	float 						maxDimension;
 	float						xSize, ySize, zSize;
	float						weights[2] = { 0.5, 0.5 };
	TQ3Point3D					points[2];
	TQ3Vector3D				 	viewVector;
	TQ3Vector3D					normViewVector;
	TQ3Vector3D					eyeToFrontClip;
	TQ3Vector3D					eyeToBackClip;
	float						viewDistance;
	TQ3Vector3D					diagonalVector;
	float						ratio;
	TQ3CameraObject				camera;
	
	TQ3ViewObject				theView = theDocument->fView ;
	TQ3GroupObject				mainGroup = theDocument->fModel ;
	
	TQ3Point3D					*documentGroupCenter = &theDocument->fGroupCenter ;
	float						*documentGroupScale  = &theDocument->fGroupScale ;

	Q3View_GetCamera( theView, &camera);
	GetGroupBBox( theDocument, &viewBBox);

	/*
	 *  If we have a point model, then the "viewBBox" would end up
	 *  being a "singularity" at the location of the point.  As
	 *  this bounding "box" is used in setting up the camera spec,
	 *  we get bogus input into Escher.
	 */
	xSize = viewBBox.max.x - viewBBox.min.x;
	ySize = viewBBox.max.y - viewBBox.min.y;
	zSize = viewBBox.max.z - viewBBox.min.z;

	if (xSize <= kQ3RealZero &&
	    ySize <= kQ3RealZero &&
		zSize <= kQ3RealZero)  {
		viewBBox.max.x += 0.0001;
		viewBBox.max.y += 0.0001;
		viewBBox.max.z += 0.0001;
		
		viewBBox.min.x -= 0.0001;
		viewBBox.min.y -= 0.0001;
		viewBBox.min.z -= 0.0001;
	}

	points[0] = viewBBox.min;
	points[1] = viewBBox.max;

	Q3Point3D_AffineComb(points, weights, 2, documentGroupCenter);

	/*
	 *  The "from" point is on a vector perpendicular to the plane
	 *  in which the bounding box has greatest dimension.  As "up" is
	 *  always in the positive y direction, look at x and z directions.
	 */
	xSize = viewBBox.max.x - viewBBox.min.x;
	zSize = viewBBox.max.z - viewBBox.min.z;
	
	if (xSize > zSize) {
		fromAxis = kQ3AxisZ;
	} else {
		fromAxis = kQ3AxisX;
	}

	/*
	 *  Compute the length of the diagonal of the bounding box.
	 *
	 *  The hither and yon planes are adjusted so that the
 	 *  diagonal of the bounding box is 7/8 the size of the
 	 *  minimum dimension of the view frustum. The diagonal is used instead
 	 *  of the maximum size (in x, y, or z) so that when you rotate
 	 *  the object, the corners don't get clipped out.
 	 */
	Q3Point3D_Subtract(
		&viewBBox.max,
		&viewBBox.min,
		&diagonalVector);

	maxDimension	=	Q3Vector3D_Length(&diagonalVector);
	maxDimension	*=	8.0 / 7.0;
	
	ratio = 1.0 / maxDimension;
			
	*documentGroupScale = ratio;
	
	Q3Camera_GetPlacement(camera, &placement);

	Q3Point3D_Subtract(
		&placement.cameraLocation,
		&placement.pointOfInterest,
		&viewVector);
		
	viewDistance = Q3Vector3D_Length(&viewVector);
	
	Q3Vector3D_Normalize(&viewVector, &normViewVector);
	
	Q3Vector3D_Scale(&normViewVector, 
					 viewDistance - ratio * maxDimension/2.0,
					 &eyeToFrontClip);
					
	Q3Vector3D_Scale(&normViewVector, 
					viewDistance + ratio * maxDimension/2.0,
					&eyeToBackClip);

	hither 	= Q3Vector3D_Length(&eyeToFrontClip);
	yon 	= Q3Vector3D_Length(&eyeToBackClip);
	
	fieldOfView = 2 * atan((ratio * maxDimension/2.0)/hither);

	range.hither 				= hither;
	range.yon 					= yon;

	Q3Camera_SetRange(camera, &range);

	Q3ViewAngleAspectCamera_SetFOV(
		camera, fieldOfView);

	Q3ViewAngleAspectCamera_SetAspectRatio(
		camera, (float) winWidth / (float) winHeight);

	Q3Object_Dispose(camera);
	
	return( *documentGroupCenter );
}


//----------------------------------------------------------------------------------
TQ3GroupObject MyNewModel(void)
{
	TQ3GroupObject			myGroup = NULL;
	TQ3ShaderObject			myIlluminationShader;

	// Create a group for the complete model.
	// do not use Q3OrderedDisplayGroup_New since in this
	// type of group all of the translations are applied before
	// the objects in the group are drawn, in this instance we 
	// dont want this.
	if ((myGroup = Q3DisplayGroup_New()) != NULL ) {
			
		// Define a shading type for the group
		// and add the shader to the group
		myIlluminationShader = Q3PhongIllumination_New();
		//myIlluminationShader = Q3NULLIllumination_New();
		//myIlluminationShader = Q3LambertIllumination_New();
		Q3Group_AddObject(myGroup, myIlluminationShader);
	}
	
	// dispose of the objects we created here
	if( myIlluminationShader ) 
		Q3Object_Dispose(myIlluminationShader);	

	//	Done!
	return ( myGroup );
}

//-------------------------------------------------------------------------------------------
//
Boolean MetafileFileSpecify( FSSpec *theFile )
{
	StandardFileReply	theSFReply ;
	SFTypeList			myTypes = { '3DMF' } ;
	const short			numTypes = 1 ;
		
	// Get the file name to open
	StandardGetFile( nil, numTypes, myTypes, &theSFReply ) ;
	
	if( theSFReply.sfGood )
		*theFile = theSFReply.sfFile ;
	
	// did the user cancel?
	return theSFReply.sfGood ;
	
}

TQ3FileObject MyGetInputFile (void)
{
TQ3FileObject myFileObj;
TQ3StorageObject myStorageObj;
FSSpec myFSSpec;

if (MetafileFileSpecify(&myFSSpec) == kQ3False)
return(NULL);

/*Create new storage object and new file object.*/
if(((myStorageObj = Q3FSSpecStorage_New(&myFSSpec)) == NULL)
	|| ((myFileObj = Q3File_New()) == NULL))
	{
	if (myStorageObj)
	Q3Object_Dispose(myStorageObj);
	return(NULL);
	}
/*Set the storage for the file object.*/
Q3File_SetStorage(myFileObj, myStorageObj);
Q3Object_Dispose(myStorageObj);
return (myFileObj);
}

// given a local to global array, convert a global point to local
// usefull for picking when local coordinates of a picked object are needed
void translateGlobalToLocal(TQ3Matrix4x4 *LtoG, TQ3Point3D *GPoint,TQ3Point3D *LPoint)
{
TQ3Matrix4x4 GtoL;

Q3Matrix4x4_Copy(LtoG,&GtoL);
Q3Matrix4x4_Invert(&GtoL,&GtoL);
Q3Point3D_Transform(GPoint, &GtoL, LPoint);
}

