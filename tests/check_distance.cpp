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
 * This test checks that distances between two nodes are OK.
 * 
 * To run it though gdb:
 * $ libtool --mode=execute gdb check_distance
 */
#include <iostream>
#include <tr1/memory>
#include "spatosc.h"

static const bool VERBOSE = false;

using namespace spatosc;

int main(int /*argc*/, char ** /*argv*/)
{
    using std::tr1::shared_ptr;

    if (VERBOSE)
        std::cout << std::endl << "Running..." << std::endl;

    Scene scene;

    scene.setTranslator<DmitriTranslator>("127.0.0.1");

    SoundSource *sound_a = scene.createSoundSource("sound_a");
    sound_a->setChannelID(1);

    Listener *listener = scene.createListener("listener");

    if (VERBOSE)
        scene.debugPrint();

    for (double x = 0.0; x < 10.0; x += 1.0)
    {
        sound_a->setPosition(  x, 0.0, 0.0);
        listener->setPosition(- x, 0.0, 0.0);
        Connection *conn = scene.getConnection(sound_a, listener);
        if (! conn)
        {
            std::cout << "Could not find a connection between the two nodes." << std::endl;
            return 1;
        }
        double dist = conn->distance();
        double expected = x * 2;
        if (dist != expected)
        {
            std::cout << "Expected " << expected << " but distance between the two nodes is " << dist << std::endl;
            return 1;
        }
        sleep(0.1);
    }

    if (VERBOSE)
    {
        scene.debugPrint();
        std::cout << "Bye.\n";
    }
    std::cout << "Success" << std::endl;
    return 0;
}

