# Microsoft Developer Studio Project File - Name="QD3DSample" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=QD3DSample - Win32 (80x86) Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "QD3DSample.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "QD3DSample.mak" CFG="QD3DSample - Win32 (80x86) Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "QD3DSample - Win32 (80x86) Debug" (based on\
 "Win32 (x86) Application")
!MESSAGE "QD3DSample - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "QD3DSample - Win32 (80x86) Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\WinDebug"
# PROP BASE Intermediate_Dir ".\WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I "..\..\Interfaces" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo
# SUBTRACT BSC32 /Iu
LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib mfc30d.lib mfco30d.lib mfcd30d.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 QD3D_d.lib version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /out:"Debug\QuickDraw 3D Sample (Debug).exe" /libpath:"..\..\Lib"

!ELSEIF  "$(CFG)" == "QD3DSample - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\QD3DSamp"
# PROP BASE Intermediate_Dir ".\QD3DSamp"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /Fr /YX /c
# ADD CPP /nologo /G5 /MD /W3 /GX /Od /Ob2 /I "..\..\Interfaces" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo
# SUBTRACT BSC32 /Iu
LINK32=link.exe
# ADD BASE LINK32 version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib qd3d_d.lib /nologo /subsystem:windows /debug /machine:I386 /out:"WinDebug/QuickDraw 3D Sample.exe"
# ADD LINK32 qd3d.lib version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /pdb:none /machine:I386 /out:"Release\QuickDraw 3D Sample.exe" /libpath:"..\..\Lib"
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "QD3DSample - Win32 (80x86) Debug"
# Name "QD3DSample - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\3DShell.c
# End Source File
# Begin Source File

SOURCE=.\Box3DSupport.c
# End Source File
# Begin Source File

SOURCE=.\Do_PixmapDrawContext.c
# End Source File
# Begin Source File

SOURCE=.\Do_Win32DCDrawContext.c
# End Source File
# Begin Source File

SOURCE=.\QD3DSample.RC
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\3dShell.h
# End Source File
# Begin Source File

SOURCE=.\Box3DSupport.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\RES\QD3DSample.ICO
# End Source File
# End Group
# End Target
# End Project
