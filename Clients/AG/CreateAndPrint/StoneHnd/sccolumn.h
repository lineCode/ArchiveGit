/****************************************************************************
Contains:	text container definitions
****************************************************************************/

#pragma once

#include "sctbobj.h"

class scRedispList;
class scSelection;
class TextMarker;
class scImmediateRedisp;
class stTextImportExport;
class scLINERefData;
class scCOLRefData;
class scXRect;
class scRedispList;
class scTypeSpecList;
class scLineInfoList;
class scSpecLocList;

class scColumn : public scTBObj
{
	scDECLARE_RTTI; 

friend class scCOLRefData;

public:
					scColumn( APPColumn,
							  MicroPoint,
							  MicroPoint,
							  scStream* p		= 0,
							  eCommonFlow	flow	= eRomanFlow );
				
					scColumn() :
						fShapePieces( 0 ),
						fRgnH( 0 ),
						fNextContext( 0 ),
						fAppName( 0 ),
						fColumnCount( 0 ),
						fSize( 0, 0 ),
						fFlowDir( eRomanFlow ),
						fStream( 0 ),
						fSelection( 0 ),
						fFirstline( 0 ){}
	
					~scColumn();
					
	///////////////////// FLOW SET METHODS ////////////////////////////
	
						// delete the stream from the flowset
						// RETURNS the damaged area(s)
						//	
	void			FlowsetClearStream( scRedispList* );

						// remove the stream from the flowset
						// RETURNS the damaged area(s)
						//	
	void			FlowsetCutStream( scStream*, scRedispList*);

						// paste the stream into the flowset
						// RETURNS the damaged area(s)
						//	
	void			FlowsetPasteStream( scStream*, scRedispList* );


	
						// get the selection object associated with
						// the flowset, if there is none it will
						// create one
						//
	scSelection*	FlowsetGetSelection( void );

						// set the selection object for the flowset
						// none should exist, since if it does
						// error recovery might be a bit tricky
						//
	void			FlowsetSetSelection( scSelection* );

						// this removes the selection from the flowset
						// NOTE: it does not delete it
						//
	void			FlowsetRemoveSelection( void );


						// invalidate any selection associated with
						// the flowset
	void			FlowsetInvalidateSelection( void );
	
						// set the flow for the flowset
						// all containers in a flowset must have the
						// same flow at this time
	void			FlowsetSetFlowdir( const scFlowDir& );

	scColumn*		GetFlowset( void ) const
						{
							return (scColumn*)FirstInChain();
						}

	void			RecomposeFlowset( long			ticks		= LONG_MAX, 
									  scRedispList* redisplist	= 0 );

	///////////////////// COLUMN METHODS //////////////////////////////

	void			Enumerate( long& );
	
						// draw the column updating the area
						// intersected by the damage rect
						//
	virtual void	Draw( const scXRect& damagedRect,
						  APPDrwCtx,
						  const scMuPoint* translation = 0 );
	

	void			Hilite( const TextMarker&, 
							const TextMarker&, 
							HiliteFuncPtr, 
							const scSelection&	selection );


	void			CheckOverflow(bool* pbOverflow );

						// FILE I/O

						// complete the read
	virtual void	Read( scSet*, APPCtxPtr, IOFuncPtr );

						// complete the write
	virtual void	Write( APPCtxPtr, IOFuncPtr );

						// restore the pointers after completing a read 
	virtual void	RestorePointers( scSet* );


	void			SetRecomposition( BOOL tf );
	BOOL			GetRecomposition( void ) const;


						// get or set the first line of the column
						//
	scTextline* 	GetFirstline( void ) const
						{
							return fFirstline;
						}
	void			SetFirstline( scTextline* txl )
						{
							fFirstline = txl;
						}

	scTextline* 	GetLastline( void ) const;


	void			TranslateLines( const scMuPoint& );
	void			RepositionLines( void );
			
	scContUnit* 	MarkParas( void );
	scContUnit* 	LastPara( void ) const;

						// return the first paragraph in this container
						// for reformatting purposes, we will assume that
						// the previous container has been successfully
						// reformatted
	scContUnit* 	FirstPara( void ) const;

						// return the number of lines for this column,
						// if it is not formatted it will return -1
	ushort			GetLinecount( void ) const;

	virtual void	Resize( const scSize& size, scRedispList* = 0 );
	void			Resize( MicroPoint, MicroPoint, scRedispList* = 0 );

