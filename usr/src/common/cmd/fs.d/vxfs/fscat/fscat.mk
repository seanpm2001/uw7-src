# @(#)src/uw/cmd/vxfs/fscat/fscat.mk	3.7.9.1 11/08/97 12:52:48 - 
#ident	"@(#)vxfs:src/uw/cmd/vxfs/fscat/fscat.mk	3.7.9.1"
#ident	"@(#)vxfs.cmds:common/cmd/fs.d/vxfs/fscat/fscat.mk	1.1.2.1"
# Copyright (c) 1996 VERITAS Software Corporation.  ALL RIGHTS RESERVED.
# UNPUBLISHED -- RIGHTS RESERVED UNDER THE COPYRIGHT
# LAWS OF THE UNITED STATES.  USE OF A COPYRIGHT NOTICE
# IS PRECAUTIONARY ONLY AND DOES NOT IMPLY PUBLICATION
# OR DISCLOSURE.
#
# THIS SOFTWARE CONTAINS CONFIDENTIAL INFORMATION AND
# TRADE SECRETS OF VERITAS SOFTWARE.  USE, DISCLOSURE,
# OR REPRODUCTION IS PROHIBITED WITHOUT THE PRIOR
# EXPRESS WRITTEN PERMISSION OF VERITAS SOFTWARE.
#
#	       RESTRICTED RIGHTS LEGEND
# USE, DUPLICATION, OR DISCLOSURE BY THE GOVERNMENT IS
# SUBJECT TO RESTRICTIONS AS SET FORTH IN SUBPARAGRAPH
# (C) (1) (ii) OF THE RIGHTS IN TECHNICAL DATA AND
# COMPUTER SOFTWARE CLAUSE AT DFARS 252.227-7013.
#	       VERITAS SOFTWARE
# 1600 PLYMOUTH STREET, MOUNTAIN VIEW, CA 94043

include $(CMDRULES)

LIBDIR	= ../libvxfs
LDLIBS = -L$(LIBDIR) -lvxfsi -lgen
LOCALINC = -I../include
LOCALDEF = $(LARGEFILES) -DVXI18N

INSDIR1 = $(USRLIB)/fs/vxfs
OWN = bin
GRP = bin

OBJECTS = fscat.o machdep.o
PROBEFILE = fscat.c
MAKEFILE = fscat.mk
BINARIES = fscat

all:
	@if [ -f $(PROBEFILE) ]; then \
		$(MAKE) -f $(MAKEFILE) binaries $(MAKEARGS) ;\
	else \
		for fl in $(BINARIES); do \
			if [ ! -f $$fl ]; then \
				echo "ERROR: $$fl is missing" 1>&2 ;\
				false ;\
				break ;\
			fi \
		done \
	fi

install: all
	[ -d $(INSDIR1) ] || mkdir -p $(INSDIR1)
	$(INS) -f $(INSDIR1) -m 0555 -u $(OWN) -g $(GRP) fscat

clean:
	-rm -f $(OBJECTS)

clobber: clean
	@if [ -f $(PROBEFILE) ]; then \
		echo "rm -f $(BINARIES)" ;\
		rm -f $(BINARIES) ;\
	fi

binaries: $(BINARIES)


fscat: $(OBJECTS) $(LIBDIR)/libvxfsi.a
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS) $(ROOTLIBS)

fscat.o: fscat.c fscat.h

machdep.o: machdep.c fscat.h
