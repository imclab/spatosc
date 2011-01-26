check_PROGRAMS = \
    check_distance \
    check_it_works \
    check_unique \
    check_connections \
    check_wrapper \
    check_spatdif

check_it_works_SOURCES = check_it_works.cpp
check_distance_SOURCES = check_distance.cpp
check_unique_SOURCES = check_unique.cpp
check_connections_SOURCES = check_connections.cpp
check_wrapper_SOURCES = check_wrapper.cpp
check_spatdif_SOURCES = check_spatdif.cpp

check_it_works_LDADD = $(top_builddir)/src/libspatosc-@SPATOSC_API_VERSION@.la
check_distance_LDADD = $(top_builddir)/src/libspatosc-@SPATOSC_API_VERSION@.la
check_unique_LDADD = $(top_builddir)/src/libspatosc-@SPATOSC_API_VERSION@.la
check_connections_LDADD = $(top_builddir)/src/libspatosc-@SPATOSC_API_VERSION@.la
check_wrapper_LDADD = $(top_builddir)/src/libspatosc-@SPATOSC_API_VERSION@.la
check_spatdif_LDADD = $(top_builddir)/src/libspatosc-@SPATOSC_API_VERSION@.la

check_distance_CPPFLAGS = -I$(top_srcdir)/include
check_it_works_CPPFLAGS = -I$(top_srcdir)/include
check_unique_CPPFLAGS = -I$(top_srcdir)/include
check_connections_CPPFLAGS = -I$(top_srcdir)/include
check_wrapper_CPPFLAGS = -I$(top_srcdir)/include
check_spatdif_CPPFLAGS = -I$(top_srcdir)/include

TESTS = $(check_PROGRAMS)

