////////////////////////////////////////////////////////////////////////////////////////////////
//	The new input dialog																	  //
//  specchartdail.cpp																	  //
//  James Vogh																				  //
////////////////////////////////////////////////////////////////////////////////////////////////



#include "tcrindialog.h"
#include "monacoprint.h"
#include "think_bugs.h"
#include "fullname.h"
#include "mcofile.h"
#include "pascal_string.h"
#include "colortran.h"
#include "techkon.h"
#include "tcr.h"
//#include "globalValues.h"
#include "recipes.h"

#include "pascal_string.h"
#include "mcocolsyn.h"
#include "errordial.h"
#include "positiondial.h"
#include <math.h>


#define RGB_PATCHES_8_11 "\pMcoPatchesRGB8-11"
#define RGB_PATCHES_11_17 "\pMcoPatchesRGB11-17"
#define RGB_PATCHES_RGB "\pMcoPatchesRGB8-11-RGB"



#define Input_Dialog		(2013)

#define	SPC_IDS { 	2,4,5,8,6,7,10,15,16,-1,17,-1,19,400,401,21,22,23,24}
/*		2,	\	// Ok_Box
		4,  \	// Save As
		5,  \	// Strip_rect
		8,  \	// Message
		6,  \	// current sheet
		7,  \	// current strip
		10,  \	// patch rect
		15,  \	// new patch rect
		16,  \	// slider
		-1,  \	// redo
		17,  \	// start
		-1,  \	// voice set
		19,  \	// threeDBox
		400,  \	// updateevent
		401,  \	// TimeEvent
		21,  \	// up
		22,  \	// down
		23,  \  // left
		24 	// right
*/

#define Message_List_ID		(1001)
#define Button_Labels (17010)



#define ClickNum 			(50)
#define EndColumnNum		(51)





SpChInDialog::SpChInDialog(ProfileDoc *dc,RawData *pD):TableDialog(dc,pD)
{
	short		iType;
	Handle		iHandle;
	Handle		sHandle;
	Rect 		iRect, r,pRect,tRect;	
	UserItemUPP  box3D;
	
	QDErr		error;
	GDHandle	oldGD;
	GWorldPtr	oldGW;
	RGBColor	c,oldbackcolor,oldforecolor;
	WindowPtr	oldP;
	OSErr			err;
	int32 		i,wi,hi;
	double w,h;
	short		spc_ids[] = SPC_IDS;
	
	for (i=0; i<NumInputIDS; i++) ids[i] = spc_ids[i];

	WinType = StripDialog;
	WinNum = 0;

	setDialog(Input_Dialog);
	frame_button(ids[Ok_Box]);	

	
	SetSheetStripText();
	
	// Added by James, 3D the box
	threeD_box(ids[ThreeDBox]);
	
	// initialize comminications with the techkon
	tcom = NULL;
	tcom = new TCR(doc->whichport,NULL,NULL);
	if (tcom != NULL) tcom->Initialize();
	

	w = patchD->patches.width;
	h = patchD->patches.height;
	
	if (tcom != NULL) tcom->setUpPatches(patchD->patches.columns,patchD->patches.rows,w,h,patchD->patches.starting,patchD->patches.direction);


	
	// initialize the big gworld 
	GetDItem (dp, ids[Strip_Rect], &iType, (Handle*)&iHandle, &iRect);
	tRect = iRect;
	OffsetRect(&tRect,-iRect.left,-iRect.top);
	error = NewGWorld( &BigGW, 32, &tRect, 0, 0, 0 );
	if (error != 0) 
		{
		delete this;
		return;
		} 
	GetGWorld(&oldGW,&oldGD);
	SetGWorld(BigGW,nil);	
	
	
//	GetBackColor(&oldbackcolor);
	GetForeColor(&oldforecolor);
	c.red = 65535;
	c.green = 65535;
	c.blue = 65535;
	RGBBackColor(&c);
	c.red = 0;
	c.green = 0;
	c.blue = 0;
	RGBForeColor(&c);		
	EraseRect( &tRect );
	
	SetGWorld(oldGW,oldGD);	
//	RGBBackColor(&oldbackcolor);
	RGBForeColor(&oldforecolor);	

//	SetPort(oldP);
		
	GetDItem (dp, ids[Patch_Rect], &iType, (Handle*)&iHandle, &pRect);
	num_display_patch = (iRect.bottom-iRect.top)/(pRect.bottom-pRect.top);
	top_patch = 0;
	
	current_top = current_patch - 2*num_display_patch/3;
	current_bottom = current_top + num_display_patch - 1;
	if (current_top < 0) 
		{
		current_top = 0;
		current_bottom = current_top + num_display_patch - 1;
		}
	if (current_bottom >= total_patches-1)
		{
		current_bottom = total_patches - 1;
		current_top = current_bottom - num_display_patch + 1;
		}
	
	current_disp = 2*num_display_patch/3;
	
	init();	
	getpatchRGB();
	
	Disable(dp,ids[Redo]);

	
	GetDItem (dp, ids[Slider], &iType, (Handle*)&iHandle, &pRect);
	SetCtlMin((ControlHandle)iHandle,0);
	SetCtlMax((ControlHandle)iHandle,total_patches-1);
	
	GetDItem(dp, ids[VoiceSet], &iType, (Handle*)&iHandle, &pRect);
	SetCtlValue((ControlHandle)iHandle,1);
	//voice_set = 1;
	checkFinished();
	DrawWindow();
}	

