#include <windows.h>
#include <mmsystem.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "proto.h"
#include "videofx.h"
#include "phapp.h"
#include "control.h"
#include "cllsnid.h"
#include "weapon.h"
#include "cllsn.h"

static BYTE ColorTbl[256] = 
{ 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 237, 236, 235, 234, 233, 232, 231, 230, 229, 228, 227, 226, 225, 224, 223, 222, 221, 220, 219, 218, 217, 216, 215, 214, 213, 212, 211, 210, 209, 208, 207, 206, 205, 204, 203, 202, 201, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 189, 188, 187, 186, 185, 184, 183, 182, 181, 180, 179, 178, 177, 176, 175, 174, 173, 172, 171, 170, 169, 168, 167, 166, 165, 164, 163, 162, 161, 160, 159, 158, 157, 156, 155, 154, 153, 152, 151, 150, 149, 148, 147, 146, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, };

//***********************************************************************
VideoFx::VideoFx(BOOL bType)
//***********************************************************************
{
	iEvent = 0;

	if (bType == VFX_TYPE_TURN)
	{
		pSndSmall = new CDirSnd();
		pSndSmall->Open(IDC_TURNSND, GetApp()->GetInstance(), TRUE);
		//pSndSmall->SetVolume(-500);

		pSndBig = new CDirSnd();
		pSndBig->Open(IDC_TURNSND180, GetApp()->GetInstance(), TRUE);
		pSndBig->SetVolume(-500);
	}
	else
	{
		pSndSmall = NULL;
		pSndBig = NULL;
	}


	pCurSnd = NULL;
	m_szText[0] = NULL;
	m_pTextDib = NULL;
	m_nPriority = 0;

	// Set up as a video observer
	VideoSetObserver(this);
}

//***********************************************************************
VideoFx::~VideoFx()
//***********************************************************************
{
	if (pSndSmall)
		delete pSndSmall;
	if (pSndBig)
		delete pSndBig;
	VideoClearObserver(this);
}

//***********************************************************************
BOOL VideoFx::DoFx(HWND hVideoWnd, int iEventCode, LPSHOT lpShot)
//***********************************************************************
{
	if ( iEventCode != EVENT_LEFT &&
		 iEventCode != EVENT_RIGHT &&
		 iEventCode != EVENT_DOWN)
	{
		iEvent = 0;
		return( NO );
	}

	m_lpShot = lpShot;
	iEvent = iEventCode;
	State = StartState;
	hWnd = hVideoWnd;
	m_hPal = NULL;

	return( YES );
}

//***********************************************************************
BOOL VideoFx::TextOn(HWND hSceneWnd, char *pszText, COLORREF cColor, 
					 int nPointSize, int nMills, int nPriority)
//***********************************************************************
{
	if (nPriority < m_nPriority)
		return FALSE;

	lstrcpy(m_szText, pszText);
	if (m_pTextDib)
		delete m_pTextDib;

	m_pTextDib = NULL;

	m_nPriority = nPriority;

	m_cColor = cColor;
	if (m_cColor == 0)
		m_cColor = RGB(255, 255, 255);
	m_nPointSize = nPointSize;
	if (m_nPointSize == 0)
		m_nPointSize = 18;

	m_nMills = nMills;
	if (nMills)
		m_dwStartTime = timeGetTime();

	LPSCENE lpScene = CScene::GetScene(hSceneWnd);
	m_hPal = lpScene->GetPalette();

	return TRUE;
}

//***********************************************************************
void VideoFx::TextOff(void)
//***********************************************************************
{
	if (m_pTextDib)
		delete m_pTextDib;
	m_pTextDib = NULL;
	m_szText[0] = NULL;
	m_nMills = 0;
	m_nPriority = 0;
}

//***********************************************************************
BOOL VideoFx::TextUp(void)
//***********************************************************************
{
	if (m_szText[0] != NULL)
		return TRUE;
	else
		return FALSE;
}

//***********************************************************************
void VideoFx::OnVideoDraw(LPBITMAPINFOHEADER lpFormat, LPTR lpData)
//***********************************************************************
{
	if (iEvent == 0 && m_szText[0] == NULL)
		return;

	if (iEvent == EVENT_LEFT)
		Turn(lpFormat, lpData, EVENT_LEFT);
	else
	if (iEvent == EVENT_RIGHT)
		Turn(lpFormat, lpData, EVENT_RIGHT);
	else
	if (iEvent == EVENT_DOWN)
		Turn(lpFormat, lpData, EVENT_DOWN);
	else
	if (iEvent == EVENT_UP)
		Invert(lpFormat, lpData);

	if (m_szText[0])
		DrawDibText(lpFormat, lpData);
}

