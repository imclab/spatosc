/**
 * Example of using the spatosc::Wrapper with the Dmitri translator.
 * 
 * This example is in the public domain.
 */
#include <iostream>
#include <string>
#include <spatosc/wrapper.h>

using namespace spatosc;

static void print(const std::string &message)
{
    std::cout << message << std::endl;
}

int main(int /*argc*/, char ** /*argv*/)
{
    Wrapper wrapper;
	wrapper.addDmitriTranslator("dmitri", "192.168.11.201", "18033", true);
    // Create source and listener, see if we can connect them:
    if (! wrapper.setAutoConnect(false))
        print("Error calling setAutoConnect");
    if (! wrapper.createSource("1"))
        print("Error calling createSource");
    if (! wrapper.createListener("listener"))
        print("Error calling createListener");
    if (! wrapper.connect("1", "listener"))
        print("Error calling connect");
    if (! wrapper.disconnect("1", "listener"))
        print("Error calling disconnect");
    if (! wrapper.connect("1", "listener"))
        print("Error calling connect");

    // See if auto connect works:
    if (! wrapper.setAutoConnect(true))
        print("Error calling setAutoConnect");
    if (! wrapper.createSource("source2"))
        print("Error calling createSource");
    if (! wrapper.disconnect("source2", "listener"))
        print("Error calling disconnect");

    // See if we can set a node's position and the like:
    if (! wrapper.setOrientation("1", 45.0, 45.0, 45.0))
        print("Error calling setOrientation");
    if (! wrapper.setPosition("1", 8.0, 30.0, 0.0))
        print("Error calling setPosition");
    return 0;
}

