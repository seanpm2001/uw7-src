#!/bin/sh

# $XConsortium: cvga256.sh /main/1 1995/09/04 19:45:10 kaleb $
# $XFree86: xc/programs/Xserver/hw/xfree86/vga256/cvga256.sh,v 3.0 1995/03/11 14:50:00 dawes Exp $
#
# This script generates vga256Conf.c
#
# usage: confvga256.sh driver1 driver2 ...
#

VGACONF=./vga256Conf.c

cat > $VGACONF <<EOF
/*
 * This file is generated automatically -- DO NOT EDIT
 */

#include "xf86.h"
#include "vga.h"

extern vgaVideoChipRec
EOF
Args="`echo $* | tr '[a-z]' '[A-Z]'`"
set - $Args
while [ $# -gt 1 ]; do
  echo "        $1," >> $VGACONF
  shift
done
echo "        $1;" >> $VGACONF
cat >> $VGACONF <<EOF

vgaVideoChipPtr vgaDrivers[] =
{
EOF
for i in $Args; do
  echo "        &$i," >> $VGACONF
done
echo "        NULL" >> $VGACONF
echo "};" >> $VGACONF
