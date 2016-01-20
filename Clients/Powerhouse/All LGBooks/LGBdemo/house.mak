# Microsoft Visual C++ generated build script - Do not modify

PROJ = HOUSE
DEBUG = 0
PROGTYPE = 0
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = /d_DEBUG 
R_RCDEFINES = /dNDEBUG 
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = D:\VIRTUA~2\LGBOOKS.SRC\LGBDEMO\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC =             
FIRSTCPP = DIALOG.CPP  
RC = rc
CFLAGS_D_WEXE = /nologo /f- /G3 /Gy /W3 /Zi /AM /Od /Ob1 /D "_DEBUG" /D "STRICT" /D "USE_WINTOON" /D "NO_MIDI" /D "NO_CONTROLS" /D "USE_DIBFX" /FR /GA /Fd"DEBUG.PDB"
CFLAGS_R_WEXE = /nologo /f- /G3 /Gy /W3 /AM /Ox /Ob1 /D "NDEBUG" /D "STRICT" /D "USE_WINTOON" /D "NO_MIDI" /D "NO_CONTROLS" /D "USE_DIBFX" /FR /GA 
LFLAGS_D_WEXE = /NOLOGO /NOD /STACK:10240 /ALIGN:16 /ONERROR:NOEXE /CO /MAP /nopackcode
LFLAGS_R_WEXE = /NOLOGO /NOD /STACK:10240 /ALIGN:16 /ONERROR:NOEXE /nopackcode
LIBS_D_WEXE = libw mlibcew oldnames commdlg.lib mmsystem.lib 
LIBS_R_WEXE = vfw libw mlibcew oldnames commdlg.lib mmsystem.lib 
RCFLAGS = /nologo 
RESFLAGS = /nologo /k
RUNFLAGS = 
DEFFILE = ..\GAMEGUTS\POWER.DEF
OBJS_EXT = 
LIBS_EXT = ..\GAMEGUTS\WING.LIB ..\GAMEGUTS\WINTOON.LIB ..\GAMEGUTS\SLIBDIB.LIB ..\GAMEGUTS\VFW.LIB \
	..\GAMEGUTS\WAVMIX16.LIB ..\GAMEGUTS\GAMEGUTS.LIB 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_WEXE)
