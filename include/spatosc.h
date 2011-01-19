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

#endif // __SPATOSC_H__
