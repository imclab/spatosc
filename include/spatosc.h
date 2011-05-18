/*
 * This file is part of Spatosc.
 *
 * Copyright (c) 2010 Society for Arts and Technologies <info@sat.qc.ca>
 *
 * Spatosc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Spatosc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Spatosc.  If not, see <http://www.gnu.org/licenses/>.
 */

/** @file
 * Single public header file for spatosc.
 *
 * Includes the other header files.
 */
#ifndef __SPATOSC_H__
#define __SPATOSC_H__

/**
 * @namespace spatosc
 *
 * C++ library to communicate with various 3D audio spatializers.
 * 
 * The whole library is within this single namespace.
 */

#include "connection.h"
#include "dmitri_translator.h"
#include "fudi_sender.h"
#include "fudi_translator.h"
#include "listener.h"
#include "maths.h"
#include "node.h"
#include "oscreceiver.h"
#include "oscsender.h"
#include "oscutils.h"
#include "properties.h"
#include "property.h"
#include "scene.h"
#include "soundsource.h"
#include "basic_translator.h"
#include "translator.h"
#ifndef _WIN32
#include "version.h"
#endif // WIN32
#include "wrapper.h"

/**
 * @mainpage The spatosc library.
 *
 * @section About
 *
 * The spatosc library provides a set of classes and functions to control different audio spatialization systems via the OpenSoundControl protocol with a single unified C++ interface.
 *
 * @section Installation
 *
 * This library uses the GNU Autotools. It is documented using the syntax of Doxygen.
 *
 * @section Dependencies
 *
 * It requires liblo.
 *
 * @section Packaging
 *
 * Include the header:
 *
   \code
   #include <spatosc/spatosc.h>
   \endcode
 *
 * (You may include individual headers, such as <spatosc/node.h> for example, but they are all included with the common one.)
 *
 * If your source file is main.cpp, you can compile it with:
 *
   \verbatim
   g++ `pkg-config --libs --cflags spatosc-0.2`  -o main main.cpp
   \endverbatim
 *
 * Alternatively, if using autoconf, use the following in configure.ac:
 *
   \verbatim
   PKG_CHECK_MODULES([SPATOSC], [spatosc-0.2])
   \endverbatim
 * 
 * Of course, change the version for spatosc-0.4, or so, according to the major
 * and minor version numbers of the library.
 *
 * Then use the generated SPATOSC_CFLAGS and SPATOSC_LIBS variables in the project Makefile.am files. For example:
 *
   \verbatim
   program_CPPFLAGS = $(SPATOSC_CFLAGS)
   program_LDADD = $(SPATOSC_LIBS)
   \endverbatim
 *
 * To build spatosc for Microsoft Windows, please follow the instructions in the README.txt file of the project's "build" directory.
 *
 * @section Usage
 *
 * One should create a instance of Scene, and set the Translator pluging to use. The Translator takes care of
 * sending messages to some audio renderer to render the scene.
 *
 * Next, one should create some SoundSource nodes so that the Listener node can hear some spatialized sound.
 *
 * Move those nodes around using the methods of the Node class.
 *
 * @section License
 *
 * Copyright (c) 2010 Society for Arts and Technologies <info@sat.qc.ca>
 *
 * Spatosc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Spatosc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Spatosc.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * @author Mike Wozniewski <mike@mikewoz.com>
 * @author Tristan Matthews <le.businessman@gmail.com>
 * @author Alexandre Quessy <alexandre@quessy.net>
 *
 *
 * \image html doc/images/logo_SAT.jpg
 */


/** @page README README
 * 
 * @htmlonly <pre style="white-space: pre-wrap;">@endhtmlonly
 * \htmlinclude ../README
 * @htmlonly </pre>@endhtmlonly
 *
 */


#endif // __SPATOSC_H__
