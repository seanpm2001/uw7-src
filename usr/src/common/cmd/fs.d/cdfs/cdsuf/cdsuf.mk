#	Copyright (c) 1991, 1992  Intel Corporation
#	All Rights Reserved
#
#	INTEL CORPORATION CONFIDENTIAL INFORMATION
#
#	This software is supplied to USL under the terms of a license 
#	agreement with Intel Corporation and may not be copied nor 
#	disclosed except in accordance with the terms of that agreement.

#ident	"@(#)cdfs.cmds:common/cmd/fs.d/cdfs/cdsuf/cdsuf.mk	1.5.1.1"
#ident	"$Header$"

# Tabstops: 4


include		$(CMDRULES)

INSDIR		= $(USRLIB)/fs/cdfs
OWN			= bin
GRP			= bin

LDLIBS		= -lcdfs -lgen
LOCALDEF        = -D_KMEMUSER


all:	cdsuf

cdsuf:	cdsuf.o
	$(CC) $(LDFLAGS) -o $@ $@.o $(LDLIBS)

install:	all
	[ -d $(INSDIR) ] || mkdir -p $(INSDIR)
	$(INS) -f $(INSDIR) -m 0555 -u $(OWN) -g $(GRP) cdsuf

headinstall:

lintit:
	$(LINT) $(LINTFLAGS) $(DEFLIST) cdsuf.c

clean:
	-rm -f *.o
	-rm -f *.ln

clobber:	clean
	rm -f cdsuf

cdsuf.o:	cdsuf.c \
	cdsuf.h \
	$(INC)/errno.h \
	$(INC)/libgen.h \
	$(INC)/locale.h \
	$(INC)/pfmt.h \
	$(INC)/stdio.h \
	$(INC)/stdlib.h \
	$(INC)/string.h \
	$(INC)/sys/cdrom.h \
	$(INC)/sys/param.h \
	$(INC)/sys/types.h \
	$(INC)/sys/fs/cdfs_fs.h \
	$(INC)/sys/fs/cdfs_ioctl.h \
	$(INC)/sys/fs/iso9660.h

