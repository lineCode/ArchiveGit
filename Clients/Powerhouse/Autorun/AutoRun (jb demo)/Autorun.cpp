// INI search values for the APP's path
#define INI_FILENAME    "pinoc.ini"
#define INI_SECTION    	"Options"
#define INI_KEY    		"Content"

// Define the application
#define APP_CLASS		"jungle"
#define APP_EXE			""
#define APP_ARGS		""
#define APP_CD_LABEL	""

// If the app is not found...
#define SETUP_EXE		"jbdemo\\jbdemo.exe"
#define SETUP_CD_LABEL	"Terra Magica"

// What about us?
#define AUTORUN_CLASS	"autorun"
#define AUTORUN_TITLE	"The Jungle Book Demonstration Disk"

// Button names
#define IDS_CONTINUE	"Run Demo"
#define IDS_INSTALL		"Run Demo"
#define IDS_EXIT		"Cancel"

// Header files
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include "autorun.h"

#ifndef WIN32
	#define DRIVE_UNKNOWN     0
	#define DRIVE_NO_ROOT_DIR 1
	#define DRIVE_CDROM       5
	#define DRIVE_RAMDISK     6
#endif

#define WINPROC		FAR pascal EXPORT
#ifdef WIN32
	#define EXPORT
#else
	#ifndef EXPORT
		#define EXPORT	__export
	#endif
#endif

// Global Variables:
HINSTANCE hInst; // current instance
char szAppPath[_MAX_PATH];

// Foward declarations of functions included in this code module:
long WINPROC EXPORT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CenterWindow (HWND, HWND);
int OnCreate (HWND hWnd);

//************************************************************************
LPSTR FixPath( LPSTR lpPath )
//************************************************************************
{
int length;
if ( !(length = lstrlen(lpPath)) )
	return( lpPath );
while ( length && (lpPath[length-1] == '\n' || lpPath[length-1] == '\r') )
	lpPath[--length] = '\0';
if ( !length )
	return( lpPath );
if ( lpPath[length-1] != '\\' )
	lstrcat( lpPath, "\\" );
return( lpPath );
}

#ifndef WIN32
//************************************************************************/
BOOL IsCDROM (int DriveNumber)
//************************************************************************/
{
    BOOL bCDfound = FALSE;

    _asm    mov     ax,01500h
    _asm    xor     bx,bx
    _asm    int     02Fh
    _asm    or      bx,bx
    _asm    jz      Done
    _asm    mov     ax,0150bh
    _asm    mov     cx,DriveNumber
    _asm    int     02Fh
    _asm    or      ax,ax
    _asm    jz      Done
    bCDfound = TRUE;
	Done:
    return bCDfound;
}

/***************************************************************************
 *
 *        Name:  Get31ExtendedDriveType
 *
 *     Purpose:  Determines the type of the specified drive.  Extends
 *               standard Windows GetDriveType by identifying RAM drives and
 *               CD ROM drives.
 *
 *  Parameters:  Drive - Drive Number to check (A=0,B=1,etc...)
 *                       If Drive is an alphabetic character,
 *                       then it will be automatically converted to
 *                       the correct drive number (e.g. 'B' is same as 1,
 *                       'c' is same as 2).
 *
 *     Returns:  0 for invalid or unknown drive, or one of the following:
 *               DRIVE_REMOVABLE        // Same as Windows
 *               DRIVE_FIXED            // Same as Windows
 *               DRIVE_REMOTE           // Same as Windows
 *               DRIVE_CDROM            // Extended Type
 *               DRIVE_RAM              // Extended Type
 */
//************************************************************************/
UINT Get31ExtendedDriveType (int Drive)
//************************************************************************/
{
    UINT DriveType;

    DriveType = GetDriveType (Drive);
    switch (DriveType)
    {
	    case DRIVE_REMOTE:
    	    if (IsCDROM (Drive))
    	    	DriveType = DRIVE_CDROM;
        break;
    }
    return DriveType;
}

