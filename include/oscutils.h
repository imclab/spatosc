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
 * OpenSoundControl utilities (networking and string handling utilities)
 */
#ifndef __OSCUTILS_H__
#define __OSCUTILS_H__

#include <vector>
#include <string>
#include <sstream>
#include <lo/lo.h>

#define SPATOSC_ARGS_END LO_ARGS_END

namespace spatosc
{

/**
 * OpenSoundControl utilities (networking and string handling utilities)
 */
namespace OSCutil
{
    // *****************************************************************************
    // networking functions

    /**
     * Returns this computer's host name.
     */
    std::string getHostname();
    /**
     * Tries to guess the IP address of one of this computer's network interfaces.
     * @return The IP address of one of this computer's network interfaces, or "127.0.0.1" if it failed to find it.
     * @warning This is not currently supported on Windows. It will return "127.0.0.1".
     */
    std::string getMyIPaddress();

    /**
     * Returns the IP address for broadcasting UDP messages to this local area network.
     * @warning This is not currently supported on Windows. It will return "255.255.255.255".
     */
    std::string getMyBroadcastAddress();

    /**
     * Checks whether this a given IP address is multicast or not.
     */
    bool isMulticastAddress(const std::string &s);

    /**
     * Checks whether this a given IP address is broadcast or not.
     */
    bool isBroadcastAddress(const std::string &s);

    // *****************************************************************************
    // string handling functions

    /**
     * Casts a string to any type.
     */
    template <class T>
    bool fromString(T& t, const std::string& s)
    {
        std::istringstream iss(s);
        return ! (iss >> t).fail();
    }

    /**
     * Converts an int to a string.
     */
    std::string stringify(int i);

    /**
     * Converts a float to a string.
     */
    std::string stringify(float x);

    /**
     * Splits a string into tokens given a delimiter.
     */
    std::vector<std::string> tokenize(const std::string& str, const std::string& delimiters = " ");

    /**
     * Takes a string and uses spaces to
     * tokenize the string into a vector of floats. If the
     * tokens are symbolic instead of numeric, they are ignored.
     */
    std::vector<float> floatsFromString (const std::string &theString);

    /**
     * Returns whether a given path matches or not a given string.
     * @return success or not
     */
    bool wildcardMatch(const char *path, const char *str);
    // TODO:2011-01-17:aalex:Please someone document wildcardMatch() well.

    /**
     * Checks that a typetag matches a desired one.
     */
    bool typeTagsMatch(const char *types, const char *desiredTypes);

} // end namespace OSCutil
} // end namespace spatosc

#endif // __OSCUTILS_H__
