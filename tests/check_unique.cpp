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
#include "spatosc.h"

using namespace spatosc;

int main(int /*argc*/, char ** /*argv*/)
{
    Scene scene;
    scene.createSoundSource("foo");
    SoundSource *foo_source = scene.createSoundSource("foo");
    if (foo_source != 0)
    {
        std::cout << "duplicate foo SoundSource" << std::endl;
        return 1;
    }
    Listener *foo_listener = scene.createListener("foo");
    if (foo_listener != 0)
    {
        std::cout << "duplicate foo Listener" << std::endl;
        return 1;
    }
    return 0;
}