SpChInDialog::~SpChInDialog(void)
{
OSErr			err;

	if (tcom != NULL) delete tcom;
}


				

McoStatus SpChInDialog::readInPatches(void)
{
int i,count;
short		item;	
short		itemtype;
Handle		cn1,sliderControl;
Rect		r;
McoStatus state;
int32		start_patch;

//current_patch = current_patch/patch_per_strip;
//current_patch *= patch_per_strip;
start_patch = current_patch;


count = 0;
do {
	state = tcom->getNextPatch(current_patch,&patch_data[current_patch*3],0);
	if (state == MCO_SUCCESS) 
		{
		 playSound(EndColumnNum);
		}
	count ++;
	} while ((state != MCO_SUCCESS) && (count < 5));
if (state != MCO_SUCCESS) 
	{
	//tcom->TurnOnStripRead();
	return state;
	}
//playSound(ClickNum);
if (doc->patchtype == No_Patches) doc->patchtype = patchtype;
GetDItem ( dp,ids[Slider], &itemtype, (Handle*)&sliderControl, &r );
SetCtlValue ( (ControlHandle)sliderControl,current_patch);
//ShowPatchScroll(current_patch);
current_patch = next_patch[current_patch];
//current_patch++;
	
// check for error
checkError(start_patch);
if (current_patch >= total_patches) 
	{
	current_patch = total_patches -1;
	_waiting = FALSE;
	}
ShowPatchScroll(current_patch);
current_sheet = current_patch/(patch_per_strip*strip_per_sheet);
current_strip = (current_patch - current_sheet*(patch_per_strip*strip_per_sheet))/patch_per_strip;
SetSheetStripText();
//tcom->TurnOnStripRead();
return MCO_SUCCESS;
}



McoStatus SpChInDialog::DoStart(void)
{
Str255	st;
int32	wi,hi;
double	w,h;
short		iType;
Handle		iHandle;
Handle		sHandle;
Rect 		iRect, r,pRect,tRect;
double		patchPoints[6];
int32		tablePoints[6];
McoStatus 	state = MCO_SUCCESS;


w = patchD->patches.width;
h = patchD->patches.height;
		
if (tcom != NULL) tcom->setUpPatches(patchD->patches.columns,patchD->patches.rows,w,h,patchD->patches.starting,patchD->patches.direction);

if (_first) 
	{
	McoMessAlert(MCO_CALIB,0L);
	_first = 0;
	}

// check to see if auto alignment is to be used
if (McoMessAlert(MCO_FIND_PATCHS,0L) == MCO_SUCCESS)
	{
	tcom->setCurrentPatch(0);
	state = PositionTable(tcom,&tablePoints[0],1);
	if (state != MCO_SUCCESS) return state;
	tcom->moveToPatch(patchD->patches.rows-1);
	state = PositionTable(tcom,&tablePoints[2],2);
	if (state != MCO_SUCCESS) return state;
	tcom->moveToPatch(patchD->patches.rows*(patchD->patches.columns-1));
	state = PositionTable(tcom,&tablePoints[4],3);
	if (state != MCO_SUCCESS) return state;
	tcom->getPatchPoint(0,&patchPoints[0]);
	tcom->getPatchPoint(patchD->patches.rows-1,&patchPoints[2]);
	tcom->getPatchPoint(patchD->patches.rows*(patchD->patches.columns-1),&patchPoints[4]);
	tcom->setPoints(tablePoints,patchPoints);
	tcom->Calibrate();
	tcom->moveToPatch(current_patch);
	_waiting=1;	
	return MCO_SUCCESS;
	}
else if (McoMessAlert(MCO_ALIGN_PATCHES,0L) == MCO_SUCCESS)
	{
	tcom->setCurrentPatch(current_patch);
	_waiting=1;	
	return MCO_SUCCESS;
	}
return MCO_FAILURE;
}







