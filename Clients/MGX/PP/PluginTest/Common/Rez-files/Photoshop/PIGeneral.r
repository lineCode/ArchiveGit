//-------------------------------------------------------------------------------
//
//	File:
//		PIGeneral.r
//
//	Copyright 1991-1998, Adobe Systems Incorporated.
//	All Rights Reserved.
//
//	Distribution:
//		PUBLIC
//
//	Description:
//		This file is maintained for backwards compatibility
//		for Photoshop resource types.  It now includes three
//		other files:
//
//		PiPL.r			All PiPL template information for all
//						Adobe GAP applications.
//
//		PiMI.r			PiMI template information for creation
//						of Photoshop PiMI resources.
//
//		PIResDefines.h	Photoshop definitions and versions
//						for all Photoshop plug-in types.
//
//	Use:
//		This file must be included to correctly compile any
//		resource (.r) files that include application-specific
//		resources such as PiPLs, PiMIs, etc.
//
//	Version history:
//		Version 1.0.0	10/9/1997	Ace		Separated files.
//			Rolled out separate resource files and compiled for
//			Photoshop 5.0, After Effects 3.1, and Premiere 5.0.
//
//-------------------------------------------------------------------------------

#ifndef __PIGeneral_r__		// Only include this once.
#define __PIGeneral_r__

//-------------------------------------------------------------------------------
//	Includes -- Include required resource files.
//-------------------------------------------------------------------------------

#include "PIResDefines.h"	// Defines plug-in versions.
#include "PIPL.r"			// PiPL resource template.
#include "PIMI.r"			// PiMI resource template.

//-------------------------------------------------------------------------------

#endif	// __PIGeneral_r__
