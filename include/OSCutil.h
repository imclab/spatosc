#ifndef __OSCutil_H
#define __OSCutil_H

#include <vector>
#include <string>
#include <sstream>


// *****************************************************************************
// networking functions

std::string getHostname();
std::string getMyIPaddress();
std::string getMyBroadcastAddress();
bool isMulticastAddress(std::string s);
bool isBroadcastAddress(std::string s);

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


#endif
