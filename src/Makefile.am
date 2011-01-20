AM_CPPFLAGS = $(all_includes)
AM_CXXFLAGS = $(all_includes) $(LIBLO_CFLAGS)

# use lib_LTLIBRARIES to build a shared lib:
lib_LTLIBRARIES = libspatosc.la
libspatosc_la_SOURCES = \
    OSCutil.cpp \
	vAudioManager.cpp \
    vBaseNode.cpp \
    vListener.cpp \
    vMath.cpp \
    vPlugin.cpp \
    vPlugin_dmitri.cpp \
    vSoundConn.cpp \
    vSoundSource.cpp

libspatosc_la_LDFLAGS = $(LIBLO_LIBS) -export-dynamic
libspatosc_la_includedir = $(includedir)/spatosc
libspatosc_la_include_HEADERS = $(top_srcdir)/include/*.h

# Build an example program:
noinst_PROGRAMS = spatoscTest
spatoscTest_SOURCES = spatoscTest.cpp
spatoscTest_LDADD = libspatosc.la

