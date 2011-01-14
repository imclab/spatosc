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
#include "spatosc.h"

using namespace spatosc;

int main(int /*argc*/, char ** /*argv*/)
{
    using std::tr1::shared_ptr;
    std::cout << "\nRunning spatoscTest ...\n" << std::endl;
    // create the scene
    Scene scene;

    // The spatosc library provides an API structured around one singleton class
    // called Scene. This class maintains lists of listeners and sound
    // sources, and provides methods to get and create instances of these items.


    // Each scene needs to have at least one Listener:
    //Listener *listener = scene.getOrCreateListener("listener");

    // A variable number of SoundSource instances can then be generated. Note
    // that a bus number must be assigned to each source in order to render in
    // D-Mitri. Setting the bus to -1 will effectively disable computation for
    // that node:
    SoundSource *foo = scene.getOrCreateSoundSource("foo");
    foo->setChannelID(1);
    SoundSource *bar = scene.getOrCreateSoundSource("bar");
    bar->setChannelID(2);

    // In order to send OSC, some output plugin must be specified. In this case,
    // we choose D-Mitri, and provide the IP address of the server on the
    // control network. Note that D-Mitri uses 2 interfaces, a control network
    // (typically IPv4) and an audio network (AVB):
    shared_ptr<DmitriTranslator> translator(new DmitriTranslator("192.168.2.26"));
    scene.setTranslator(translator);

    // The Scene class can print out everything to the console:
    scene.debugPrint();

    // Now we just move nodes around and updates should be sent to D-Mitri:

    foo->setPosition(0,10,0);
    bar->setPosition(5,5,0);

    sleep(0.1);

    foo->setPosition(0,5,0);
    bar->setPosition(-5,5,0);

    scene.debugPrint();

    std::cout << "Exitting...\n";
    return 0;
}
