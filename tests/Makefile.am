check_PROGRAMS = \
    check_asynchronous \
    check_connectfilter \
    check_connections \
    check_distance \
    check_itworks \
    check_spatdif \
    check_unique \
    check_wrapper

TESTS = $(check_PROGRAMS)

check_asynchronous_SOURCES = check_asynchronous.cpp
check_connectfilter_SOURCES = check_connectfilter.cpp
check_connections_SOURCES = check_connections.cpp
check_distance_SOURCES = check_distance.cpp
check_itworks_SOURCES = check_it_works.cpp
check_spatdif_SOURCES = check_spatdif.cpp
check_unique_SOURCES = check_unique.cpp
check_wrapper_SOURCES = check_wrapper.cpp

check_asynchronous_LDADD = $(top_builddir)/src/libspatosc-@SPATOSC_API_VERSION@.la
check_connectfilter_LDADD = $(top_builddir)/src/libspatosc-@SPATOSC_API_VERSION@.la
check_itworks_LDADD = $(top_builddir)/src/libspatosc-@SPATOSC_API_VERSION@.la
check_distance_LDADD = $(top_builddir)/src/libspatosc-@SPATOSC_API_VERSION@.la
check_unique_LDADD = $(top_builddir)/src/libspatosc-@SPATOSC_API_VERSION@.la
check_connections_LDADD = $(top_builddir)/src/libspatosc-@SPATOSC_API_VERSION@.la
check_wrapper_LDADD = $(top_builddir)/src/libspatosc-@SPATOSC_API_VERSION@.la
check_spatdif_LDADD = $(top_builddir)/src/libspatosc-@SPATOSC_API_VERSION@.la

check_asynchronous_CPPFLAGS = -I$(top_srcdir)/include
check_connectfilter_CPPFLAGS = -I$(top_srcdir)/include
check_itworks_CPPFLAGS = -I$(top_srcdir)/include
check_distance_CPPFLAGS = -I$(top_srcdir)/include
check_unique_CPPFLAGS = -I$(top_srcdir)/include
check_connections_CPPFLAGS = -I$(top_srcdir)/include
check_wrapper_CPPFLAGS = -I$(top_srcdir)/include
check_spatdif_CPPFLAGS = -I$(top_srcdir)/include

