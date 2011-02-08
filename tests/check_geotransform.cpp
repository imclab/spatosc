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
 * This test checks that the geotransform works.
 *
 * To run it though gdb:
 * $ libtool --mode=execute gdb check_geotransform
 */
#include "geotransform.h"
#include "scene.h"
#include "soundsource.h"

using namespace spatosc;

bool check_translation()
{
    Scene scene;
    SoundSource *node = scene.createSoundSource("bob");
    double x = 0.5;
    double y = 0.75;
    double z = 1.5;
    double offset = 1.0;

    // translate by offset in x, y and z;
    scene.getTransform().translate(offset, offset, offset);

    // now set the position and make sure it's been offset
    node->setPosition(x, y, z);
    if (node->getPosition().x != x + offset || node->getPosition().y != y + offset || node->getPosition().z != z + offset)
        return false;
    return true;
}

bool check_scaling()
{
    Scene scene;
    SoundSource *node = scene.createSoundSource("bob");
    double x = 0.5;
    double y = 0.75;
    double z = 1.5;
    double scaling = 2.0;

    // translate by offset in x, y and z;
    scene.getTransform().scale(scaling, scaling, scaling);

    // now set the position and make sure it's been offset
    node->setPosition(x, y, z);
    if (node->getPosition().x != x * scaling || node->getPosition().y != y * scaling || node->getPosition().z != z * scaling)
        return false;
    return true;
}

int main(int /*argc*/, char ** /*argv*/)
{
    if (! check_translation() or ! check_scaling())
        return 1;
    return 0;
}