#endif // WIN32

//************************************************************************/
UINT GetExtendedDriveType(int i)
//************************************************************************/
{
	#ifdef WIN32
		FNAME szPath;
	
		*szPath = i + 'a';
		*MyCharNextN( szPath, 1 ) = ':';
		*MyCharNextN( szPath, 2 ) = '\\';
		*MyCharNextN( szPath, 3 ) = '\0';
		return( GetDriveType( szPath ));
	#else
		return(Get31ExtendedDriveType (i));
	#endif
}

/***********************************************************************/
int Print( LPSTR lpFormat, ... )
/***********************************************************************/
{
	// Put up a modal message box with arguments specified exactly like printf
	LPSTR lpArguments = (LPSTR)&lpFormat + sizeof(lpFormat);
	char szBuffer[512];
	wvsprintf( szBuffer, lpFormat, (LPSTR)lpArguments );
	HWND hFocusWindow = GetFocus();
	// Load app name resource
	int retc = MessageBox( hFocusWindow/*MessageParent*/, szBuffer, AUTORUN_TITLE, MB_OK );
	SetFocus( hFocusWindow );
	return( retc );
}

/***********************************************************************/
int PrintOKCancel( LPSTR lpFormat, ... )
/***********************************************************************/
{
	// Put up a modal message box with arguments specified exactly like printf
	LPSTR lpArguments = (LPSTR)&lpFormat + sizeof(lpFormat);
	char szBuffer[512];
	wvsprintf( szBuffer, lpFormat, (LPSTR)lpArguments );
	HWND hFocusWindow = GetFocus();
	// Load app name resource
	int retc = MessageBox( hFocusWindow/*MessageParent*/, szBuffer, AUTORUN_TITLE, MB_OKCANCEL );
	SetFocus( hFocusWindow );
	return( retc );
}

/************************************************************************/
BOOL FileExists( LPSTR lpFileName, LPSTR lpCDLabel )
/************************************************************************/
{
	UINT errmode = SetErrorMode(SEM_FAILCRITICALERRORS);

	// see if installed path is cool
	OFSTRUCT of;
    if ( OpenFile( lpFileName, &of, OF_EXIST ) != HFILE_ERROR )
	{
		SetErrorMode(errmode);
		return( TRUE );
	}

	if ( !lpCDLabel || !lstrlen(lpCDLabel) )
	{
		SetErrorMode(errmode);
		return( FALSE );
	}

	// keep trying to we get it or user gives up
	while (TRUE)
	{
		// see if there is a CDROM drive that exists with our program on it.
		for (int drive = 0; drive < 26; ++drive)
		{
			UINT type = GetExtendedDriveType(drive);
			if (type == DRIVE_CDROM)
			{
				lpFileName[0] = drive + 'a';
			    if ( OpenFile( lpFileName, &of, OF_EXIST ) != HFILE_ERROR )
					break;
			}
			else
			if (type == 1) 
			{
				drive = 26;
				break;
			}
		}
		// see if we found the file somewhere
		if (drive < 26)
			break; // file was found
		// ask user to insert CD into cdrom drive
		if (PrintOKCancel("Please insert the '%s' disk to continue.", (LPSTR)lpCDLabel) == IDCANCEL)
		{
			SetErrorMode(errmode);
			return( FALSE );
		}
		// keep trying
	}
									
	SetErrorMode(errmode);
	return( TRUE );
}

/************************************************************************/
BOOL IsInstalled()
/************************************************************************/
{
	if ( !lstrlen(APP_EXE) )
		return(FALSE);

	szAppPath[0] = '\0';
	GetPrivateProfileString(INI_SECTION, INI_KEY, "", szAppPath, sizeof(szAppPath), INI_FILENAME);
	FixPath( szAppPath );
	lstrcat( szAppPath, APP_EXE );

	if (lstrlen(szAppPath))
		return(FileExists(szAppPath, APP_CD_LABEL));
	else
		return(FALSE);
}

