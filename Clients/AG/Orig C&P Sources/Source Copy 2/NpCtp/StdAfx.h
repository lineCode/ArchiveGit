// stdafx.h : include file for standard system include files,
//		or project specific include files that are used frequently,
//		but are changed infrequently

#if !defined(AFX_STDAFX_H__4B81C688_1084_11D3_9330_00104BC4A611__INCLUDED_)
#define AFX_STDAFX_H__4B81C688_1084_11D3_9330_00104BC4A611__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define STRICT
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define _WIN32_WINNT 0x0400
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <atlctl.h>
#include <atlwin.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#define WIDTH(r)		((r).right -(r).left)
#define HEIGHT(r)		((r).bottom -(r).top)
#define APP_RESOLUTION	1440

#endif // !defined(AFX_STDAFX_H__4B81C688_1084_11D3_9330_00104BC4A611__INCLUDED)
