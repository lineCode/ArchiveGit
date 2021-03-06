/****************************************************************************
 *                   Copyright  1995  INSO Corporation                      *
 *                International CorrectSpell[TM] Software                   *
 *                                                                          *
 * This work is protected by United States and international copyright laws *
 * and contains certain trade secret information.  All use of this work is  *
 * governed by the terms of agreements between INSO Corporation and its     *
 * licensees or potential licensees.  Disclosure or disassembly of the      *
 * contents of embodied programs or algorithms is prohibited.  All rights   *
 * are reserved worldwide.                                                  *
 ****************************************************************************
*/

#ifndef _CMSTSTRU_DEF
#define _CMSTSTRU_DEF

typedef struct STRINGSTR {
        UCHAR    A[STRINGMAX+1];
} stringstr;

typedef struct STRINGTYPE {
        stringlen       len;
        stringstr       str;
} stringtype;

#endif
