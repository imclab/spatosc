# "foreign" means not GNU, and is common for avoiding boring messages about files organized differently from what gnu expects
AUTOMAKE_OPTIONS = foreign
# ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src

EXTRA_DIST = README LICENSE doxyfile.in spatosc.pc.in autogen.sh NEWS
DISTCLEANFILES = spatosc.pc

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = spatosc.pc

html-local: doxygen
doxygen:
	@if [ -z `which doxygen` ]; then echo "No Doxygen!"; else doxygen; fi
clean-local:
	rm -rf html
