#ifndef NOIDENT
#ident	"@(#)olbitmaps:bitmaps/mediumOut.h	1.2"
#endif

#define mediumOut_width 9
#define mediumOut_height 8

#ifdef INACTIVE_CURSOR
static unsigned char mediumOut_bits[] = {
   0x00, 0x00,
   0x00, 0x00,
   0x00, 0x00,
   0x10, 0x00, 
   0x28, 0x00, 
   0x54, 0x00, 
   0x28, 0x00, 
   0x10, 0x00
};
#else
static unsigned char mediumOut_bits[] = {
   0x00, 0x00, 
   0x00, 0x00, 
   0x00, 0x00, 
   0x00, 0x00, 
   0x00, 0x00, 
   0x00, 0x00, 
   0x00, 0x00, 
   0x00, 0x00, 
};
#endif
