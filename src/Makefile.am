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
libspatosc_la_include_HEADERS = $(top_srcdir)/include/*.h

# Build example programs:
noinst_PROGRAMS = spatoscTest check_distance
spatoscTest_SOURCES = spatoscTest.cpp
spatoscTest_LDADD = libspatosc.la

check_distance_SOURCES = check_distance.cpp
check_distance_LDADD = libspatosc.la

