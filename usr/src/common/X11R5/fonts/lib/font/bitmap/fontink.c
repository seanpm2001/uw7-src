#ident	"@(#)r5fonts:lib/font/bitmap/fontink.c	1.1"

/*
 * $XConsortium: fontink.c,v 1.2 91/05/10 15:58:28 keith Exp $
 *
 * Copyright 1990 Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * M.I.T. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL M.I.T.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Keith Packard, MIT X Consortium
 */

#include "fontfilest.h"

static unsigned char ink_mask_msb[8] = {
    0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01,
};

static unsigned char ink_mask_lsb[8] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
};

FontCharInkMetrics(pFont, pCI, pInk)
    FontPtr     pFont;
    CharInfoPtr pCI;
    xCharInfo  *pInk;
{
    int         leftBearing,
                ascent,
                descent;
    register int vpos,
                hpos,
                bpos;
    int         bitmapByteWidth,
                bitmapByteWidthPadded;
    int         bitmapBitWidth;
    int         span;
    register unsigned char *p;
    unsigned char *ink_mask;
    register int bmax;
    register unsigned char charbits;

    if (pFont->bit == MSBFirst)
	ink_mask = ink_mask_msb;
    else if (pFont->bit == LSBFirst)
	ink_mask = ink_mask_lsb;
    pInk->characterWidth = pCI->metrics.characterWidth;
    pInk->attributes = pCI->metrics.attributes;

    leftBearing = pCI->metrics.leftSideBearing;
    ascent = pCI->metrics.ascent;
    descent = pCI->metrics.descent;
    bitmapBitWidth = GLYPHWIDTHPIXELS(pCI);
    bitmapByteWidth = GLYPHWIDTHBYTES(pCI);
    bitmapByteWidthPadded = BYTES_PER_ROW(bitmapBitWidth, pFont->glyph);
    span = bitmapByteWidthPadded - bitmapByteWidth;

    p = (unsigned char *) pCI->bits;
    for (vpos = descent + ascent; --vpos >= 0;) {
	for (hpos = bitmapByteWidth; --hpos >= 0;) {
	    if (*p++ != 0)
		goto found_ascent;
	}
	p += span;
    }
    /*
     * special case -- font with no bits gets all zeros
     */
    pInk->leftSideBearing = leftBearing;
    pInk->rightSideBearing = leftBearing;
    pInk->ascent = 0;
    pInk->descent = 0;
    return;
found_ascent:
    pInk->ascent = vpos - descent + 1;

    p = ((unsigned char *) pCI->bits) + bitmapByteWidthPadded * 
	(descent + ascent - 1) + bitmapByteWidth;

    for (vpos = descent + ascent; --vpos >= 0;) {
	for (hpos = bitmapByteWidth; --hpos >= 0;) {
	    if (*--p != 0)
		goto found_descent;
	}
	p -= span;
    }
found_descent:
    pInk->descent = vpos - ascent + 1;

    bmax = 8;
    for (hpos = 0; hpos < bitmapByteWidth; hpos++) {
	charbits = 0;
	p = (unsigned char *) pCI->bits + hpos;
	for (vpos = descent + ascent; --vpos >= 0; p += bitmapByteWidthPadded)
	    charbits |= *p;
	if (charbits) {
	    if (hpos == bitmapByteWidth - 1)
		bmax = bitmapBitWidth - (hpos << 3);
	    p = ink_mask;
	    for (bpos = bmax; --bpos >= 0;) {
		if (charbits & *p++)
		    goto found_left;
	    }
	}
    }
found_left:
    pInk->leftSideBearing = leftBearing + (hpos << 3) + bmax - bpos - 1;

    bmax = bitmapBitWidth - ((bitmapByteWidth - 1) << 3);
    for (hpos = bitmapByteWidth; --hpos >= 0;) {
	charbits = 0;
	p = (unsigned char *) pCI->bits + hpos;
	for (vpos = descent + ascent; --vpos >= 0; p += bitmapByteWidthPadded)
	    charbits |= *p;
	if (charbits) {
	    p = ink_mask + bmax;
	    for (bpos = bmax; --bpos >= 0;) {
		if (charbits & *--p)
		    goto found_right;
	    }
	}
	bmax = 8;
    }
found_right:
    pInk->rightSideBearing = leftBearing + (hpos << 3) + bpos + 1;
}

#define	ISBITONMSB(x, line)	((line)[(x)/8] & (1 << (7-((x)%8))))
#define	SETBITMSB(x, line)	((line)[(x)/8] |= (1 << (7-((x)%8))))
#define	ISBITONLSB(x, line)	((line)[(x)/8] & (1 << ((x)%8)))
#define	SETBITLSB(x, line)	((line)[(x)/8] |= (1 << ((x)%8)))

#define Min(a,b)    ((a)<(b)?(a):(b))
#define Max(a,b)    ((a)>(b)?(a):(b))

FontCharReshape(pFont, pSrc, pDst)
    FontPtr     pFont;
    CharInfoPtr pSrc,
                pDst;
{
    int         x,
                y;
    unsigned char *in_line,
               *out_line;
    unsigned char *oldglyph,
               *newglyph;
    int         inwidth;
    int         outwidth,
                outheight;
    int         out_bytes,
                in_bytes;
    int         y_min,
                y_max,
                x_min,
                x_max;

    newglyph = (unsigned char *) pDst->bits;
    outwidth = pDst->metrics.rightSideBearing - pDst->metrics.leftSideBearing;
    outheight = pDst->metrics.descent + pDst->metrics.ascent;
    out_bytes = BYTES_PER_ROW(outwidth, pFont->glyph);

    oldglyph = (unsigned char *) pSrc->bits;
    inwidth = pSrc->metrics.rightSideBearing - pSrc->metrics.leftSideBearing;
    in_bytes = BYTES_PER_ROW(inwidth, pFont->glyph);

    bzero(newglyph, out_bytes * outheight);
    in_line = oldglyph;
    out_line = newglyph;
    y_min = Max(-pSrc->metrics.ascent, -pDst->metrics.ascent);
    y_max = Min(pSrc->metrics.descent, pDst->metrics.descent);
    x_min = Max(pSrc->metrics.leftSideBearing, pDst->metrics.leftSideBearing);
    x_max = Min(pSrc->metrics.rightSideBearing, pDst->metrics.rightSideBearing);
    in_line += (y_min + pSrc->metrics.ascent) * in_bytes;
    out_line += (y_min + pDst->metrics.ascent) * out_bytes;
    if (pFont->bit == MSBFirst) {
	for (y = y_min; y < y_max; y++) {
	    for (x = x_min; x < x_max; x++) {
		if (ISBITONMSB(x - pSrc->metrics.leftSideBearing, in_line))
		    SETBITMSB(x - pDst->metrics.leftSideBearing, out_line);
	    }
	    in_line += in_bytes;
	    out_line += out_bytes;
	}
    } else {
	for (y = y_min; y < y_max; y++) {
	    for (x = x_min; x < x_max; x++) {
		if (ISBITONLSB(x - pSrc->metrics.leftSideBearing, in_line))
		    SETBITLSB(x - pDst->metrics.leftSideBearing, out_line);
	    }
	    in_line += in_bytes;
	    out_line += out_bytes;
	}
    }
    return;
}
