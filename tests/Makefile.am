AM_CXXFLAGS = -I$(top_srcdir)/include
LDADD = $(top_builddir)/src/libspatosc-@SPATOSC_API_VERSION@.la

check_PROGRAMS = \
    check_asynchronous \
    check_azim_elev \
    check_connectfilter \
    check_connections \
    check_distance \
    check_itworks \
    check_spatdif \
    check_spatdif_osc \
    check_unique \
    check_wrapper

TESTS = $(check_PROGRAMS)

check_asynchronous_SOURCES = check_asynchronous.cpp
check_azim_elev_SOURCES = check_azim_elev.cpp
check_connectfilter_SOURCES = check_connectfilter.cpp
check_connections_SOURCES = check_connections.cpp
check_distance_SOURCES = check_distance.cpp
check_itworks_SOURCES = check_it_works.cpp
check_spatdif_SOURCES = check_spatdif.cpp
check_spatdif_osc_SOURCES = check_spatdif_osc.cpp
check_unique_SOURCES = check_unique.cpp
check_wrapper_SOURCES = check_wrapper.cpp
