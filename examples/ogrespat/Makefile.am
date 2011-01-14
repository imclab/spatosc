noinst_HEADERS= BaseApplication.h TutorialApplication.h
 
bin_PROGRAMS= OgreApp
OgreApp_CPPFLAGS= -I$(top_srcdir)
OgreApp_SOURCES= BaseApplication.cpp TutorialApplication.cpp
OgreApp_CXXFLAGS= $(OGRE_CFLAGS) $(OIS_CFLAGS) $(SPATOSC_CFLAGS) -Wall -Werror -Wfatal-errors
OgreApp_LDADD= $(OGRE_LIBS) $(OIS_LIBS) $(SPATOSC_LIBS)
 
EXTRA_DIST = autogen.sh
