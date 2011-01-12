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
