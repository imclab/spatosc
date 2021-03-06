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
#include <tr1/memory>
#include <spatosc/spatosc.h>

static const bool VERBOSE = true;

int main(int argc, char ** /*argv*/)
{
    using std::tr1::shared_ptr;
    using namespace spatosc;
    if (VERBOSE)
        std::cout << "\nRunning spatoscTest ...\n" << std::endl;

    // The spatosc library provides an API structured around one scene class
    // which  maintains lists of listeners and sound
    // sources, and provides methods to get and create instances of these items.

    // create the scene
    Scene scene;

    // set verbosity of info messages:
    scene.setVerbose(VERBOSE);

    // In order to send OSC, some output plugin must be specified. In this case,
    // we choose the BasicTranslator, which can be rendered, for example, by
    // the pd-vbap example.
    if (argc > 1)
        scene.addTranslator("fudi", new FudiTranslator("localhost", "31337"));
    else
        scene.addTranslator("basic", new BasicTranslator("osc.udp://127.0.0.1:18032"));

    // Each scene needs to have at least one Listener:
    scene.createListener("listener");

    // A variable number of SoundSource instances can then be generated.
    SoundSource *foo = scene.createSoundSource("foo");
    SoundSource *bar = scene.createSoundSource("bar");

    // Specifying sounds for the nodes is not fully supported yet (ie, supports
    // only live inputs). However, the BasicRenderer that comes with Pdsheefa
    // recognizes the following message:
    foo->setStringProperty("setMediaURI", "plugin://plugins/testnoise~");
    bar->setStringProperty("setMediaURI", "plugin://plugins/testone~");

    // The Scene class can print out everything to the console:
    if (VERBOSE)
        scene.debugPrint();

    // Now we just move nodes around and updates should be sent to D-Mitri:
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

            foo->setPosition(sinf(angle)*orbitRadius,
                             cosf(angle)*orbitRadius,
                             1.0);
            bar->setPosition(sinf(-angle)*orbitRadius,
                             0.0,
                             1.0);
	        usleep(1000000 * orbitDuration / numSamples);
        }
    }

    if (VERBOSE)
        scene.debugPrint();

    if (VERBOSE)
        std::cout << "Exitting...\n";
    return 0;
}
