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

/**
 * Tests that there can only be unique node names.
 */

#include <iostream>
#include "spatosc.h"

using namespace spatosc;

bool test_unique_source()
{
    Scene scene;
    scene.createSoundSource("foo");
    SoundSource *foo_source = scene.createSoundSource("foo");
    if (foo_source != 0)
    {
        std::cout << "FAIL: duplicate foo SoundSource" << std::endl;
        return false;
    }
    return true;
}

bool test_unique_listener()
{
    Scene scene;
    scene.createListener("foo");
    Listener *foo_listener = scene.createListener("foo");
    if (foo_listener != 0)
    {
        std::cout << "FAIL: duplicate foo Listener" << std::endl;
        return false;
    }
    return true;
}

bool test_unique_mixed_node_types()
{
    Scene scene;
    scene.createListener("foo");
    SoundSource *foo_listener = scene.createSoundSource("foo");
    if (foo_listener != 0)
    {
        std::cout << "FAIL: duplicate node named foo" << std::endl;
        return false;
    }
    return true;
}

bool test_multiple_translators()
{
    Scene scene;
    if (! scene.addTranslator<ConsoleTranslator>("default"))
    {
        std::cout << "FAIL: could not add a first translator." << std::endl;
        return false;
    }
    if (scene.addTranslator<ConsoleTranslator>("default"))
    {
        std::cout << "FAIL: could add a second translator with same name." << std::endl;
        return false;
    }
    if (! scene.hasTranslator("default"))
    {
        std::cout << "FAIL: could not find translator." << std::endl;
        return false;
    }
    if (! scene.removeTranslator("default"))
    {
        std::cout << "FAIL: could not remove translator." << std::endl;
        return false;
    }
    if (scene.removeTranslator("default"))
    {
        std::cout << "FAIL: could remove translator twice." << std::endl;
        return false;
    }
    if (scene.hasTranslator("default"))
    {
        std::cout << "FAIL: still has translator." << std::endl;
        return false;
    }
    return true;
}

int main(int /*argc*/, char ** /*argv*/)
{
    if (! test_unique_listener())
        return 1;
    if (! test_unique_source())
        return 1;
    if (! test_unique_mixed_node_types())
        return 1;
    return 0;
}

