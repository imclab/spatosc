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

#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>

#ifndef WIN32
#include <sys/utsname.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#endif

#include "oscutils.h"

namespace spatosc
{

// *****************************************************************************
// networking functions
std::string OSCutil::getMyIPaddress()
{
#ifdef WIN32
    return "127.0.0.1";
#else
    using std::string;
    struct ifaddrs *interfaceArray = 0, *tempIfAddr = 0;
    void *tempAddrPtr = 0;
    int rc = 0;
    char addressOutputBuffer[INET6_ADDRSTRLEN];

    string IPaddress;

    rc = getifaddrs(&interfaceArray);  /* retrieve the current interfaces */
    if (rc == 0)
    {
        for (tempIfAddr = interfaceArray; tempIfAddr != NULL; tempIfAddr = tempIfAddr->ifa_next)
        {
            if (tempIfAddr->ifa_addr && (tempIfAddr->ifa_addr->sa_family == AF_INET)) // check if it is IP4
            {
                tempAddrPtr = &((struct sockaddr_in *)tempIfAddr->ifa_addr)->sin_addr;

                if (string(tempIfAddr->ifa_name).find("lo")==string::npos) // skip loopback
                {
                    IPaddress = inet_ntop(tempIfAddr->ifa_addr->sa_family, tempAddrPtr, addressOutputBuffer, sizeof(addressOutputBuffer));

                    //printf("Internet Address: [%s] %s \n", tempIfAddr->ifa_name, IPaddress.c_str());

                    // TODO: for now we just return the first address found. Eventually, we could ask for a specific address (eg, "eth0" vs "eth1")
                    break;
                }
            }
        }
    }
    return IPaddress;
#endif
}

std::string OSCutil::getMyBroadcastAddress()
{
#ifndef WIN32
    std::string myIP(getMyIPaddress());
#else
    std::string myIP("255.255.255.255");
#endif
    return myIP.substr(0,myIP.rfind(".")) + ".255";
}

bool OSCutil::isMulticastAddress(const std::string &s)
{
    bool b = false;
    try
    {
        int i = atoi(s.substr(0, s.find(".")).c_str());
        if (i >= 224 && i <= 239)
            b = true;
    }
    catch (int i)
    {
        b = false;
    }
    return b;
}

bool OSCutil::isBroadcastAddress(const std::string &s)
{
    bool b = false;
    try
    {
        if (s.substr(s.rfind(".") + 1) == "255")
            b = true;
    }
    catch (int i)
    {
        b = false;
    }
    return b;
}

// *****************************************************************************
// string handling functions

std::string OSCutil::stringify(int i)
{
    std::ostringstream o;
    if ((o << i) == 0)
        return "";
    else
        return o.str();
}

std::string OSCutil::stringify(float x)
{
    std::ostringstream o;
    if ((o << x) == 0)
        return "";
    else
        return o.str();
}

std::vector<std::string> OSCutil::tokenize(const std::string& str, const std::string& delimiters)
{
    using std::vector;
    using std::string;

    vector<string> tokens;

    // skip delimiters at beginning:
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // find first "non-delimiter":
    string::size_type pos = str.find_first_of(delimiters, lastPos);

    if (lastPos == string::npos)
    {
        // this is an empty string, so return empty vector:
        return tokens;
    }
    else if (pos == string::npos)
    {
        // no delimiter could be found (ie, there is just one token)
        tokens.push_back(str);
        return tokens;
    }
    else
    {
        while (string::npos != pos || string::npos != lastPos)
        {
            // found a token, add it to the vector:
            tokens.push_back(str.substr(lastPos, pos - lastPos));
            // skip delimiters (Note the "not_of"):
            lastPos = str.find_first_not_of(delimiters, pos);
            // find next "non-delimiter":
            pos = str.find_first_of(delimiters, lastPos);
        }
        return tokens;
    }
}

std::vector<float> OSCutil::floatsFromString(const std::string &theString)
{
    using std::vector;
    using std::string;

    vector<string> in_Tokens = tokenize(theString);
    vector<float> out_Tokens;
    float num;

    for (unsigned int i = 0; i < in_Tokens.size(); i++)
    {
        // only add to vector if token is a number:
        if (fromString<float>(num, in_Tokens[i]))
            out_Tokens.push_back(num);
        //if (fromString(num, in_Tokens[i])) out_Tokens.push_back(num);
    }
    return out_Tokens;
}

bool OSCutil::wildcardMatch(const char *path, const char *str)
{
    switch (*path)
    {
        // every case returns, no need for break statements
        case '\0':
            return *str == '\0';
        case '*':
            return wildcardMatch(path + 1, str) || (*str && wildcardMatch(path, str + 1));
        case '?':
            return *str && wildcardMatch(path + 1, str + 1);
        default:
            return (*path == *str) && (wildcardMatch(path + 1, str + 1));
    }
}


bool OSCutil::typeTagsMatch(const char *types, const char *desiredTypes)
{
    if (!types)
        return false;
    if (!desiredTypes)
        return false;
    if (strcmp(types, desiredTypes) != 0)
    {
        std::cerr << "Expected OSC typetags " << desiredTypes << " but got " << types << "." << std::endl;
        return false;
    }
    else
        return true;
}

} // end namespace spatosc
