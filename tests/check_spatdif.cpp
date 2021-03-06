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
 * This test checks a client using BasicReceiver
 *
 * To run it though gdb:
 * $ libtool --mode=execute gdb check_spatdif
 */

#include <iostream>
#include <cassert>
#include <map>
#include <string>
#include "scene.h"
#include "soundsource.h"
#include "translator.h"

int main()
{
    spatosc::Scene scene;
    spatosc::SoundSource *source(scene.createSoundSource("dummy"));
    assert(source->getID() == "dummy");

    lo_arg **args = new lo_arg*[3];
    int num_args = 3;
    for (int i = 0 ; i != 3; ++i)
        args[i] = new lo_arg;
    static const float x = 1.4f;
    static const float y = 2.4f;
    static const float z = 3.4f;
    args[0]->f = x;
    args[1]->f = y;
    args[2]->f = z;

    source->handleMessage("xyz", num_args, args, "fff");

    for (int i = 0 ; i != num_args; ++i)
        delete args[i];
    delete [] args;
    spatosc::Vector3 pos = source->getPosition();
    if (pos.x != x || pos.y != y || pos.z != z)
        return 1;
    return 0;
}