	scXRect&		RepaintExtent( scXRect& );
	void			QueryMargins( scXRect& ) const;
	void			QuerySize( scSize& ) const;
	void			QueryTextDepth( MicroPoint& ) const;
	MicroPoint		TextDepth( void ) const;
	void			GetTSList( scTypeSpecList& ) const;

	void			Rebreak( scRedispList* );
	void			Rebreak2( scRedispList* );

	void			ExternalSize( long& );
	void			ZeroEnumeration( void );
	
	BOOL			GetStrip( scLINERefData&, int, scCOLRefData& );

	void			DeleteExcessLines( scContUnit*, scTextline*, BOOL, scCOLRefData& );

	/////////////////// COLUMN SHAPE METHODS //////////////////////////
	
	void			ReplacePoly( scVertHandle, scRedispList* );
	void			PastePoly( scVertHandle, scRedispList* );
	void			CopyPoly( scVertHandle* );

	void			PasteRgn( const HRgnHandle, scRedispList* );
	void			CopyRgn( HRgnHandle& );

	void			ClearShape( scRedispList* );

	///////////////// COLUMN LINKAGE METHODS //////////////////////////
	
	void			Link( scColumn*, BOOL, scRedispList* );
	void			Unlink( scRedispList* );

	void			Renumber( void );

	void			BreakChain( scColumn* );
	
						// get the next or previous column that
						// actually contains lines (i.e. composed text )
						//
	scColumn*		PrevWithLines( void ) const;
	scColumn*		NextWithLines( void ) const;

	void			ComputeInkExtents( void );

	void			SetInkExtents( MicroPoint x1, MicroPoint y1, MicroPoint x2, MicroPoint y2 )
						{ 
							fInkExtents.Set( x1, y1, x2, y2 ); 
						}

	const scXRect&	GetInkExtents( void ) const
						{
							return fInkExtents;
						}
	void			UnionInkExtents( const scXRect& xrect )
						{
							fInkExtents.Union( xrect );
						}


	BOOL			MoreText( void ) const;
	BOOL			HasText( void ) const;

	scStream*		GetStream( void ) const
						{
							return fStream;
						}
	void			SetStream( scStream* stream )
						{
							fStream = stream;
						}
	void			SetFlowsetStream( scStream* stream );
	void			FreeStream( void );

	eVertJust		GetVertJust( void ) const
						{
							return (eVertJust)fLayBits.fLayAdjustment;
						}
	void			SetVertJust( eVertJust vj )
						{
							fLayBits.fLayAdjustment = vj;
						}

	eColShapeType	GetShapeType( void ) const
						{
							return (eColShapeType)fLayBits.fLayType;
						}
	void			SetShapeType( eColShapeType st );


	ushort			GetShapePieces( void ) const
						{
							return fShapePieces;
						}

	scVertHandle	GetVertList( void ) const
						{
							return fVertH;
						}
	void			SetVertList( scVertHandle vl )
						{
							fVertH = vl;
						}

	HRgnHandle		GetRgn( void ) const
						{
							return fRgnH;
						}
	void			SetRgn( HRgnHandle rgn )
						{
							fRgnH = rgn;
						}

	void			SetAPPName( APPColumn appcol )
						{
							fAppName = appcol;
						}
	APPColumn		GetAPPName( void ) const
						{
							return fAppName;
						}

	void			SetWidth( MicroPoint w )
						{
							fSize.SetWidth( w );
						}
	MicroPoint		Width( void ) const
						{
							return fSize.Width();
						} 

	void			SetDepth( MicroPoint d )		{ fSize.SetDepth( d ); }
	MicroPoint		Depth( void ) const 			{ return fSize.Depth(); }

	void			SetSize( const scSize& size )
						{
							fSize = size;
						}
	const scSize&	GetSize( void ) const
						{
							return fSize;
						}

	void			SetSize( MicroPoint w, MicroPoint d )
						{
							fSize.SetWidth( w ), fSize.SetDepth( d );
						}

	scColumn*		GetPrev( void ) const
						{
							return (scColumn*)Prev();
						}	
	scColumn*		GetNext( void ) const			{ return (scColumn*)Next(); }  

	void			SetCount( long count )
						{
							fColumnCount = count;
						}
	long			GetCount( void ) const
						{
							return fColumnCount;
						}