//************************************************************************
LPCSTR FileName( LPCSTR lpPath )
//************************************************************************
{
	LPCSTR lp = lpPath;
	lp += lstrlen( lpPath );
	while( *lp != '\\' && *lp != ':' )
	{
		if ( (--lp) < lpPath )
		    return( lpPath + lstrlen(lpPath) ); // null string
	}
	lp++;
	return( lp ); // the file name
}

//************************************************************************
LPSTR StripFile( LPSTR lpPath )
//************************************************************************
{
	LPSTR lp;
	if ( lp = (LPSTR)FileName( lpPath ) )
		*lp = '\0';
	return( lpPath );
}

//************************************************************************
BOOL ActivateApp()
//************************************************************************
{
	HWND hwnd = FindWindow(AUTORUN_CLASS, NULL);
	if (hwnd)
	{
		// We found another version of ourself. Lets defer to it:
		if (IsIconic(hwnd))
			ShowWindow(hwnd, SW_RESTORE);

		#ifdef WIN32
		SetForegroundWindow (GetLastActivePopup (hwnd));	
		#else
		SetActiveWindow (GetLastActivePopup (hwnd));	
		#endif

		// If this app actually had any functionality, we would
		// also want to communicate any action that our 'twin'
		// should now perform based on how the user tried to
		// execute us.
		return TRUE;
	}
	return(FALSE);
}

/************************************************************************/
BOOL InitApplication(HINSTANCE hInstance)
/************************************************************************/
{
	// Win32 will always set hPrevInstance to NULL, so lets check
	// things a little closer. This is because we only want a single
	// version of this app to run at a time
	#ifdef WIN32
	if (ActivateApp())
		return(FALSE);
	#endif

	// Fill in window class structure with parameters that describe
	// the main window.
	WNDCLASS  wc;
	wc.lpszClassName = AUTORUN_CLASS;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.lpszMenuName  = NULL;
	wc.hIcon         = LoadIcon (hInstance, MAKEINTRESOURCE(-1));
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);

	return RegisterClass(&wc);
}


/************************************************************************/
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
/************************************************************************/
{
	HWND hWnd = CreateWindow(AUTORUN_CLASS, AUTORUN_TITLE, 
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 0, 0, 0, 0,
		NULL, NULL, hInstance, NULL);

	if (!hWnd)
		return (FALSE);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return(TRUE);
}


