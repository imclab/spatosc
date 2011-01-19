AM_CPPFLAGS = -I$(top_srcdir)/include

# use lib_LTLIBRARIES to build a shared lib:
lib_LTLIBRARIES = libspatosc-@SPATOSC_API_VERSION@.la

## Define the source file list for the "libspatosc-@SPATOSC_API_VERSION@.la"
## target.  Note that @SPATOSC_API_VERSION@ is not interpreted by Automake and
## will therefore be treated as if it were literally part of the target name,
## and the variable name derived from that.
## The file extension .cc is recognized by Automake, and makes it produce
## rules which invoke the C++ compiler to produce a libtool object file (.lo)
## from each source file.  Note that it is not necessary to list header files
## which are already listed elsewhere in a _HEADERS variable assignment.
libspatosc_@SPATOSC_API_VERSION@_la_SOURCES = \
    oscutils.cpp \
    oscreceiver.cpp \
    oscsender.cpp \
    scene.cpp \
    node.cpp \
    listener.cpp \
    maths.cpp \
    translator.cpp \
    dmitritranslator.cpp \
    connection.cpp \
    soundsource.cpp

## Instruct libtool to include ABI version information in the generated shared
## library file (.so).  The library ABI version is defined in configure.ac, so
## that all version information is kept in one place.
libspatosc_@SPATOSC_API_VERSION@_la_LDFLAGS = $(LIBLO_LDFLAGS) -export-dynamic -version-info $(SPATOSC_SO_VERSION)

libspatosc_@SPATOSC_API_VERSION@_la_includedir = $(includedir)/spatosc-$(SPATOSC_API_VERSION)/spatosc

libspatosc_@SPATOSC_API_VERSION@_la_include_HEADERS = \
    $(top_srcdir)/include/oscutils.h \
    $(top_srcdir)/include/oscreceiver.h \
    $(top_srcdir)/include/oscsender.h \
    $(top_srcdir)/include/scene.h \
    $(top_srcdir)/include/node.h \
    $(top_srcdir)/include/listener.h \
    $(top_srcdir)/include/maths.h \
    $(top_srcdir)/include/translator.h \
    $(top_srcdir)/include/dmitritranslator.h \
    $(top_srcdir)/include/connection.h \
    $(top_srcdir)/include/soundsource.h \
    $(top_srcdir)/include/spatosc.h

EXTRA_DIST=$(top_srcdir)/include/config.h

