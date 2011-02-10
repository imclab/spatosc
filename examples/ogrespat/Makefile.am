noinst_HEADERS= base_application.h spat_application.h text_renderer.h
bin_PROGRAMS= ogrespat
ogrespat_CPPFLAGS= -I$(top_srcdir)
ogrespat_SOURCES= base_application.cpp spat_application.cpp text_renderer.cpp
ogrespat_CXXFLAGS= $(OGRE_CFLAGS) $(OIS_CFLAGS) $(SPATOSC_CFLAGS) -Wall -Werror -Wfatal-errors
ogrespat_LDADD= $(OGRE_LIBS) $(OIS_LIBS) $(SPATOSC_LIBS)
EXTRA_DIST = autogen.sh
