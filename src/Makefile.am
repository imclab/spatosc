AM_CPPFLAGS = -I$(top_srcdir)/include

# use lib_LTLIBRARIES to build a shared lib:
lib_LTLIBRARIES = libspatosc.la
libspatosc_la_SOURCES = \
    oscutils.cpp \
    oscreceiver.cpp \
    scene.cpp \
    node.cpp \
    listener.cpp \
    maths.cpp \
    translator.cpp \
    dmitritranslator.cpp \
    connection.cpp \
    soundsource.cpp

libspatosc_la_LDFLAGS = $(LIBLO_LDFLAGS) -export-dynamic
libspatosc_la_includedir = $(includedir)/spatosc
libspatosc_la_include_HEADERS = \
    $(top_srcdir)/include/oscutils.h \
    $(top_srcdir)/include/oscreceiver.h \
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

