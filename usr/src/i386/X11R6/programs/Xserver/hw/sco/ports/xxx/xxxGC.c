/*
 * @(#) xxxGC.c 11.1 97/10/22
 *
 * Copyright (C) 1991-1994 The Santa Cruz Operation, Inc.
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
 * xxxGC.c
 *
 * Template for machine dependent ValidateWindowGC() routine
 */

#include "X.h"
#include "Xproto.h"
#include "pixmapstr.h"
#include "gcstruct.h"
#include "scrnintstr.h"
#include "colormapst.h"
#include "regionstr.h"

#include "ddxScreen.h"

#include "nfb/nfbDefs.h"
#include "nfb/nfbGCStr.h"
#include "nfb/nfbWinStr.h"

#include "xxxProcs.h"

extern nfbGCOps xxxSolidPrivOps, xxxTiledPrivOps,
		xxxStippledPrivOps, xxxOpStippledPrivOps;

/*
 * xxxValidateWindowGC() - set GC ops and privates based on GC values.
 *
 * Just set the nfb GC private ops here;
 * nfbHelpValidateGC() will do most of the work.
 */
void
xxxValidateWindowGC(
	GCPtr pGC,
	Mask changes,
	DrawablePtr pDraw)
{
	nfbGCPrivPtr pPriv = NFB_GC_PRIV(pGC);

	/* set the private ops based on fill style */
	if (changes & GCFillStyle)
		switch (pGC->fillStyle) {
		    case FillSolid:
			pPriv->ops = &xxxSolidPrivOps;
			break;
		    case FillTiled:
			pPriv->ops = &xxxTiledPrivOps;
			break;
		    case FillStippled:
			pPriv->ops = &xxxStippledPrivOps;
			break;
		    case FillOpaqueStippled:
			if (pGC->fgPixel == pGC->bgPixel)
			    pPriv->ops = &xxxSolidPrivOps;
			else
			    pPriv->ops = &xxxOpStippledPrivOps;
			break;
		    default:
			FatalError("xxxValidateWindowGC: illegal fillStyle\n");
		}

	/* let nfb do the rest */
	nfbHelpValidateGC(pGC, changes, pDraw);
}