/************************************************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
/************************************************************************/
{
	// Store instance handle in our global variable
	hInst = hInstance;

	// see if application we launch is already running
	if (FindWindow(APP_CLASS, NULL))
		return(FALSE);

	if (!hPrevInstance)
	{ // Will always be TRUE on Windows 95
		// Perform instance initialization:
		if (!InitApplication(hInstance))
			return (FALSE);
	}
	else
	{
		ActivateApp();
		return(FALSE);
	}

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
		return (FALSE);

	HANDLE hAccelTable = LoadAccelerators (hInstance, AUTORUN_CLASS);

	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator (msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (msg.wParam);

	lpCmdLine; // This will prevent 'unused formal parameter' warnings
}

/************************************************************************/
BOOL LaunchApplication(void)
/************************************************************************/
{
	char szImagePath[_MAX_PATH];
	lstrcpy(szImagePath, szAppPath);
	lstrcat(szImagePath, " ");
	lstrcat(szImagePath, APP_ARGS);

	if ( !FileExists(szImagePath, APP_CD_LABEL ) )
		return( FALSE );

	#ifdef WIN32
	STARTUPINFO         si;
	PROCESS_INFORMATION pi;

	si.cb = sizeof(STARTUPINFO);
	// Yeah, this could be done with a 'memset', but this is more illustrative:
	si.lpReserved =
		si.lpDesktop =
		si.lpTitle	=
		si.dwX	=
		si.dwY =
		si.dwXSize	=
		si.dwYSize =
		si.dwXCountChars =
		si.dwYCountChars =
		si.dwFillAttribute =
		si.dwFlags =
		si.wShowWindow =
		si.cbReserved2 =
		si.lpReserved2 =
		si.hStdInput =
		si.hStdOutput =
		si.hStdError =
		NULL;

	return CreateProcess (NULL, szImagePath, NULL, NULL, FALSE,
		CREATE_NEW_PROCESS_GROUP, NULL, NULL, &si, &pi);
	#else
	UINT err = WinExec(szImagePath, SW_SHOW);
	return (err >= 32);
	#endif
}

/************************************************************************/
BOOL LaunchSetup(void)
/************************************************************************/
{
	char szImagePath[_MAX_PATH];
//	GetModuleFileName(hInst, szImagePath, sizeof(szImagePath));
//	StripFile(szImagePath);
	szImagePath[0] = '\0';
	GetPrivateProfileString(INI_SECTION, INI_KEY, "", szImagePath, sizeof(szImagePath), INI_FILENAME);
	FixPath(szImagePath);
	lstrcat(szImagePath, SETUP_EXE);

	if ( !FileExists(szImagePath, SETUP_CD_LABEL ) )
		return( FALSE );

	#ifdef WIN32
	STARTUPINFO         si;
	PROCESS_INFORMATION pi;

	si.cb = sizeof(STARTUPINFO);
	// Yeah, this could be done with a 'memset', but this is more illustrative:
	si.lpReserved =
		si.lpDesktop =
		si.lpTitle	=
		si.dwX	=
		si.dwY =
		si.dwXSize	=
		si.dwYSize =
		si.dwXCountChars =
		si.dwYCountChars =
		si.dwFillAttribute =
		si.dwFlags =
		si.wShowWindow =
		si.cbReserved2 =
		si.lpReserved2 =
		si.hStdInput =
		si.hStdOutput =
		si.hStdError =
		NULL;

	return CreateProcess (NULL, szImagePath, NULL, NULL, FALSE,
		CREATE_NEW_PROCESS_GROUP, NULL, NULL, &si, &pi);
	#else
	UINT err = WinExec(szImagePath, SW_SHOW);
	return (err >= 32);
	#endif
}

HPALETTE hpal;
HBITMAP hbmAutoRun;
BITMAP bmAutoRun;

/************************************************************************/
long WINPROC EXPORT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
/************************************************************************/
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
		case WM_CREATE:
		{
			return OnCreate (hWnd);
		}

		case WM_PALETTECHANGED:
		{
			if ((HWND)wParam == hWnd)
				break;
		}

		// fall through to WM_QUERYNEWPALETTE

		case WM_QUERYNEWPALETTE:
		{
			hdc = GetDC(hWnd);

			if (hpal)
				SelectPalette(hdc, hpal, FALSE);

			UINT uMappedColors = RealizePalette(hdc);
			ReleaseDC(hWnd,hdc);

			if (uMappedColors>0)
			{
				InvalidateRect(hWnd,NULL,TRUE);
				return TRUE;
			}
			else
				return FALSE;
			break;
		}

		case WM_COMMAND:
		{
			wmId    = LOWORD(wParam); // Remember, these are...
			wmEvent = HIWORD(wParam); // ...different for Win32!
		
			switch (wmId)
			{
				case IDM_CONTINUE:
					if (!LaunchApplication())
					{
						// failed to launch your application, you need
						// to decide what to do in this case and add code
						// to do that here. You can use GetLastError() to
						// determine the cause of this failure.
						// Print( "Can't find '%ls'", (LPSTR)APP_EXE );
					}
					else
						PostMessage (hWnd, WM_CLOSE, 0, 0);
					break;

				case IDM_INSTALL:
					if (!LaunchSetup())
					{
						// failed to launch your application, you need
						// to decide what to do in this case and add code
						// to do that here. You can use GetLastError() to
						// determine the cause of this failure.
						// Print( "Can't find '%ls'", (LPSTR)SETUP_EXE );
					}
					else
						PostMessage (hWnd, WM_CLOSE, 0, 0);
					break;

				case IDM_EXIT:
					DestroyWindow (hWnd);
					break;

				default:
					return (DefWindowProc(hWnd, message, wParam, lParam));
			}
			break;
		}

		case WM_PAINT:
		{
			RECT rect; // Used in WM_PAINT
			HDC  hdcSrc; // Used in WM_PAINT
			HBITMAP hbmOld; // Used in WM_PAINT

			hdc = BeginPaint (hWnd, &ps);
			SelectPalette(hdc, hpal, FALSE);
			RealizePalette(hdc);
			
			GetClientRect (hWnd, &rect);
			hdcSrc = CreateCompatibleDC (hdc);
			hbmOld = SelectObject (hdcSrc, hbmAutoRun);
			SelectPalette(hdcSrc, hpal, FALSE);
			RealizePalette(hdcSrc);
			if (!BitBlt (hdc, 0, 0, bmAutoRun.bmWidth, bmAutoRun.bmHeight, hdcSrc, 0, 0, SRCCOPY)) {
				MessageBeep(0);
			}
			SelectObject (hdcSrc, hbmOld);
			DeleteDC (hdcSrc);
			EndPaint (hWnd, &ps);
			break;        
		}

		case WM_DESTROY:
		{
			// Tell WinHelp we don't need it any more...
			WinHelp (hWnd, "autorun.hlp", HELP_QUIT,(DWORD)0);
			DeleteObject (hpal);
			DeleteObject (hbmAutoRun);
			PostQuitMessage(0);
			break;
		}

		default:
			return (DefWindowProc(hWnd, message, wParam, lParam));
	}
	return (0);
}


