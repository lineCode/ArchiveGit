# Microsoft Developer Studio Project File - Name="CTPUtil" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=CTPUtil - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CTPUtil.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CTPUtil.mak" CFG="CTPUtil - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CTPUtil - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "CTPUtil - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/CTPUtil", OKAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CTPUtil - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp2 /MD /W3 /GX /O1 /I "..\ZLib" /I "..\Stonehnd" /I "..\AgDoc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib ..\Stonehnd\Release\Stonehnd.lib ..\ZLib\Release\ZLib.lib /nologo /subsystem:windows /pdb:none /machine:I386

!ELSEIF  "$(CFG)" == "CTPUtil - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp2 /MDd /W3 /Gm /GX /Zi /Od /Gy /I "..\ZLib" /I "..\Stonehnd" /I "..\AgDoc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 version.lib ..\Stonehnd\Debug\Stonehnd.lib ..\ZLib\Debug\ZLib.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "CTPUtil - Win32 Release"
# Name "CTPUtil - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CTPUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\CTPUtil.rc
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CTPUtil.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\ProgDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\CTPUtil.ico
# End Source File
# Begin Source File

SOURCE=.\res\CTPUtil.rc2
# End Source File
# End Group
# Begin Group "AgDoc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\AgDoc\AGDC.cpp
# End Source File
# Begin Source File

SOURCE=..\AgDoc\AGDC.h
# End Source File
# Begin Source File

SOURCE=..\AgDoc\AGDib.h
# End Source File
# Begin Source File

SOURCE=..\AgDoc\AGDoc.cpp
# End Source File
# Begin Source File

SOURCE=..\AgDoc\AGDoc.h
# End Source File
# Begin Source File

SOURCE=..\AgDoc\AGLayer.cpp
# End Source File
# Begin Source File

SOURCE=..\AgDoc\AGLayer.h
# End Source File
# Begin Source File

SOURCE=..\AgDoc\AGMatrix.cpp
# End Source File
# Begin Source File

SOURCE=..\AgDoc\AGMatrix.h
# End Source File
# Begin Source File

SOURCE=..\AgDoc\AGPage.cpp
# End Source File
# Begin Source File

SOURCE=..\AgDoc\AGPage.h
# End Source File
# Begin Source File

SOURCE=..\AgDoc\AGSym.cpp
# End Source File
# Begin Source File

SOURCE=..\AgDoc\AGSym.h
# End Source File
# Begin Source File

SOURCE=..\AgDoc\AGText.cpp
# End Source File
# Begin Source File

SOURCE=..\AgDoc\AGText.h
# End Source File
# End Group
# End Target
# End Project
