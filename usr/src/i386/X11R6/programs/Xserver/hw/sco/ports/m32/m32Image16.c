/*
 * @(#) m32Image16.c 11.1 97/10/22
 *
 * Copyright (C) The Santa Cruz Operation, 1993.
 * This Module contains Proprietary Information of
 * The Santa Cruz Operation, and should be treated as Confidential.
 */
/*
 * SCO MODIFICATION HISTORY
 *
 * S000, 27-Jul-93, buckm
 *	Created.
 */
/*
 * m32Image.c
 *
 * Mach-32 ReadImage and DrawImage routines
 */

#include "X.h"
#include "miscstruct.h"
#include "scrnintstr.h"
#include "windowstr.h"

#include "nfb/nfbDefs.h"
#include "nfb/nfbWinStr.h"
#include "nfb/nfbScrStr.h"

#include "m32Defs.h"
#include "m32ScrStr.h"

/*
 * m32ReadImage16() - Read a rectangular area of a window into image.
 *	pbox - the rectangle to read.
 *	image - where to write the pixels.  pack 16-bit pixels one per short.
 *	stride - the number of bytes from the start of one scanline
 *		 to the next within image.
 *	pDraw - the window from which to read.
 */
void 
m32ReadImage16(pbox, image, stride, pDraw)
	BoxPtr pbox;
	unsigned char *image;
	unsigned int stride;
	DrawablePtr pDraw;
{
	int w, h;
	M32_DBG_NAME("ReadImage16");

	w = pbox->x2 - pbox->x1;
	h = pbox->y2 - pbox->y1;

	M32_CLEAR_QUEUE(8);
	outw(M32_DP_CONFIG,	M32_DP_RWHOST);
	outw(M32_ALU_FG_FN,	m32RasterOp[GXcopy]);
	outw(M32_WRT_MASK,	0xFFFF);
	outw(M32_CUR_X,		pbox->x1);
	outw(M32_CUR_Y,		pbox->y1);
	outw(M32_DEST_X_START,	pbox->x1);
	outw(M32_DEST_X_END,	pbox->x2);
	outw(M32_DEST_Y_END,	pbox->y2);

	M32_DATA_READY();

	while (--h >= 0) {
		int n = w;
		unsigned short *p = (unsigned short *)image;

		while ((n -= 8) >= 0) {
			p[0] = inw(M32_PIX_TRANS);
			p[1] = inw(M32_PIX_TRANS);
			p[2] = inw(M32_PIX_TRANS);
			p[3] = inw(M32_PIX_TRANS);
			p[4] = inw(M32_PIX_TRANS);
			p[5] = inw(M32_PIX_TRANS);
			p[6] = inw(M32_PIX_TRANS);
			p[7] = inw(M32_PIX_TRANS);
			p += 8;
		}
		if (n += 8) {
			while (--n >= 0)
				*p++ = inw(M32_PIX_TRANS);
		}
		image += stride;
	}
}

/*
 * m32DrawImage16() - Draw pixels in a rectangular area of a window.
 *	pbox - the rectangle to draw into.
 *	image - the pixels to draw.
 *	stride - the number of bytes from the start of one scanline
 *		 to the next within image.
 *	alu - the raster op to use when drawing.
 *	planemask - the window planes to be affected.
 *	pDraw - the window to draw to.
 */
void 
m32DrawImage16(pbox, image, stride, alu, planemask, pDraw)
	BoxPtr pbox;
	unsigned char *image;
	unsigned int stride;
	unsigned char alu;
	unsigned long planemask;
	DrawablePtr pDraw;
{
	int w, h;

	w = pbox->x2 - pbox->x1;
	h = pbox->y2 - pbox->y1;

	M32_CLEAR_QUEUE(8);
	outw(M32_DP_CONFIG,	M32_DP_WWHOST);
	outw(M32_ALU_FG_FN,	m32RasterOp[alu]);
	outw(M32_WRT_MASK,	planemask);
	outw(M32_CUR_X,		pbox->x1);
	outw(M32_CUR_Y,		pbox->y1);
	outw(M32_DEST_X_START,	pbox->x1);
	outw(M32_DEST_X_END,	pbox->x2);
	outw(M32_DEST_Y_END,	pbox->y2);

	if ((w * 2) == stride) {
		int n = w * h;
		unsigned short *p = (unsigned short *)image;

		while ((n -= 8) >= 0) {
			M32_CLEAR_QUEUE(8);
			outw(M32_PIX_TRANS, p[0]);
			outw(M32_PIX_TRANS, p[1]);
			outw(M32_PIX_TRANS, p[2]);
			outw(M32_PIX_TRANS, p[3]);
			outw(M32_PIX_TRANS, p[4]);
			outw(M32_PIX_TRANS, p[5]);
			outw(M32_PIX_TRANS, p[6]);
			outw(M32_PIX_TRANS, p[7]);
			p += 8;
		}
		if (n += 8) {
			M32_CLEAR_QUEUE(n);
			while (--n >= 0)
				outw(M32_PIX_TRANS, *p++);
		}
	} else {
		while (--h >= 0) {
			int n = w;
			unsigned short *p = (unsigned short *)image;

			while ((n -= 8) >= 0) {
				M32_CLEAR_QUEUE(8);
				outw(M32_PIX_TRANS, p[0]);
				outw(M32_PIX_TRANS, p[1]);
				outw(M32_PIX_TRANS, p[2]);
				outw(M32_PIX_TRANS, p[3]);
				outw(M32_PIX_TRANS, p[4]);
				outw(M32_PIX_TRANS, p[5]);
				outw(M32_PIX_TRANS, p[6]);
				outw(M32_PIX_TRANS, p[7]);
				p += 8;
			}
			if (n += 8) {
				M32_CLEAR_QUEUE(n);
				while (--n >= 0)
					outw(M32_PIX_TRANS, *p++);
			}
			image += stride;
		}
	}
}
