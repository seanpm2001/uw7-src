#ident	"@(#)devintf:common/cmd/devintf/devices/reserve/reserve/reserve.mk	1.5.7.2"
#ident "$Header$"

include $(CMDRULES)

INSDIR		= $(USRSADM)/sysadm/menu/devices/devices/reserve/reserve
OWN=bin
GRP=bin

DESTDIR		= $(INSDIR)
HELPSRCDIR 	= .

SHFILES		= listavails
FMTFILES	= 
DISPFILES	= Form.reserve Text.c_rsv
HELPFILES	= Help

all: $(SHFILES) $(HELPFILES) $(FMTFILES) $(DISPFILES) 

$(SHFILES):

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
	for i in $(SHFILES) ;\
	do \
		$(INS) -f $(DESTDIR) -m 0750 -u $(OWN) -g $(GRP) $$i ;\
	done

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
