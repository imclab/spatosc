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
 * This test checks a client using SpatdifReceiver
 *
 * To run it though gdb:
 * $ libtool --mode=execute gdb check_spatdif_osc
 */

#include "oscsender.h"
#include "scene.h"
#include "soundsource.h"

int main()
{
    // test's scene's receiving facilities
    const char *TEST_PORT = "11111";
    spatosc::OscSender sender("127.0.0.1", TEST_PORT);
    spatosc::Scene scene(TEST_PORT);
    spatosc::SoundSource *source(scene.createSoundSource("dummy"));
    
    static const float x = 1.4f;
    static const float y = 2.4f;
    static const float z = 3.4f;

    sender.sendMessage("/spatosc/core/source/dummy/xyz", "fff", x, y, z, LO_ARGS_END);
    scene.poll();

    spatosc::Vector3 pos = source->getPosition();
    if (pos.x != x || pos.y != y || pos.z != z)
        return 1;
    return 0;
}
