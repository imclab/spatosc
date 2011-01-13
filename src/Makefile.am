AM_CPPFLAGS = -I$(top_srcdir)/include

# use lib_LTLIBRARIES to build a shared lib:
lib_LTLIBRARIES = libspatosc.la
libspatosc_la_SOURCES = \
    OSCutil.cpp \
    oscreceiver.cpp \
	scene.cpp \
    node.cpp \
    listener.cpp \
    vMath.cpp \
    translator.cpp \
    dmitritranslator.cpp \
    vSoundConn.cpp \
    soundsource.cpp

libspatosc_la_LDFLAGS = $(LIBLO_LDFLAGS) -export-dynamic
libspatosc_la_includedir = $(includedir)/spatosc
libspatosc_la_include_HEADERS = $(top_srcdir)/include/*.h

# Build an example program:
noinst_PROGRAMS = spatoscTest
spatoscTest_SOURCES = spatoscTest.cpp
spatoscTest_LDADD = libspatosc.la

