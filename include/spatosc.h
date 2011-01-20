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
 * The spatosc library provides a set of classes and functions to control different audio spatialization systems via the OpenSoundControl protocol with a single unified interface.
 */

#include "oscutils.h"
#include "oscreceiver.h"
#include "oscsender.h"
#include "scene.h"
#include "node.h"
#include "listener.h"
#include "maths.h"
#include "dmitritranslator.h"
#include "translator.h"
#include "connection.h"
#include "soundsource.h"
#include "version.h"

/**
 * @mainpage The spatosc library.
 *
 * @section Installation
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
   g++ `pkg-config --libs --cflags spatosc-0.1`  -o main main.cpp
   \endverbatim
 *
 * Alternatively, if using autoconf, use the following in configure.ac:
 * 
   \verbatim
   PKG_CHECK_MODULES([SPATOSC], [spatosc-0.1])
   \endverbatim
 * 
 * Then use the generated SPATOSC_CFLAGS and SPATOSC_LIBS variables in the project Makefile.am files. For example:
 * 
   \verbatim
   program_CPPFLAGS = $(SPATOSC_CFLAGS)
   program_LDADD = $(SPATOSC_LIBS)
   \endverbatim
 * 
 * @section Usage
 *
 * One should create a instance of Scene, and set the Translator pluging to use. The Translator takes care of 
 * sending messages to some audio renderer to render the scene.
 *
 * Next, one should create some SoundSource nodes so that the Listener node can hear some spatialized sound. 
 *
 * Move those nodes around using the methods of the Node class.
 */

#endif // __SPATOSC_H__