McoStatus SpChInDialog::DoEvents(short item, Point clickPoint, Window_Events &code, MS_WindowTypes &wintype, int32 &winnum, void **data, Boolean &changed)
{
UserItemUPP myfilter;
Str255		theString;
short		itemtype;
Handle		cn1,sliderControl;
Rect		r;
short		part;
Point		mpoint;
int32		next_patch;
McoStatus	status;
WindowPtr	oldp;
int32		temp;
double 		m=1;

	GetPort(&oldp);
	SetPort(dp);

	Changed = FALSE;
	code = WE_Do_Nothing;
	wintype = No_Dialog;
	winnum = -1;
	status = MCO_SUCCESS;

	if (isPressed(0x38)) m = 10;

	if (item != -1) {
		if (item == TIMER_ITEM ) // a timer event has occured
			{   
			_last_tick_time2 = TickCount();
			if (_waiting)
				{
				doc->needsSave = TRUE;
			//	GetDItem ( dp,Message, &itemtype, (Handle*)&cn1, &r );
			//	GetIndString(theString,Message_List_ID,4);
			//	SetIText(cn1,theString);
				readInPatches();
				next_patch = findNextPatch();
				if (next_patch == -1)
					{
				//	GetDItem ( dp,Message, &itemtype, (Handle*)&cn1, &r );
				//	GetIndString(theString,Message_List_ID,5);
				//	SetIText(cn1,theString);
					_done == TRUE;
					if (current_patch == total_patches) _waiting = FALSE;
					Disable(dp,ids[Redo]);
					//Disable(dp,ReadNext);
					if (NotAlreadFinished) Disable(dp,Cancel);
					Enable(dp,ids[Save_as]);	
					Enable(dp,OK);			
					}
				else
					{

				//	GetDItem ( dp,Message, &itemtype, (Handle*)&cn1, &r );
				//	GetIndString(theString,Message_List_ID,2);
				//	SetIText(cn1,theString);
					}
				if (!_waiting)
					{
					GetDItem ( dp,ids[Start], &itemtype, (Handle*)&cn1, &r );
					GetIndString(theString,Button_Labels,1);
					SetCTitle((ControlHandle)cn1,theString);
					}
				checkFinished();
				}
			}
		else if (item == ids[Slider])   // the slider has been adjusted
			{
			GetMouse(&mpoint);
			do {
				current_patch = GetSlider(mpoint,ids[Slider]);
				ShowPatchScroll(current_patch);
				SetSheetStripText();
				current_sheet = current_patch/(patch_per_strip*strip_per_sheet);
				current_strip = (current_patch - current_sheet*(patch_per_strip*strip_per_sheet))/patch_per_strip;	
				GetMouse(&mpoint);
				} while ( StillDown ( ) );
			checkError((current_patch/patch_per_strip)*patch_per_strip);
			//_waiting = 0;
			Enable(dp,ids[Redo]);
			SetSheetStripText();
			}
		else if (item == ids[Start]) // start or stop reading
			{
			if (_waiting) 
				{
				_waiting = 0;
				GetDItem ( dp,ids[Start], &itemtype, (Handle*)&cn1, &r );
				GetIndString(theString,Button_Labels,1);
				SetCTitle((ControlHandle)cn1,theString);
				current_patch--;
				ShowPatchScroll(current_patch);
				SetSheetStripText();
				current_sheet = current_patch/(patch_per_strip*strip_per_sheet);
				current_strip = (current_patch - current_sheet*(patch_per_strip*strip_per_sheet))/patch_per_strip;
				}
			else 
				{
				if (DoStart() == MCO_SUCCESS)
					{
					GetDItem ( dp,ids[Start], &itemtype, (Handle*)&cn1, &r );
					GetIndString(theString,Button_Labels,2);
					SetCTitle((ControlHandle)cn1,theString);
					}
				}
			}
		else if (item == ids[Position_Up])
			{
			if (!_waiting)
				{
				tcom->moveDistance(0,m*0.5);
				}
			}
		else if (item == ids[Position_Down])
			{
			if (!_waiting)
				{	
				tcom->moveDistance(0,-0.5*m);
				}
			}
		else if (item == ids[Position_Left])
			{
			if (!_waiting)
				{
				tcom->moveDistance(-0.5*m,0);
				}
			}
		else if (item == ids[Position_Right])
			{
			if (!_waiting)
				{
				tcom->moveDistance(0.5*m,0);
				}
			}		
		else if (item == ids[Save_as])
			{
			StorePatches();
			doc->patchtype = patchtype;
			doc->handle_save_datafile(patchD);
			ShowPatch(current_patch);
			}
		else if (item == ids[VoiceSet]) 
			{
			GetDItem(dp, ids[VoiceSet], &itemtype, (Handle*)&cn1, &r );
			voice_set = GetCtlValue((ControlHandle)cn1);
			}
		else if (item == ids[Strip_Rect])
			{
			GlobalToLocal(&clickPoint);
			temp = whichPatch(clickPoint);
			if (temp != -1)
				{
				current_patch = temp;
				current_disp = current_patch-current_top;
				GetDItem ( dp, ids[Slider], &itemtype, (Handle*)&sliderControl, &r );
				SetCtlValue ( (ControlHandle)sliderControl,current_patch);
				ShowPatch(current_patch);
				SetSheetStripText();
				}
			}
		else if (item == OK)
			{
			StorePatches();
			doc->patchtype = patchtype;
			//doc->needsSave = TRUE;
			code = WE_Close_Window;
			wintype = WinType;
			winnum = WinNum;
			}
		else if (item == Cancel)
			{
			code = WE_Close_Window;
			wintype = WinType;
			winnum = WinNum;
			}
		}
	SetPort(oldp);
	return status;	    	
}


