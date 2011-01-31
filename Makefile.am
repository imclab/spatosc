# "foreign" means not GNU, and is common for avoiding boring messages about files organized differently from what gnu expects
ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = foreign

SUBDIRS = src tests build

EXTRA_DIST = \
    LICENSE \
    NEWS \
    README \
    autogen.sh \
    configure.ac \
    doxyfile.in \
    spatosc.pc.in

DISTCLEANFILES = spatosc.pc

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = spatosc-@SPATOSC_API_VERSION@.pc

dist_noinst_SCRIPTS = autogen.sh

html-local: doxygen
doxygen:
	@if [ -z `which doxygen` ]; then echo "No Doxygen!"; else doxygen; fi
clean-local:
	rm -rf html
