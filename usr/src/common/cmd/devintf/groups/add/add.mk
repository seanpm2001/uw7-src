#ident	"@(#)devintf:common/cmd/devintf/groups/add/add.mk	1.8.7.2"
#ident "$Header$"

include $(CMDRULES)

INSDIR		= $(USRSADM)/sysadm/menu/devices/groups/add
OWN=bin
GRP=bin

DESTDIR		= $(INSDIR)
HELPSRCDIR 	= .

SHFILES		=
FMTFILES	= 
DISPFILES	= Form.add Text.c_add
HELPFILES	= Help

all: $(SHFILES) $(HELPFILES) $(FMTFILES) $(DISPFILES) 

# $(SHFILES):

# $(FMTFILES):

$(HELPFILES):

$(DISPFILES):

clean:

clobber: clean

size:

strip:

lintit:

install: $(DESTDIR) all
	for i in $(DISPFILES) ;\
	do \
		$(INS) -f $(DESTDIR) -m 0640 -u $(OWN) -g $(GRP) $$i ;\
	done
#	for i in $(FMTFILES) ;\
#	do \
#		$(INS) -m 640 -g bin -u bin -f $(DESTDIR) $$i ;\
#	done
	for i in $(HELPFILES) ;\
	do \
		$(INS) -f $(DESTDIR) -m 0640 -u $(OWN) -g $(GRP) $(HELPSRCDIR)/$$i ;\
	done
#	for i in $(SHFILES) ;\
#	do \
#		$(INS) -m 750 -g bin -u bin -f $(DESTDIR) $$i ;\
#	done

$(DESTDIR):
	builddir() \
	{ \
		if [ ! -d $$1 ]; \
		then \
		    builddir `dirname $$1`; \
		    mkdir $$1; \
		fi \
	}; \
	builddir $(DESTDIR)
