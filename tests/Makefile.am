check_PROGRAMS = \
    check_distance \
    check_it_works \
    check_unique \
    check_connections

check_it_works_SOURCES = check_it_works.cpp
check_distance_SOURCES = check_distance.cpp
check_unique_SOURCES = check_unique.cpp
check_connections_SOURCES = check_connections.cpp

check_it_works_LDADD = $(top_builddir)/src/libspatosc-@SPATOSC_API_VERSION@.la
check_distance_LDADD = $(top_builddir)/src/libspatosc-@SPATOSC_API_VERSION@.la
check_unique_LDADD = $(top_builddir)/src/libspatosc-@SPATOSC_API_VERSION@.la
check_connections_LDADD = $(top_builddir)/src/libspatosc-@SPATOSC_API_VERSION@.la

check_distance_CPPFLAGS = -I$(top_srcdir)/include
check_it_works_CPPFLAGS = -I$(top_srcdir)/include
check_unique_CPPFLAGS = -I$(top_srcdir)/include
check_connections_CPPFLAGS = -I$(top_srcdir)/include

TESTS = $(check_PROGRAMS)

