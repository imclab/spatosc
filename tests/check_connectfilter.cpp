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
 * This test checks that the connect filter works.
 *
 * To run it though gdb:
 * $ libtool --mode=execute gdb check_connectfilter
 */
#include <iostream>
#include "spatosc.h"

using namespace spatosc;

bool check_regex()
{
#ifdef HAVE_REGEX
    Scene scene;
    scene.setConnectFilter("yes.*");
    Listener *listener = scene.createListener("listener");
    SoundSource *yes = scene.createSoundSource("yes");
    SoundSource *no = scene.createSoundSource("no");
    if (scene.getConnection(no, listener) != 0)
    {
        std::cout << "It did connect even if should not match regex filter." << std::endl;
        return false;
    }
    if (scene.getConnection(yes, listener) == 0)
    {
        std::cout << "It did not connect even if should match regex filter." << std::endl;
        return false;
    }
#endif
    return true;
}

int main(int /*argc*/, char ** /*argv*/)
{
    if (! check_regex())
        return 1;
    return 0;
}