LFLAGS = $(LFLAGS_D_WEXE)
LIBS = $(LIBS_D_WEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_WEXE)
LFLAGS = $(LFLAGS_R_WEXE)
LIBS = $(LIBS_R_WEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = TOONCTRL.SBR \
		BELONGS.SBR \
		CREDITS.SBR \
		LGBOOK.SBR \
		HOUSE.SBR \
		CHASE.SBR \
		COLORBOK.SBR \
		OPTIONS.SBR \
		SOUNDBOX.SBR \
		CONNECT.SBR \
		MIXUP.SBR


WING_DEP = 

WINTOON_DEP = 

SLIBDIB_DEP = 

VFW_DEP = 

WAVMIX16_DEP = 

GAMEGUTS_DEP = 

TOONCTRL_DEP = d:\virtua~2\lgbooks.src\lgbook\lgbook.h \
	d:\virtua~2\lgbooks.src\lgbook\lgbookid.h \
	d:\virtua~2\lgbooks.src\lgbook\toonctrl.h \
	d:\virtua~2\lgbooks.src\lgbook\toon.h


BELONGS_DEP = d:\virtua~2\lgbooks.src\lgbook\belongs.h \
	d:\virtua~2\lgbooks.src\lgbook\lgbook.h \
	d:\virtua~2\lgbooks.src\lgbook\lgbookid.h \
	d:\virtua~2\lgbooks.src\lgbook\toonctrl.h \
	d:\virtua~2\lgbooks.src\lgbook\toon.h \
	d:\virtua~2\lgbooks.src\lgbook\belondef.h


CREDITS_DEP = d:\virtua~2\lgbooks.src\lgbook\credits.h \
	d:\virtua~2\lgbooks.src\lgbook\lgbook.h \
	d:\virtua~2\lgbooks.src\lgbook\lgbookid.h \
	d:\virtua~2\lgbooks.src\lgbook\toonctrl.h \
	d:\virtua~2\lgbooks.src\lgbook\toon.h


LGBOOK_DEP = d:\virtua~2\lgbooks.src\lgbook\lgbook.h \
	d:\virtua~2\lgbooks.src\lgbook\lgbookid.h \
	d:\virtua~2\lgbooks.src\lgbook\toonctrl.h \
	d:\virtua~2\lgbooks.src\lgbook\toon.h \
	d:\virtua~2\lgbooks.src\lgbook\belongs.h \
	d:\virtua~2\lgbooks.src\lgbook\credits.h \
	d:\virtua~2\lgbooks.src\lgbook\options.h \
	d:\virtua~2\lgbooks.src\lgbook\colorbok.h \
	d:\virtua~2\lgbooks.src\lgbook\soundbox.h \
	d:\virtua~2\lgbooks.src\lgbook\lgbres.h


HOUSE_RCDEP = d:\virtua~2\lgbooks.src\lgbdemo\houseid.h


HOUSE_DEP = d:\virtua~2\lgbooks.src\lgbdemo\house.h \
	d:\virtua~2\lgbooks.src\lgbdemo\houseid.h \
	d:\virtua~2\lgbooks.src\lgbdemo\chase.h \
	d:\virtua~2\lgbooks.src\lgbdemo\chasedef.h \
	d:\virtua~2\lgbooks.src\lgbdemo\connect.h


CHASE_DEP = d:\virtua~2\lgbooks.src\lgbdemo\house.h \
	d:\virtua~2\lgbooks.src\lgbdemo\houseid.h \
	d:\virtua~2\lgbooks.src\lgbdemo\chase.h \
	d:\virtua~2\lgbooks.src\lgbdemo\chasedef.h


COLORBOK_DEP = d:\virtua~2\lgbooks.src\lgbook\colorbok.h \
	d:\virtua~2\lgbooks.src\lgbook\lgbook.h \
	d:\virtua~2\lgbooks.src\lgbook\lgbookid.h \
	d:\virtua~2\lgbooks.src\lgbook\toonctrl.h \
	d:\virtua~2\lgbooks.src\lgbook\toon.h \
	d:\virtua~2\lgbooks.src\lgbook\colordef.h


OPTIONS_DEP = d:\virtua~2\lgbooks.src\lgbook\options.h \
	d:\virtua~2\lgbooks.src\lgbook\lgbook.h \
	d:\virtua~2\lgbooks.src\lgbook\lgbookid.h \
	d:\virtua~2\lgbooks.src\lgbook\toonctrl.h \
	d:\virtua~2\lgbooks.src\lgbook\toon.h


SOUNDBOX_DEP = d:\virtua~2\lgbooks.src\lgbook\soundbox.h \
	d:\virtua~2\lgbooks.src\lgbook\lgbook.h \
	d:\virtua~2\lgbooks.src\lgbook\lgbookid.h \
	d:\virtua~2\lgbooks.src\lgbook\toonctrl.h \
	d:\virtua~2\lgbooks.src\lgbook\toon.h


CONNECT_DEP = d:\virtua~2\lgbooks.src\lgbdemo\connect.h \
	d:\virtua~2\lgbooks.src\lgbdemo\house.h \
	d:\virtua~2\lgbooks.src\lgbdemo\conndef.h \
	d:\virtua~2\lgbooks.src\lgbdemo\houseid.h


MIXUP_DEP = d:\virtua~2\lgbooks.src\lgbdemo\house.h \
	d:\virtua~2\lgbooks.src\lgbdemo\houseid.h


all:	$(PROJ).EXE $(PROJ).BSC

TOONCTRL.OBJ:	..\LGBOOK\TOONCTRL.CPP $(TOONCTRL_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c ..\LGBOOK\TOONCTRL.CPP

BELONGS.OBJ:	..\LGBOOK\BELONGS.CPP $(BELONGS_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c ..\LGBOOK\BELONGS.CPP

CREDITS.OBJ:	..\LGBOOK\CREDITS.CPP $(CREDITS_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c ..\LGBOOK\CREDITS.CPP

LGBOOK.OBJ:	..\LGBOOK\LGBOOK.CPP $(LGBOOK_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c ..\LGBOOK\LGBOOK.CPP

HOUSE.RES:	HOUSE.RC $(HOUSE_RCDEP)
	$(RC) $(RCFLAGS) $(RCDEFINES) -r HOUSE.RC

HOUSE.OBJ:	HOUSE.CPP $(HOUSE_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c HOUSE.CPP

CHASE.OBJ:	CHASE.CPP $(CHASE_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c CHASE.CPP

COLORBOK.OBJ:	..\LGBOOK\COLORBOK.CPP $(COLORBOK_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c ..\LGBOOK\COLORBOK.CPP

OPTIONS.OBJ:	..\LGBOOK\OPTIONS.CPP $(OPTIONS_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c ..\LGBOOK\OPTIONS.CPP

SOUNDBOX.OBJ:	..\LGBOOK\SOUNDBOX.CPP $(SOUNDBOX_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c ..\LGBOOK\SOUNDBOX.CPP

CONNECT.OBJ:	CONNECT.CPP $(CONNECT_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c CONNECT.CPP

MIXUP.OBJ:	MIXUP.CPP $(MIXUP_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c MIXUP.CPP


$(PROJ).EXE::	HOUSE.RES

$(PROJ).EXE::	TOONCTRL.OBJ BELONGS.OBJ CREDITS.OBJ LGBOOK.OBJ HOUSE.OBJ CHASE.OBJ \
	COLORBOK.OBJ OPTIONS.OBJ SOUNDBOX.OBJ CONNECT.OBJ MIXUP.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
TOONCTRL.OBJ +
BELONGS.OBJ +
CREDITS.OBJ +
LGBOOK.OBJ +
HOUSE.OBJ +
CHASE.OBJ +
COLORBOK.OBJ +
OPTIONS.OBJ +
SOUNDBOX.OBJ +
CONNECT.OBJ +
MIXUP.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
c:\msvc\lib\+
..\GAMEGUTS\WING.LIB+
..\GAMEGUTS\WINTOON.LIB+
..\GAMEGUTS\SLIBDIB.LIB+
..\GAMEGUTS\VFW.LIB+
..\GAMEGUTS\WAVMIX16.LIB+
..\GAMEGUTS\GAMEGUTS.LIB+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF
	$(RC) $(RESFLAGS) HOUSE.RES $@
	@copy $(PROJ).CRF MSVC.BND

$(PROJ).EXE::	HOUSE.RES
	if not exist MSVC.BND 	$(RC) $(RESFLAGS) HOUSE.RES $@

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
