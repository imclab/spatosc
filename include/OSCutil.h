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
 * OpenSoundControl utilities.
 */
#ifndef __OSCutil_H
#define __OSCutil_H

#include <vector>
#include <string>
#include <sstream>

namespace OSCutil 
{
    // *****************************************************************************
    // networking functions

    std::string getHostname();
    std::string getMyIPaddress();
    std::string getMyBroadcastAddress();
    bool isMulticastAddress(const std::string &s);
    bool isBroadcastAddress(const std::string &s);

    // *****************************************************************************
    // string handling functions

    template <class T> bool fromString(T& t, const std::string& s)
    {
        std::istringstream iss(s);
        return !(iss >> t).fail();
    }

    std::string stringify(int i);
    std::string stringify(float x);
    std::string leadingSpaces(int n);
    std::vector<std::string> tokenize(const std::string& str, const std::string& delimiters = " ");
    std::vector<float> floatsFromString (const std::string &theString);

    bool wildcardMatch(const char *pat, const char *str);
};

#endif