//***********************************************************************
void VideoFx::DrawDibText(LPBITMAPINFOHEADER lpbi, LPTR lpDestBits)
//***********************************************************************
{
	// If timed
	if (m_nMills)
	{
		int nDif = (int)timeGetTime() - m_dwStartTime;
		if (nDif > m_nMills)
		{
			TextOff();	
			return;
		}
	}

	if (!m_pTextDib)
	{

		m_pTextDib = TextToDib (m_szText, "Arial", m_nPointSize, 0, m_cColor,
			m_hPal, RGB(0, 0, 0) );
	}

	if (m_pTextDib)
	{
		int x = 320;							// Horizontal middle
		int y = 310 - m_pTextDib->GetHeight();	// Vertical bottom

		DrawSprite(100, x, y, m_pTextDib, lpbi, lpDestBits );
	}
}

//***********************************************************************
void VideoFx::Turn(LPBITMAPINFOHEADER lpbi, LPTR lpDestBits, int iEventCode)
//***********************************************************************
{
	int nDestHeight 	= lpbi->biHeight;
	int nDestWidth      = lpbi->biWidth;
	int	nDestByteWidth	= (((lpbi->biBitCount * (int)lpbi->biWidth) + 31) & (~31) ) / 8;

	int x1, x2, xi;
	if ( iEventCode == EVENT_LEFT || iEventCode == EVENT_DOWN )
	{
		x1 = 0;
		x2 = nDestWidth - 1;
		xi = 1;
	}
	else
	if ( iEventCode == EVENT_RIGHT )
	{
		x1 = nDestWidth - 1;
		x2 = 0;
		xi = -1;
	}
	else
		return;

	if (State == StartState)
	{
		long lCurrentFrame;
		LPVIDEO lpVideo;
		LPSHOT lpShot;

		// Turn sound track off
//		PostMessage( hWnd, WM_VIDEOSOUND_ONOFF, OFF, 0);
		nCount = 1;

		// Make sure there are enough frames to do the
		// transition (we may be at the end of the video
		// up against the wall)
		lpVideo = (LPVIDEO)GetWindowLong( hWnd, GWL_DATAPTR );
		nMaxCount = 0;
		if (lpVideo)
		{
			lpShot = Video_GetShot( lpVideo, lpVideo->lCurrentShot );
			if (lpShot)
			{
				lCurrentFrame = MCIGetPosition(lpVideo->hDevice);
				nMaxCount = lpShot->lEndFrame - lCurrentFrame;

				// If nMaxCount is close to the end then assign it a small number
				if (nMaxCount < 21)
					nMaxCount = 4;
				else
					nMaxCount = 0;
			}
		}

		if (iEventCode == EVENT_DOWN)
		{
			if (nMaxCount == 0)
				nMaxCount = 15;
				//nMaxCount = 7;	// temp for testing (restore above)
		}
		else
		{
			if (nMaxCount == 0)
				nMaxCount = 7;
				//nMaxCount = 3; // temp for testing (restore above)
		}
		//if (nMaxCount > 15 || nMaxCount == 0)
		//	nMaxCount = ( iEventCode == EVENT_DOWN ? 15 : 7 );

		nMidCount = (nMaxCount+1)/2;
		nSize = 2;
		State = MidState;
	}
	else
	if (State == MidState)
	{
		if (nCount == 1)
		{
			if (iEventCode == EVENT_DOWN)
				pCurSnd = pSndBig;
			else
				pCurSnd = pSndSmall;

			pCurSnd->SetPosition(0, 0, 0);
			pCurSnd->Play();
		}

		if ( ++nCount >= nMaxCount )
		{
			State = EndState;

		}
		if ( nCount <= nMidCount )
		{
			nSize *= 4;

			// If turning right for the first half of the turn...
			if (iEventCode == EVENT_RIGHT)
			{
				int x = (nMidCount - nCount) * 2;
				pCurSnd->SetPosition( x, 0, 0 );
			}
			// Else turning left or 180 for first half of the turn
			else
			{
				int x = (nMidCount - nCount) * -1 * 2;
				pCurSnd->SetPosition( x, 0, 0 );
			}

			// If the next frame is the middle one then go ahead and make the real turn
			if ( nCount == nMidCount )
			{	// Post the event to the video control
				// Have to post the message so we don't hang the drawproc (mci calls)
				if (!m_lpShot)
				{
					PostMessage( hWnd, WM_VIDEO_EVENT, iEventCode, 0);
				}
				else
				{
					PostMessage( hWnd, WM_VIDEO_GOTOSHOT, 0, (LPARAM)m_lpShot);
					//LPVIDEO lpVideo = (LPVIDEO)GetWindowLong( hWnd, GWL_DATAPTR );
					//Video_GotoShot( lpVideo->hWnd, lpVideo, m_lpShot->lShotID, 
					//	1/*lCount*/, 0/*iFrames*/, m_lpShot->iLastEvent );
				}

			}
		}
		else
		{
			nSize /= 4;

			// If turning right for the last half of the turn...
			if (iEventCode == EVENT_RIGHT)
			{
				int x = (nCount - nMidCount) * 7 * -1;
				pCurSnd->SetPosition( x, 0, 0 );
			}
			// Else turning left or 180 for last half of the turn
			else
			{
				int x = (nCount - nMidCount) * 7;
				pCurSnd->SetPosition( x, 0, 0 );
			}

		}

	}
	else 
	{
		iEvent = 0;

		// Turn sound track back on
		//pSndSmall->Stop();
//		PostMessage( hWnd, WM_VIDEOSOUND_ONOFF, ON, 0);

	}

	if ( nSize <= 1 )
		return;

	if ( nDestHeight < 3 )
		return;

	LPTR pLast = lpDestBits;				// line 0
	LPTR pThis = pLast + nDestByteWidth;	// line 1
	LPTR pNext = pThis + nDestByteWidth;	// line 2

//	LFIXED fPos = fget( x1 + (xi*nSize), 1 );
//	LFIXED fLast = fPos;
//	int yLast = (int)sqrt( ((long)nDestHeight * nDestHeight) - ((long)nSize * nSize) );
//	LFIXED fRate = fget( -(xi*nSize), yLast );
//	yLast = nDestHeight - yLast;

	// For every line
	for( int y=2; y<nDestHeight; y++ )
	{ // loop on all but the first two lines
		int n = GetRandomNumber(nSize);

		BYTE Pixel = *pNext;
		LPTR pDest = pNext;

		// For the pixels on the line
		for( int x=x1; x!=x2; x+=xi )
		{
			if ( !n )
			{
				Pixel = *(pNext + x);
				n = nSize - 1;
			}
			else
			{
				*(pDest + x) = Pixel;
				if ( pDest == pLast )
					pDest = pThis;
				else
				if ( pDest == pThis )
					pDest = pNext;
				else
				if ( pDest == pNext )
					pDest = pLast;
				--n;
			}
		}

//		if ( y >= yLast )
//		{
//			int xx1 = fmul(1,fPos);
//			int xx2 = fmul(1,fLast);
//			if ( (xx1 > xx2 && (xi > 0)) ||
//				 (xx1 < xx2 && (xi < 0)) )
//				return;
//			for( int x=xx1; x!=xx2; x+=xi )
//			{
//				if ( x >= 0 && x <= nDestWidth )
//					*(pNext + x) = 255;
//				else
//					return;
//			}
//			fLast = fPos;
//			fPos += fRate;
//		}

		pLast += nDestByteWidth;
		pThis = pNext;
		pNext += nDestByteWidth;
	}
}

