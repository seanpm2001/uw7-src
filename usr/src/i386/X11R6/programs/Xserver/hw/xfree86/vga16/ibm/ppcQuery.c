/* $XFree86: $ */
/*
 * Copyright IBM Corporation 1987,1988,1989
 *
 * All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that 
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of IBM not be
 * used in advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.
 *
 * IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
 * IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 *
*/
/* $XConsortium: ppcQuery.c /main/2 1995/11/13 07:05:37 kaleb $ */

#include "X.h"
#include "Xproto.h"

void
ppcQueryBestSize( class, pwidth, pheight )
register int class ;
register short *pwidth ;
register short *pheight ;
{
if ( class == CursorShape )
  *pwidth = *pheight = 32 ; /* ppc's cursor max out at 32 by 32 */
else /* either TileShape or StippleShape */
  /* Round Up To Nearest Multiple Of 8 -- We don't care what height they use */
  *pwidth = ( *pwidth + 0x7 ) & ~ 0x7 ;

return ;
}
