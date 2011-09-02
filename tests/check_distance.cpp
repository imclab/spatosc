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
#include "spatosc.h"

static const bool VERBOSE = false;
using namespace spatosc;

bool check_simple_distances()
{
    Scene scene;
    scene.addTranslator("dmitri", "DmitriTranslator");
    SoundSource *sound_a = scene.createSoundSource("sound_a");
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
            return false;
        }
        double dist = conn->distance();
        double expected = x * 2;
        if (dist != expected)
        {
            std::cout << "Expected " << expected << " but distance between the two nodes is " << dist << std::endl;
            return false;
        }
    }
    if (VERBOSE)
    {
        scene.debugPrint();
    }
    return true;
}


bool xyz_matches_aed(SoundSource *node, Vector3 xyz, Vector3 aed)
{
    node->setPositionAED(xyz.x, xyz.y, xyz.z);
    Vector3 pos = node->getPosition();
    if (pos.x != xyz.x || pos.y != xyz.y || pos.z != xyz.z)
    {
        std::cout << "Expected XYZ:(" << xyz.x << ", " << xyz.y << ", " << xyz.z
            << ") for AED:(" << aed.x << ", " << aed.y << ", " << aed.z << ")"
            << " but we got XYZ:(" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
        return false;
    }
    return true;
}

bool check_position_aed()
{
    Scene scene;
    SoundSource *sound_a = scene.createSoundSource("sound_a");
    xyz_matches_aed(sound_a, Vector3(0.0, 0.0, 0.0), Vector3(0.0, 0.0, 0.0));
    xyz_matches_aed(sound_a, Vector3(0.0, 0.0, 10.0), Vector3(0.0, 10.0, 0.0));
    return true;
}

int main(int /*argc*/, char ** /*argv*/)
{
    if (! check_simple_distances())
        return 1;
    if (! check_position_aed())
        return 1;
    return 0;
}