	void			SetFlowdir( const scFlowDir& fd )
						{
							fFlowDir = fd;
						}
	const scFlowDir& GetFlowdir( void ) const
						{
							return fFlowDir;
						}

	void			SetContext( scColumn* ctx )
						{
							fNextContext = ctx;
						}
	scColumn*		GetContext( void ) const
						{
							return fNextContext;
						}

	void			SetVertFlex( BOOL, scRedispList* );
	void			SetHorzFlex( BOOL, scRedispList* );
	BOOL			GetVertFlex( void ) const
						{
							return GetShapeType() & eVertFlex;
						}
	BOOL			GetHorzFlex( void ) const
						{
							return GetShapeType() & eHorzFlex;
						}


	void			Delete( scRedispList* );
	void			Free( void );
	void			FreeShape( void );
	void			FreeScrap( void );
	void			UpdateLine( scImmediateRedisp&, APPDrwCtx );

	void			LineExtents( scImmediateRedisp& );
	void			FreeLines( BOOL, scXRect& );

	void			InvertExtents( HiliteFuncPtr, APPDrwCtx );
#if SCDEBUG > 1
	virtual void	scAssertValid( BOOL recurse = true ) const;
	void			DbgPrintInfo( int debugLevel = 0 ) const;
#else
	virtual void	scAssertValid( BOOL = true ) const{}
#endif

	static scColumn*	FindFlowset( const scStream* );

					// context list
	static scColumn*	GetBaseContextList( void )
						{
							return fTheContextList;
						}
	static void 		FiniCTXList( void );
	
	void			AddToCTXList( )
							{
								fNextContext	= fTheContextList;
								fTheContextList = this;
							}
	void			DeleteFromCTXList(	);
	void			VerifyCTXList( void ) const;


	static void 	ChangedTS( TypeSpec, eSpecTask, scRedispList* );
	static void 	Update( scRedispList* );




	void			LineInfo( scLineInfoList*,
							  long&,
							  BOOL& ) const;


	void			VertJustify( void );
	void			SetDepthNVJ( MicroPoint, scRedispList* );
	void			SetVJ( eVertJust );


				// COLUMN SELECTION
	void			ClickEvaluate( const scMuPoint&, 
								   double& );


	void			StartShiftClick( scStreamLocation&,
									 const scMuPoint&,
									 HiliteFuncPtr,
									 APPDrwCtx,
									 scSelection*& );

	void			StartClick( const scMuPoint&,
								HiliteFuncPtr,
								APPDrwCtx,
								scSelection*& );

	void			ContinueClick( const scMuPoint& ,
								   HiliteFuncPtr,
								   scSelection* );


	BOOL			Select( const scMuPoint&	hitPt,
							TextMarker* 		textMarker,
							double*				bestDist );
	

	void			InitialSelection( TypeSpec&, scSelection*& );

	void			SelectSpecial( const scMuPoint&,
								   eSelectModifier,
								   scSelection*& );

	void			LimitDamage( scRedispList*, long );

	void			PasteAPPText( stTextImportExport&, scRedispList* );

	void			ReadTextFile( TypeSpec,
								  APPCtxPtr,
								  IOFuncPtr,
								  scRedispList* );


protected:

						// do not confuse the following with flowset operations
	scSelection*	GetSelection( void )
						{
							return fSelection;
						}
	void			SetSelection( scSelection* sel )
						{
							fSelection = sel;
						}

						// actually allocate the real estate for lines
	virtual BOOL	GetStrip2( scLINERefData&, int, scCOLRefData& );

private:

	static scColumn*	fTheContextList;

	void			CreateSelection( void );
	
	scColumn*		fNextContext;

	APPColumn		fAppName;		// application name

	long			fColumnCount;
	scSize			fSize;

	scFlowDir		fFlowDir;		// the basic flow direction of a container

	scStream*		fStream;		// hook into stream
	scSelection*	fSelection;

	scXRect 		fInkExtents;	// actual extents w/ italics, idents, etc.

	scTextline* 	fFirstline; 	// firstline of the column

	ushort			fShapePieces;	// num of components of shape
	union {
		scVertHandle	fVertH;
		HRgnHandle		fRgnH;
	};
};

/*=========================================================================*/

#define FIRST_LINE_POSITION 		(LONG_MIN + 1)
#define HorzFlexMeasure 			(LONG_MAX - one_pica)

// these seems arbitrary, but we need to get it away from LONG_MAX

/*=========================================================================*/

short		COLLineNum( scSelection* );
