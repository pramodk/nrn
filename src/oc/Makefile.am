## Basic HOC library:
lib_LTLIBRARIES = liboc.la libocxt.la

nsrc=$(top_srcdir)/src
IV_INCLUDES = @IV_INCLUDE@ $(X_CFLAGS)
includes = -I$(nsrc) -I$(nsrc)/memacs -I$(nsrc)/sparse -I$(nsrc)/nrnmpi -I$(nsrc)/nrnoc $(IV_INCLUDES)

AM_CPPFLAGS = -DOOP=1 -DINTERVIEWS=1 -DHOC=1 $(includes)

RanFiles = isaac64.cpp mcran4.cpp nrnisaac.cpp nrnran123.cpp

if BUILD_NRNMPI_DYNAMIC
nrnmpila =
nrnmpidynam = nrnmpi_dynam.cpp
else
nrnmpila = ../nrnmpi/libnrnmpi.la
nrnmpidynam =
endif

liboc_la_SOURCES = parse.ypp bksub.cpp getelm.cpp lineq.cpp subrows.cpp prmat.cpp \
	code2.cpp debug.cpp ocerf.cpp fileio.cpp ftime.cpp \
	getsym.cpp hoc.cpp hocedit.cpp math.cpp nonlin.cpp \
	list.cpp regexp.cpp audit.cpp symbol.cpp fmenu.cpp \
	version.cpp xred.cpp parallel.cpp functabl.cpp plot.cpp plt.cpp axis.cpp \
	settext.cpp x.cpp  termio.cpp isoc99.cpp $(nrnmpidynam)\
	scoprand.cpp $(RanFiles) cygwinprt.cpp nrnfilewrap.cpp

## MAC OS X does not allow multiple definitions in separate dyld files
## therefore we create a separate library for the oc objects that
## should appear in oc and ivoc but not nrnoc and nrniv
## modlreg.cpp is another special case since it should be missing only
## in a special so we will put it explicitly into nrnoc and nrniv
libocxt_la_SOURCES = code.cpp hoc_init.cpp hoc_oop.cpp \
	spinit.cpp spinit1.cpp spinit2.cpp

AM_YFLAGS = -d

EXTRA_DIST = mk_hocusr_h.py hocusr.cpp macprt.cpp mswinprt.cpp system.cpp \
	$(nrnmpidynam)

if BUILD_MINGW
liboc_la_LIBADD = -lpthread
else
liboc_la_LIBADD = $(X_LIBS)
endif

bin_PROGRAMS = oc

## modlreg.cpp is here because the newer libtool no longer
## always makes a .o link to the .lo file
oc_SOURCES = ocmain.cpp ocnoiv.cpp nocable.cpp modlreg.cpp ockludge.cpp

oc_LDADD = libocxt.la liboc.la \
	$(nrnmpila) \
	@MEMACSLIBLA@ @NRN_READLINE_LIBS@

## hocdec now primary and hoc.h includes it
##hocdec.h: hoc.h hoc2dec.sh
##	./hoc2dec.sh
	
## Header files that need to be installed:
pkginclude_HEADERS = hoc.h hoclist.h hocdec.h hocassrt.h nrnisaac.h \
	oc_ansi.h hocparse.h ocfunc.h ocmisc.h nrnjava.h hocgetsym.h \
	nrnpthread.h hoc_membf.h nrnran123.h scoplib_ansi.h nrnapi.h \
	nrnassrt.h

## Header files that need to be copied into the distribution but do not need
## to be installed:
noinst_HEADERS = code.h equation.h hocmodl.h hocstr.h \
	profile.h redef.h parse.hpp mcran4.h nrnmpi.h nrnrt.h isaac64.h \
	nrnfilewrap.h nrn_vsscanf.cpp classreg.h

## files that depend on the existence of parse.hpp
## this allows parallel make -j to work at the beginning when parse.hpp
## does not exist and dependencies have not yet been determined
## I am not sure whether both .lo and .o lists are necessary.
## Also, oc/parse.hpp is included in files in the nrnoc, ivoc, and
## nrniv directories so we hope that parse.hpp is created before
## make -v gets into those.

code2.lo code.lo fileio.lo getsym.lo hoc.lo hoc_init.lo hoc_oop.lo \
list.lo nonlin.lo parallel.lo symbol.lo: parse.hpp

code2.o code.o fileio.o getsym.o hoc.o hoc_init.o hoc_oop.o \
list.o nonlin.o parallel.o symbol.o: parse.hpp

