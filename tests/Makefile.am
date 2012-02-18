AM_CXXFLAGS = -I$(top_srcdir)/include
LDADD = $(top_builddir)/src/libspatosc-@SPATOSC_API_VERSION@.la

check_PROGRAMS = \
    check_asynchronous \
    check_azim_elev \
    check_connectfilter \
    check_connections \
    check_distance \
    check_it_works \
    check_maths \
    check_osc \
    check_oscutils \
    check_properties \
    check_spatdif \
    check_spatdif_osc \
    check_unique \
    check_wrapper \
    check_geotransform

TESTS = $(check_PROGRAMS)

check_asynchronous_SOURCES = check_asynchronous.cpp
check_azim_elev_SOURCES = check_azim_elev.cpp
check_connectfilter_SOURCES = check_connectfilter.cpp
check_connections_SOURCES = check_connections.cpp
check_distance_SOURCES = check_distance.cpp
check_it_works_SOURCES = check_it_works.cpp
check_maths_SOURCES = check_maths.cpp
check_osc_SOURCES = check_osc.cpp
check_oscutils_SOURCES = check_oscutils.cpp
check_properties_SOURCES = check_properties.cpp
check_spatdif_SOURCES = check_spatdif.cpp
check_spatdif_osc_SOURCES = check_spatdif_osc.cpp
check_unique_SOURCES = check_unique.cpp
check_wrapper_SOURCES = check_wrapper.cpp
check_geotransform_SOURCES = check_geotransform.cpp
