/*
 * @(#) dfbData.c 11.1 97/10/22
 *
 * Copyright (C) 1991-1993 The Santa Cruz Operation, Inc.
 *
 * The information in this file is provided for the exclusive use of the
 * licensees of The Santa Cruz Operation, Inc.  Such users have the right 
 * to use, modify, and incorporate this code into other products for purposes 
 * authorized by the license agreement provided they include this notice 
 * and the associated copyright notice with any such product.  The 
 * information in this file is provided "AS IS" without warranty.
 * 
 */
/*
 * dfbData.c
 *
 * dfb data structures.
 */

#include "Xproto.h"
#include "scrnintstr.h"

#include "scoext.h"
#include "ddxScreen.h"

#include "dfbProcs.h"

ddxScreenInfo dfbScreenInfo = {
    dfbProbe,		/* Bool (*screenProbe)()	*/
    dfbInit,		/* Bool (*screenInit)()		*/
    "dfb",		/* char *screenName		*/
};

scoScreenInfo dfbSysInfo = {
    NULL,		/* ScreenPtr pScreen		*/
    dfbSetGraphics,	/* void (*SetGraphics)()	*/
    dfbSetText,		/* void (*SetText)()		*/
    (void (*)())NoopDDA,	/* void (*SaveGState)() */
    (void (*)())NoopDDA,        /* void (*RestoreGState)() */
    dfbFreeScreen,	/* void (*FreeScreen)()		*/
    TRUE,		/* Bool exposeScreen		*/
    TRUE,		/* Bool isConsole		*/
    TRUE,		/* Bool runSwitched		*/
    XSCO_VERSION,	/* float version		*/
};
