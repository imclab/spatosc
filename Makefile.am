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

html-local: htmldoc
htmldoc:
	@if [ -z `which doxygen` ]; then echo "No Doxygen!"; else doxygen; fi

# man pages:
if HAVE_DOXYGEN
dist_man_MANS = $(top_srcdir)/man/man3/*.3
$(top_srcdir)/man/man3/*.3: doxyfile.stamp

doxyfile.stamp:
	$(DOXYGEN) doxyfile
	echo `date -R` > doxyfile.stamp

CLEANFILES = doxyfile.stamp

all-local: doxyfile.stamp
clean-local:
	rm -rf $(top_builddir)/man
	rm -rf html
endif 
