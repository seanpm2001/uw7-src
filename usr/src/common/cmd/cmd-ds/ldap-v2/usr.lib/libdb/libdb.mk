#ident	"@(#)libdb.mk	1.5"
#ident	"$Header$"

include $(LIBRULES)

HDIR=		$(LDAPTOP)/include
LOCALINC=	-I$(HDIR) -I.

SUBDIRS=	btree db hash mpool recno 
SUBLIBS=	btree/btree.a db/db.a hash/hash.a mpool/mpool.a recno/recno.a
LIBRARY=	libdb.a
TESTS=		test/dbtest
TESTDIRS=	test

all: 		$(LIBRARY) $(TESTS)

sublibs:	
		@for i in $(SUBDIRS); do \
			cd $$i; \
			echo make $$i; \
			$(MAKE) -f $$i.mk $(MAKEARGS) all ; \
			cd ..; \
		done; \
		echo; 

$(LIBRARY): 	sublibs
	$(RM) -rf tmp
	mkdir tmp
	@for i in $(SUBLIBS); do \
		( \
		  cd tmp; \
		  $(AR) x ../$$i; \
		  pre=`echo $$i | sed -e 's/\/.*$$//' -e 's/back-//'`; \
		  for j in *.o; do \
			mv $$j $${pre}$$j; \
		  done; \
		  $(AR) ruv libdb.a *.o 2>&1 | grep -v truncated; \
		  $(RM) -f *.o __.SYMDEF; \
		  echo "added component library $$i"; \
		); \
	done
	@mv -f tmp/libdb.a ./libdb.a
	@$(RM) -r tmp
	@if [ ! -z "$(RANLIB)" ]; then \
		$(RANLIB) libdb.a; \
	fi
	$(RM) -f ../$@; \
	$(LN) -s libdb/$@ ../$@;

$(TESTS):	$(LIBRARY) test/dbtest.c
		@for i in $(TESTDIRS); do \
			cd $$i; \
			echo make $$i; \
			$(MAKE) -f $$i.mk $(MAKEARGS) all ; \
			cd ..; \
		done; \
		echo; 


install: all

clean:
	@for i in $(SUBDIRS) tools; do \
		if [ -d $$i ]; then \
		echo; echo "  cd $$i; $(MAKE) -f $$i.mk $(MAKEARGS) $@"; \
		( cd $$i; $(MAKE) -f $$i.mk $@ ); \
		fi; \
	done

clobber:	clean
	$(RM) -f libdb.a
	@for i in $(SUBDIRS) tools; do \
		if [ -d $$i ]; then \
		echo; echo "  cd $$i; $(MAKE) -f $$i.mk $(MAKEARGS) $@"; \
		( cd $$i; $(MAKE) -f $$i.mk $@ ); \
		fi; \
	done

lintit:
	@for i in $(SUBDIRS) tools ; do \
		if [ -d $$i ]; then \
		echo; echo "  cd $$i; $(MAKE) -f $$i.mk $(MAKEARGS) $@"; \
		( cd $$i; $(MAKE) -f $$i.mk $@ ); \
		fi; \
	done


FRC:

#
# Header dependencies
#
