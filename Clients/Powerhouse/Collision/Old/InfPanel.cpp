#include <windows.h>
#include <stdlib.h>
#include "proto.h"
#include "control.h"
#include "worlds.h"
#include "infpanel.h"

/**********************************************************************************************/
CCollInfo::CCollInfo()
/**********************************************************************************************/
{
	for ( int i=0; i<9; i++ )
	{
		ctColl[i].szColName = "";
		ctColl[i].iBitmapID = 0;
		ctColl[i].bCollected = FALSE;	
	}
}

/**********************************************************************************************/
CCollInfo::~CCollInfo()
/**********************************************************************************************/
{
}

/**********************************************************************************************/
void CCollInfo::SetCollected(short iCollNum)
/**********************************************************************************************/
{
	if ( iCollNum >= 1 && iCollNum <= 9 )
		ctColl[iCollNum-1].bCollected = TRUE;
}

/**********************************************************************************************/
void CCollInfo::SetUnCollected(short iCollNum)
/**********************************************************************************************/
{
	if ( iCollNum >= 1 && iCollNum <= 9 )
		ctColl[iCollNum-1].bCollected = FALSE;
}

/**********************************************************************************************/
BOOL CCollInfo::IsCollected(short iCollNum)
/**********************************************************************************************/
{
	if ( iCollNum >= 1 && iCollNum <= 9 )
		return ctColl[iCollNum-1].bCollected;
	else
		return FALSE;
}


/**********************************************************************************************/
void CCollInfo::SetBitmapIDs(short iLevel)
/**********************************************************************************************/
{
	for ( int i=0; i<9; i++ )
		ctColl[i].iBitmapID = (1000 + ((iLevel - 1) * 100) + 10+i+1);
}
