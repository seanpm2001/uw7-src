#ident	"@(#)umount.mk	1.3"
#ident	"$Header$"

include $(CMDRULES)

BINS= umount
OBJS= umount.o mountxdr.o bindresvport.o 
SRCS= $(OBJS:.o=.c)
INCSYS = $(INC)
INSDIR = $(USRLIB)/fs/nfs
OWN = bin
GRP = bin

LINTFLAGS= -hbax $(DEFLIST)
COFFLIBS= -lsocket -lnsl_s -lgen
ELFLIBS = -lsocket -lnsl -lgen
LDLIBS=`if [ x$(CCSTYPE) = xCOFF ] ; then echo "$(COFFLIBS)" ; else echo "$(ELFLIBS)" ; fi`

all: $(BINS)

$(BINS): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS) $(SHLIBS)

install: all
	[ -d $(INSDIR) ] || mkdir -p $(INSDIR)
	$(INS) -f $(INSDIR) -m 0555 -u $(OWN) -g $(GRP) $(BINS)

lintit:
	$(LINT) $(LINTFLAGS) $(SRCS)

tags: $(SRCS)
	ctags $(SRCS)

clean:
	-rm -f $(OBJS)

clobber: clean
	-rm -f $(BINS)