// This is a 'utility' function I find usefull. It will center one
// window over another. It also makes sure that the placement is within
// the 'working area', meaning that it is both within the display limits
// of the screen, -and- not obscured by the tray or other frameing
// elements of the desktop.
/************************************************************************/
BOOL CenterWindow (HWND hwndChild, HWND hwndParent)
/************************************************************************/
{
	RECT    rChild, rParent, rWorkArea = {0,0,0,0};
	int     wChild, hChild, wParent, hParent;
	int     wScreen, hScreen, xScreen, yScreen, xNew, yNew;

	// Get the Height and Width of the child window
	GetWindowRect (hwndChild, &rChild);
	wChild = rChild.right - rChild.left;
	hChild = rChild.bottom - rChild.top;

	// Get the Height and Width of the parent window
	GetWindowRect (hwndParent, &rParent);
	wParent = rParent.right - rParent.left;
	hParent = rParent.bottom - rParent.top;

	// Get the limits of the 'workarea'
	#ifdef WIN32
	#if !defined(SPI_GETWORKAREA)
	#define SPI_GETWORKAREA 48
	#endif

	BOOL bResult = SystemParametersInfo(
    	SPI_GETWORKAREA,	// system parameter to query or set
    	sizeof(RECT),	// depends on action to be taken
    	&rWorkArea,	// depends on action to be taken
    	0);	
	wScreen = rWorkArea.right - rWorkArea.left;
	hScreen = rWorkArea.bottom - rWorkArea.top;
	xScreen = rWorkArea.left;
	yScreen = rWorkArea.top;

    #else 
    wScreen = hScreen = xScreen = yScreen = 0;
    #endif

	// On Windows NT, the above metrics aren't valid (yet), so they all return
	// '0'. Lets deal with that situation properly:
	if (wScreen==0 && hScreen==0) {
		wScreen = GetSystemMetrics(SM_CXSCREEN);
		hScreen = GetSystemMetrics(SM_CYSCREEN);
		xScreen = 0; // These values should already be '0', but just in case
		yScreen = 0;
	}

	// Calculate new X position, then adjust for screen
	xNew = rParent.left + ((wParent - wChild) /2);
	if (xNew < xScreen) {
		xNew = xScreen;
	} else if ((xNew+wChild) > wScreen) {
		xNew = (xScreen + wScreen) - wChild;
	}

	// Calculate new Y position, then adjust for screen
	yNew = rParent.top  + ((hParent - hChild) /2);
	if (yNew < yScreen) {
		yNew = yScreen;
	} else if ((yNew+hChild) > hScreen) {
		yNew = (yScreen + hScreen) - hChild;
	}

	// Set it, and return
	return SetWindowPos (hwndChild, NULL, xNew, yNew, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}


/************************************************************************/
LPBITMAPFILEHEADER ReadBitmap(LPSTR lpFileName)
/************************************************************************/
{
	HFILE fh;
	OFSTRUCT of;
	DWORD dwSize;
	LPBITMAPFILEHEADER lp;

	if ( (fh = OpenFile(lpFileName, &of, OF_READ)) < 0 )
		return(NULL);
		
	dwSize = _llseek(fh, 0, 2);	
	_llseek(fh, 0, 0);

	lp = (LPBITMAPFILEHEADER)GlobalAllocPtr(GPTR, dwSize);
	if (!lp)
		return(NULL);

	// read in the bits
	_hread( fh, lp, dwSize );
	_lclose( fh );
	return(lp);
}

// This function will be called when the WM_CREATE message is processed
// by our application. This is where we will load the bitmap to be
// displayed, create a palette for the image, resize the window to fit
// the bitmap, and put in our control buttons that will allow the user
// to tell us how to proceed.
/************************************************************************/
int OnCreate(HWND hWnd)
/************************************************************************/
{
	RECT rect;
	HFONT hfontBtn;
	HWND hwndBtnOk, hwndBtnExit;

	int i;
	LPBITMAPINFO lpbmi;
	LPBITMAPFILEHEADER lpbfh;

	int id;
	LPLOGPALETTE plgpl;
	int cColors;
	LPVOID pimage;
	TEXTMETRIC tm;
	int iHeight, iWidth, x, y;
	char szString[20];

	HDC hdc = GetDC(hWnd);
	if ( !hdc )
	{
		Print( "Can't get DC" );
		return(-1);
	}

	hpal = NULL;

	char szFileName[_MAX_PATH];
	GetModuleFileName(hInst, szFileName, sizeof(szFileName));
	StripFile(szFileName);
	lstrcat(szFileName, "title.bmp");
	
	lpbfh = ReadBitmap(szFileName);
	if (!lpbfh)
	{
		Print( "Can't find 'title.bmp'" );
		return(-1);
	}

	lpbmi = (LPBITMAPINFO)((LPSTR)lpbfh + sizeof(BITMAPFILEHEADER));

	// How many colors does it use?:
	cColors = (int)lpbmi->bmiHeader.biClrUsed;
	if (cColors == 0)
		cColors = (1 << (int)(lpbmi)->bmiHeader.biBitCount);

	// Use that to determine where the actual bitmap image starts:
	pimage = &(lpbmi->bmiColors[cColors]);

	// Now lets create a palette based on the image we loaded:
	plgpl = (LPLOGPALETTE )GlobalAllocPtr (GPTR, sizeof(LOGPALETTE) + (cColors-1)*sizeof(PALETTEENTRY));
	if (!plgpl)
	{
		Print( "Can't allocate palette memory" );
		return -1; // failure
	}

	plgpl->palVersion = 0x300;
	plgpl->palNumEntries = cColors;
	for (i=0; i<cColors; i++)
	{
		plgpl->palPalEntry[i].peRed   = lpbmi->bmiColors[i].rgbRed;
		plgpl->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;
		plgpl->palPalEntry[i].peBlue  = lpbmi->bmiColors[i].rgbBlue;
		//plgpl->palPalEntry[i].peFlags = PC_NOCOLLAPSE; // is this needed?
	}
	hpal = CreatePalette(plgpl);

	// And free up the memory we allocated:
	GlobalFreePtr (plgpl);

	if (!hpal)
	{
		Print( "Can't create a palette" );
		return -1; // Fail on no palette
	}

	// Now create a true DIBitmap from all of this:
	// First assign the palette to the DC:
	SelectPalette(hdc, hpal, FALSE);
	RealizePalette(hdc);

	// Now create a DIB based off the the DC with the bitmap image from the resource:
	hbmAutoRun = CreateDIBitmap (hdc, (BITMAPINFOHEADER FAR *)lpbmi,
		CBM_INIT, pimage, lpbmi, DIB_RGB_COLORS);

	GlobalFreePtr(lpbfh);

	if (!hbmAutoRun)
	{
		Print( "Can't create a dib" );
		return(-1);
	}

	// And get the header data:
	GetObject (hbmAutoRun, sizeof(BITMAP), &bmAutoRun);

	SetWindowPos (hWnd, NULL, 0, 0, bmAutoRun.bmWidth, bmAutoRun.bmHeight,
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	// Since what we really want to do is size the 'Client' window,
	// Lets find out how far off we were, adjust our values, and
	// try it again...
	GetClientRect (hWnd, &rect);
	x = bmAutoRun.bmWidth - (rect.right-rect.left);
	y = bmAutoRun.bmHeight - (rect.bottom-rect.top);
	SetWindowPos (hWnd, NULL, 0, 0, bmAutoRun.bmWidth+x,
		bmAutoRun.bmHeight+y, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	CenterWindow (hWnd, GetDesktopWindow ());

	// Ok, our image is loaded, and our window properly sized
	// Lets add in some controls: The text to use for these
	// buttons is coming out of the resource of this app
	if (IsInstalled())
	{
		id = IDM_CONTINUE;
		lstrcpy( szString, IDS_CONTINUE );
	}
	else
	{
		id = IDM_INSTALL;
		lstrcpy( szString, IDS_INSTALL );
	}

	hwndBtnOk = CreateWindow ("BUTTON", szString,
		WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | BS_DEFPUSHBUTTON,
		0, 0, 0, 0,
		hWnd, (HMENU)id, hInst, NULL);

	lstrcpy( szString, IDS_EXIT );
	hwndBtnExit = CreateWindow ("BUTTON", szString, 
		WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0,
		hWnd, (HMENU)IDM_EXIT, hInst, NULL);

	hfontBtn = GetStockObject (ANSI_VAR_FONT);
	SendMessage (hwndBtnOk, WM_SETFONT, (WPARAM)hfontBtn,(LPARAM)TRUE);
	SendMessage (hwndBtnExit, WM_SETFONT, (WPARAM)hfontBtn, (LPARAM)TRUE);
	SelectObject (hdc, hfontBtn);
	GetTextMetrics (hdc, &tm);

	iHeight = (tm.tmHeight + tm.tmExternalLeading) * 2;
	iWidth = (bmAutoRun.bmWidth / 2) - (iHeight * 2);
			
	x = ((bmAutoRun.bmWidth/2) / 2) - (iWidth/2);
	y = bmAutoRun.bmHeight - iHeight - (iHeight/2);

	SetWindowPos (hwndBtnOk, NULL, x, y, iWidth, iHeight,
		SWP_NOZORDER | SWP_NOACTIVATE);

	x += bmAutoRun.bmWidth/2;
	SetWindowPos (hwndBtnExit, NULL, x, y, iWidth, iHeight,
		SWP_NOZORDER | SWP_NOACTIVATE);

	ShowWindow (hWnd, SW_SHOW);

	ReleaseDC (hWnd, hdc);

	return 0; // Sucess
}
