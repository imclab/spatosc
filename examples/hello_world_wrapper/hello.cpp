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

#include <iostream>
#include <cmath>
#include <spatosc/wrapper.h>


int main(int /*argc*/, char ** /*argv*/)
{
    std::cout << "\nRunning hello_world ...\n" << std::endl;

    // Create an instance of the spatosc::Wrapper. This will be used to for all
    // interaction with spatosc
    spatosc::Wrapper spatosc;

    // We will output messages using several translators to show the full
    // capabilities of SpatOSC. Usually, only one translator is necassary.
    // (Note that DMitri requires a special function)
    spatosc.addTranslator("fudi", "FudiTranslator", "localhost", "31337");
    spatosc.addTranslator("basic", "BasicTranslator", "localhost", "9999");
    spatosc.addTranslator("dmitri", "DmitriTranslator", "localhost", "18033");


    // Each scene needs to have at least one Listener:
    spatosc.createListener("listener");

    // A variable number of SoundSource instances can then be generated.
    spatosc.createSource("foo");
    spatosc.createSource("bar");

    // Each node can have properties, which are used by the translators for
    // various functions. Here, we set the bus number for each sound source
    // (used by D-Mitri), and properties like 'enableGain', 'enableDelay'
    // may be used by several translators to determine if they should send
    // those values
    spatosc.setNodeIntProperty("foo", "bus", 1);
    spatosc.setNodeIntProperty("bar", "bus", 2);
    spatosc.setNodeIntProperty("foo", "enableDelay", 0);
    spatosc.setNodeIntProperty("bar", "enableDelay", 1);

    // Example of more dmitri-specific properties:
    //spatosc.setNodeFloatProperty("listener", "spacemapEquator", 100.0);
    //spatosc.setNodeFloatProperty("listener", "spacemapPole", 200.0);

    // Print out everything to the console:
    spatosc.debugPrint();

    // Now we move nodes around and updates should be sent to the translators:
    //
    // 'foo' will orbit in a circle, while 'bar' will slide back and forth
    // along the X axis
    
    float orbitRadius = 5.0;
    float orbitDuration = 10.0;
    int numSamples = 100;

    while (1)
    {
        for (int i=0; i<numSamples; i++)
        {
            float angle = i * 2.0 * M_PI / (numSamples-1);

            spatosc.setPosition("foo",
                                sinf(angle)*orbitRadius,
                                cosf(angle)*orbitRadius,
                                1.0);
            spatosc.setPosition("bar",
                                sinf(-angle)*orbitRadius,
                                0.0,
                                1.0);
	        usleep(1000000 * orbitDuration / numSamples);
        }
    }


    std::cout << "Exitting...\n";
    return 0;
}
