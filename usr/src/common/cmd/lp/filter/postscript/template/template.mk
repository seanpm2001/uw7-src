#ident	"@(#)template.mk	1.2"
#ident	"$Header$"
#
# template makefile - change all occurrences of template (TEMPLATE) to the name
# (NAME) of your program.
#

include $(CMDRULES)

MAKEFILE=template.mk
ARGS=all

#
# Common header and source files have been moved to $(COMMONDIR).
#

COMMONDIR=../common

LOCALDEF= -DSYSV
LOCALINC= -I$(COMMONDIR)

CFILES=template.c \
       $(COMMONDIR)/request.c \
       $(COMMONDIR)/glob.c \
       $(COMMONDIR)/misc.c

HFILES=template.h \
       $(COMMONDIR)/comments.h \
       $(COMMONDIR)/ext.h \
       $(COMMONDIR)/gen.h \
       $(COMMONDIR)/path.h

TEMPLATE=template.o\
       $(COMMONDIR)/request.o\
       $(COMMONDIR)/glob.o\
       $(COMMONDIR)/misc.o

ALLFILES=README $(MAKEFILE) $(HFILES) $(CFILES)


all : template

install : template
	@if [ ! -d "$(BINDIR)" ]; then \
	    mkdir $(BINDIR); \
	    $(CH)chmod 775 $(BINDIR); \
	    $(CH)chgrp $(GROUP) $(BINDIR); \
	    $(CH)chown $(OWNER) $(BINDIR); \
	fi
	$(INS) -m 775 -u $(OWNER) -g $(GROUP) -f $(BINDIR) template

template : $(TEMPLATE)
	$(CC) -o template $(TEMPLATE) $(LDFLAGS) $(SHLIBS)

$(COMMONDIR)/glob.o : $(COMMONDIR)/glob.c $(COMMONDIR)/gen.h
	cd $(COMMONDIR); $(CC) $(CFLAGS) $(DEFLIST) -c glob.c

$(COMMONDIR)/misc.o : $(COMMONDIR)/misc.c $(COMMONDIR)/ext.h $(COMMONDIR)/gen.h
	cd $(COMMONDIR); $(CC) $(CFLAGS) $(DEFLIST) -c misc.c

$(COMMONDIR)/request.o : $(COMMONDIR)/request.c $(COMMONDIR)/ext.h $(COMMONDIR)/gen.h $(COMMONDIR)/path.h
	cd $(COMMONDIR); $(CC) $(CFLAGS) $(DEFLIST) -c request.c

template.o : $(HFILES)

clean :
	rm -f $(TEMPLATE)

clobber : clean
	rm -f template

list :
	pr -n $(ALLFILES) | $(LIST)

lintit:
