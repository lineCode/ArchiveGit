#pragma once 

#include "sctypes.h"
#include "scselect.h"
#include "scspcrec.h"
#include "screfdat.h"
#include "scparagr.h"

// FOR USE IN THE LINE BREAKER

class CandBreak {
public:
	long	breakCount; 	/* the count */
	long	startCount; 	/* stream count at start of line */
	long	streamCount;	/* stream count from start of this line */
	ushort		wsSpaceCount;	/* # of inter-word spaces at this break */
	ushort		spaceCount; 	/* # of glue spaces in interword spaces */
	ushort		trailingSpaces; /* # of trailing spaces */
	ushort		chCount;		/* # of chars */
	ushort		fillSpCount;	/* # of fillspaces we have to patch */
	int 		lineVal;		/* offset into leadvals of lead */
	eBreakType	breakVal;		/* goodness of break val */
	MicroPoint	minGlue;		/* minimum glue */
	MicroPoint	optGlue;		/* width of optGlue to this point */
	MicroPoint	maxGlue;		/* max glue */
	MicroPoint	curBox; 		/* width of immovable to this point */
	MicroPoint	fHangable;		/* width of hanging character if any */
	CharRecordP theChRec;		/* pointer into stream */
	short		specChanged;	/* spec changed since last candidate */
	TypeSpec	spec;			/* spec at this break point */
	scSpecRecord *specRec;

				CandBreak();
	void		Init();

	CandBreak&	operator=( const CandBreak& ); 
};

class scMaxLineVals { 
public:
	scMaxLineVals() :
	fSpecRec( 0 ),
	fOblique( 0 ) {}

	void Init( void )
		{ fSpecRec = 0; fMaxLead.Init( scFlowDir( eRomanFlow ) ); 
		  fMaxInkExtents.Set( 0, 0, 0, 0 ); fOblique = 0; } 
	scSpecRecord*		fSpecRec;
	scLEADRefData		fMaxLead;
	scXRect 			fMaxInkExtents;
	scAngle 			fOblique;
};


/* ========================================================== */

enum eBreakEvent {
	start_of_line,
	in_line,
	measure_exceeded,
	end_of_stream_reached
};

typedef eBreakEvent (*BrFunc)( void );

class BreakStruct {
public:
	BreakStruct();
	~BreakStruct();

	void				Init();

	BrFunc* 			breakMach;
	CandBreak*			candBreak;
	
		// CURRENT BREAK POINT STATE
	CandBreak			cB; 		

	scMemHandle 		brkLineValsH;
			// a list of max line vals for each spec on the line */
	scMaxLineVals*		fMaxLineVals;
			// zero this and make sure it stays that way */
	scMaxLineVals		fZeroMaxLineVals;

	CharRecordP 		gStartRec;

	TypeSpec			pspec_;

	scSpecRecord*		theSpecRec;

	MicroPoint			tmpMinGlue;
	MicroPoint			tmpOptGlue;
	MicroPoint			tmpMaxGlue;
	GlyphSize			letterSpaceAdj;

	MicroPoint			originalMeasure;
	MicroPoint			desiredMeasure;
	MicroPoint			hyphenationZone;
	
						/* length of last line set, for ragged setting */
	MicroPoint			lastLineLen;
	GlyphSize			justSpace;
	MicroPoint			theLineOrg;

		/* space set by character indent */
	MicroPoint			charIndent;
	MicroPoint			minRelPosition;

		/* we need local values of this in case 
		 * the spec changes on the line 
		 */
	MicroPoint			brkLeftMargin;
	MicroPoint			brkRightMargin;

	MicroPoint			totalTrailingSpace;

	long				theLineCount;

	BOOL				firstGlue;
	BOOL				firstBox;
	BOOL				allowHyphens;
	BOOL				allowJustification;

	BOOL				fNoStartline;			/* true if previous char was		*/
												/* starting punctuation 			*/
	MicroPoint			fLastHangable;			/* width of last character that was hangable */
	short				numTargetChars; 		/* num target chars rubi applied to */

	short				lineHyphenated;

		/* this the setting for the line based upon
		 * the first spec found on the line or a quad 
		 * character
		 */
	eTSJust 			effectiveRag;

		/* if the column has horz flex we 
		 * fit all the line flush left and
		 * then reposition all the lines
		 */
	eTSJust 			colShapeRag;

	scColumn				*theBreakColH;

		/* we found a character indent char on this line */
	BOOL				foundCharIndent;
};

/**************************************************************************/

class GlobalColumnStruct {
public:
	GlobalColumnStruct() {}
	~GlobalColumnStruct() {}
	TypeSpec defaultSpec;
	/* this is the current column we are breaking in */
	scColumn* theActiveColH;
};

/***********************************************************************/

extern BreakStruct			gbrS;
extern GlobalColumnStruct	ggcS;
extern scStreamChangeInfo	gStreamChangeInfo;
