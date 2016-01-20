/*		Type				Kind	Data Size	Data Type		Description						*/
#define	TextBegin			150	/*	6			TTxtPicRec		Begin text layout modifications		*/
#define	TextEnd				151	/*	0			NIL				End text layout modifications		*/
#define	StringBegin			152	/*	0			NIL				Begin segmented string				*/
#define	StringEnd			153	/*	0			NIL				End segmented string				*/
#define	TextCenter			154	/*	8			TTxtCenter		Specify center of rotation for		*/
								/*								the TextBegin comment (above)		*/
								
#define	LineLayoutOff		155	/*	0			NIL				Disable driver's line layout routine*/	
#define	LineLayoutOn		156	/*	0			NIL				Enable driver's line layout routine	*/	
#define	LineLayoutConfig	157	/*	16			LLConfig		Customize driver's line layout routine*/

#define	PolyBegin			160	/*	0			NIL				Begin PostScript polygon			*/	
#define	PolyEnd				161	/*	0			NIL				End PostScript polygon				*/	
#define	PolyIgnore			163	/*	0			NIL				Ignore StdLine calls(for smoothed polys)*/	
#define	PolySmooth			164	/*	1			PolyVerb		Smooth a polygon using B-Splines	*/	
#define	picPlyClo			165	/*	0			NIL				Close an open polygon(connect start and	*/
								/*								end points							*/
								
#define	DashedLine			180	/*	-			TDashedLine		Begin PostScript line dashing		*/
#define	DashedStop			181	/*	0			NIL				End PostScript line dashing			*/
#define	SetLineWidth		182	/*	4			Point			Set hi resolution line width 		*/	

#define	PostScriptBegin		190	/*	0			NIL				Signal driver to ingnore Quickdraw calls*/
#define	PostScriptEnd		191	/*	0			NIL				Re-enable Quickdraw					*/
#define	PostScriptHandle	192	/*	-			Handle			Transfer a block of PostScript		*/	
#define	PostScriptFile		193	/*	-			StringHandle	Transfer a file of PostScript		*/	
#define	TextIsPostScript	194	/*	0			NIL				Use DrawString to send PostScript	*/
#define	ResourcePS			195	/*	8			Type/ID/Index	Transfer a resource of PostScript	*/	
#define	PSBeginNoSave		196	/*	0			NIL				Like #190 but doesn't save PS state */


#define	RotateBegin			200	/*	4			TRotation		Begin hi-resolution graphics rotation*/	
#define	RotateEnd			201	/*	0			NIL				End rotation						*/
#define	RotateCenter		202	/*	8			Center			Specify center of rotation			*/

#define	FormsPrinting		210	/*	0			NIL				Specify graphics to remain resident	*/
								/*								for all pages in the print job		*/
#define	EndFormsPrinting	211	/*	0			NIL				Evict the residents					*/


typedef struct TTxtPicRec {
	unsigned char	tJus;		/* Justification for line layout of text.								*/
	unsigned char	tFlip;		/* Horizontal or vertical flipping										*/
	short			tRot;		/* Text rotation (0..360) (for fractional rot., see extTTxtPicRec below)*/
	unsigned char	tLine;		/* Single, 1 1/2, or double line spacing								*/
	unsigned char	tCmnt;		/* Reserved																*/
} TTxtPicRec, *TTxtPicPtr, **TTxtPicHdl;

typedef struct ExtTTxtPicRec {
	unsigned char		tJus;		/* Justification for line layout of text.							*/
	unsigned char		tFlip;		/* Horizontal or vertical flipping									*/
	short				tRot;		/* (Ignored)														*/
	unsigned char		tLine;		/* Single, 1 1/2, or double line spacing							*/
	unsigned char		tCmnt;		/* Reserved															*/
	short				tRotFixed;	/* Text rotation (0..360)											*/
} ExtTTxtPicRec, *ExtTTxtPicPtr, **ExtTTxtPicHdl;

typedef struct TTxtCenter {
	short		y, x;		/* Offset from current pen location to center of rotation			*/
	short	xFrac, yFrac;
} TTxtCenter, *TTxtCenterPtr, **TTxtCenterHdl;

typedef struct TPolyVerb {
	Boolean f7, f6, f5, f4, /* Reserved															*/
		fPolyRes, 			/* TRUE = Polygon was specified with doubled points					*/
		fPolyClose, 		/* TRUE = Polygon should be closed									*/
		fPolyFill, 			/* TRUE = Polygon should be filled									*/
		fPolyFrame;			/* TRUE = Polygon should be framed									*/
} TPolyVerb, *TPolyVerbPtr, **TPolyVerbHdl;

typedef struct TDashedLine {
	SignedByte	offset;		/* Offset as specified by PostScript ???							*/
	SignedByte	centered;	/* (Ignored) ???													*/
	SignedByte	*dashed;	/* Dynamic array of dash intervals									*/
} TDashedLine, *TDashedLinePtr, **TDashedLineHdl;

struct PSRsrc {
	ResType		PSType;		/* Resource type.  (Actual resource format is 'STR ' or 'STR#')		*/
	short		PSID;		/* Resource ID.														*/
	short		PSIndex;	/* If PSType = 'STR ' then 0 otherwise index into 'STR#'.			*/
};
typedef struct PSRsrc *PSRsrcPtr, **PSRsrcHdl;

typedef struct Rotation 
{
	short	flip;			/* Horizontal/vertical flipping (0=none,1=horizontal,2=vertical)	*/
	short	angle;			/* Clockwise rotation in degrees (0..360)							*/
} Rotation, *RotationPtr, **RotationHdl;

typedef struct ExtRotation 
{
	short		flip;		/* Horizontal/vertical flipping (0=none,1=horizontal,2=vertical)	*/
	short		angle;		/* (Ignored)														*/
	Fixed		angleFixed;	/* Clockwise rotation in degrees (0..360)							*/
} ExtRotation, *ExtRotationPtr, **ExtRotationHdl;