//***********************************************************************
void VideoFx::Invert(LPBITMAPINFOHEADER lpbi, LPTR lpData) 
//***********************************************************************
{
	/***
	int nDestHeight		= lpbi->biHeight;
	int nDestWidth      = lpbi->biWidth;
	int i, j;
	long lDestOffset;
	int	nDestByteWidth	= (((lpbi->biBitCount * (int)lpbi->biWidth) + 31) & (~31) ) / 8;
	long lStart = timeGetTime();
	long lTime;

	for( i=0; i<nDestHeight; i++ )
	{
		for( j=0; j<nDestWidth; j++ )
		{
			lDestOffset = (nDestHeight - i - 1) * nDestByteWidth + j - 1;
			*(lpData + lDestOffset) = 255 - *(lpData + lDestOffset);
		}
	}
	lTime = timeGetTime() - lStart;
	i = 5;
	***/

	long lCount;
	long lStart = timeGetTime();
	long lIndex;
	long lTime;

	lCount = lpbi->biHeight * (((lpbi->biBitCount * (int)lpbi->biWidth) + 31) & (~31) ) / 8;

	for( lIndex=0; lIndex<lCount; ++lIndex )
	{
		*lpData = ColorTbl[*lpData];
		++lpData;
	}

	lTime = timeGetTime() - lStart;
	lCount = 5;
}

//***********************************************************************
void VideoFx::PlayFxSound( ITEMID id )
//***********************************************************************
{
	if (id == 0)
		pSndSmall->Play();
	else
		pSndBig->Play();
}

