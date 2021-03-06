/*.T(32)  STDTYPES    Standard Types for C Programs   */

/*  旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
/*  �   Copyright (c) 1993-1995, Pegasus Imaging Corp.  All rights reserved.�*/
/*  突袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴�*/

#if defined(USEPICCLIB) || defined(USEPICSETJMP)
  #if !defined(USEPICCLIB)
    #if !defined(USECLIBMALLOC)
      #define USECLIBMALLOC
    #endif
    #if !defined(USECLIBMISC)
      #define USECLIBMISC
    #endif
  #endif
#include "picclib.h"
#endif

#if !defined(_STDTYPES)
#define _STDTYPES

/*  旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�*/
/*  �                                                                       �*/
/*  �   The following types are independent of the model:                   �*/
/*  �                                                                       �*/
/*  �   CHAR  / BYTE        - signed / unsigned 8-bit value                 �*/
/*  �   SHORT / WORD        - signed / unsigned 16-bit value                �*/
/*  �   LONG  / DWORD       - signed / unsigned 32-bit value                �*/
/*  �                                                                       �*/
/*  �   The following type is dependent on the model:                       �*/
/*  �                                                                       �*/
/*  �   INT   / UINT        - signed / unsigned value (16 or 32 bits)       �*/
/*  �   BOOL                - signed int (FALSE = 0, TRUE = 1)              �*/
/*  �                                                                       �*/
/*  �   The enumeration type should NOT be used since some compilers require�*/
/*  �   that this be an int, while others allow it to be 1-, 2-, or 4-bytes �*/
/*  �   depending on the values used by the enumeration elements.           �*/
/*  �                                                                       �*/
/*  突袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴�*/

#if defined(WINDOWS)
    #pragma message ("WINDOWS defined")
		#include <windows.h>
    #define EXPORTWINAPI WINAPI
#if defined(__FLAT__)
    #define DLLEXPORTWINAPI WINAPI
    #define PICHUGE
    #define PICFAR
#else
    #define DLLEXPORTWINAPI WINAPI __loadds
    #define PICHUGE __huge
    #define PICFAR  __far
#endif
#else
#ifdef OS2
    #define INCL_DOS
    #define INCL_DOSPROCESS
    #include <os2.h>
		#if defined(__IBMC__)
        #define DLLEXPORTWINAPI _System
				#define EXPORTWINAPI    _System
    #else
        #pragma message ("OS2 defined")
        #define DLLEXPORTWINAPI _syscall
        #define EXPORTWINAPI    _syscall
    #endif
#else
    #pragma message ("WINDOWS NOT defined")
    typedef signed   int    BOOL;
    typedef unsigned char   BYTE;
    typedef signed long     LONG;
    typedef unsigned int    UINT;
    #define DLLEXPORTWINAPI    
    #define EXPORTWINAPI
#endif
    typedef unsigned short  WORD;
    typedef unsigned long   DWORD;
		typedef signed   long   LPARAM;
#if defined(MACINTOSH)
		#define __cdecl
#else
		#define FALSE           0
		#define TRUE            1
#endif
		#define PICHUGE
		#define PICFAR
#endif

typedef signed char  SBYTE;
#define CHAR    char
#define INT     int
#define SHORT   short
#define PRIVATE static
#define PUBLIC

#ifdef __unix__

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))
#define wsprintf sprintf
#define OutputDebugString printf
#define LOWORD(x) (WORD)((x) && (0x0000FFFF))
#define HIWORD(x) (WORD)(x >> 16)
#define LOBYTE(x) (BYTE)((x) && (0x00FF))
#define HIBYTE(x) (BYTE)(x >> 8)

typedef WORD*  LPWORD;
typedef DWORD* LPDWORD;
typedef BYTE*  LPBYTE;
typedef void*  LPVOID;
typedef const char* LPCSTR;

#define _MAX_PATH 260

#endif /* __unix__ */

#endif
