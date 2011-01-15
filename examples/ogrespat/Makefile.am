noinst_HEADERS= BaseApplication.h TutorialApplication.h
 
bin_PROGRAMS= ogrespat
ogrespat_CPPFLAGS= -I$(top_srcdir)
ogrespat_SOURCES= BaseApplication.cpp TutorialApplication.cpp
ogrespat_CXXFLAGS= $(OGRE_CFLAGS) $(OIS_CFLAGS) $(SPATOSC_CFLAGS) -Wall -Werror -Wfatal-errors
ogrespat_LDADD= $(OGRE_LIBS) $(OIS_LIBS) $(SPATOSC_LIBS)
 
EXTRA_DIST = autogen.sh
