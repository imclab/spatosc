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
 * Tests that the Wrapper works.
 */

#include <iostream>
#include "spatosc.h"

using namespace spatosc;

bool test_wrapper()
{
    Wrapper wrapper;
    // Create source and listener, see if we can connect them:
    if (! wrapper.setAutoConnect(false))
        return false;
    if (! wrapper.createSource("source"))
        return false;
    if (! wrapper.createListener("listener"))
        return false;
    if (! wrapper.connect("source", "listener"))
        return false;
    if (! wrapper.disconnect("source", "listener"))
        return false;
    if (! wrapper.connect("source", "listener"))
        return false;

    // See if auto connect works:
    if (! wrapper.setAutoConnect(true))
        return false;
    if (! wrapper.createSource("source2"))
        return false;
    if (! wrapper.disconnect("source2", "listener"))
        return false;

    // See if we can set a node's position and the like:
    if (! wrapper.setOrientation("source", 45.0, 45.0, 45.0))
        return false;
    if (! wrapper.setPosition("source", 1.0, 1.0, 1.0))
        return false;

    // see if we can fine tune a connection:
    if (! wrapper.setDopplerFactor("source", "listener", 45.0))
        return false;
    if (! wrapper.setDistanceFactor("source", "listener", 45.0))
        return false;
    return true;
}

int main(int /*argc*/, char ** /*argv*/)
{
    if (! test_wrapper())
        return 1;
    return 0;
}

