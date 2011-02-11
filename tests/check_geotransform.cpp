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
#include <iostream>

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
    scene.translate(offset, offset, offset);

    // now set the position and make sure it's been offset
    node->setPosition(x, y, z);
    if (node->getPosition().x != x + offset || node->getPosition().y != y + offset || node->getPosition().z != z + offset)
        return false;
    return true;
}

bool check_rotation()
{
    Scene scene;
    SoundSource *node = scene.createSoundSource("bob");
    double x = 1.5;
    double y = 1.5;
    double z = 1.5;
    double pitch = 90.0;
    double roll = 0.0;
    double yaw = 0.0;
    double rotatedX = 1.5;
    double rotatedY = -1.5;
    double rotatedZ = 1.5;

    scene.rotate(pitch, roll, yaw);

    // now set the position and make sure it's been offset
    node->setPosition(x, y, z);

    // we can't check for equality due to loss of precision, 
    // so we just make sure the calculated and expected values
    // are within a tolerance, EPSILON
    static const double EPSILON = 0.0001;
    if (fabs(node->getPosition().x - rotatedX) > EPSILON || 
        fabs(node->getPosition().y - rotatedY) > EPSILON || 
        fabs(node->getPosition().z - rotatedZ) > EPSILON)
        return false;
    else
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
    scene.scale(scaling, scaling, scaling);

    // now set the position and make sure it's been offset
    node->setPosition(x, y, z);
    if (node->getPosition().x != x * scaling || node->getPosition().y != y * scaling || node->getPosition().z != z * scaling)
        return false;
    return true;
}

// tests if preexisting nodes are transformed correctly if we apply a transofrmation after creating them
bool check_late_transformation()
{
    Scene scene;
    SoundSource *node = scene.createSoundSource("bob");
    double x = 0.5;
    double y = 0.75;
    double z = 1.5;
    double scaling = 2.0;
    
    // now set the position and make sure it's been offset
    node->setPosition(x, y, z);

    // translate by offset in x, y and z;
    scene.scale(scaling, scaling, scaling);

    if (node->getPosition().x != x * scaling || node->getPosition().y != y * scaling || node->getPosition().z != z * scaling)
        return false;
    return true;
}

int main(int /*argc*/, char ** /*argv*/)
{
    if (!check_translation())
    {
        std::cerr << "Check translation failed" << std::endl;
        return 1;
    }
    else if (!check_rotation())
    {
        std::cerr << "Check rotation failed" << std::endl;
        return 1;
    }
    else if (!check_scaling())
    {
        std::cerr << "Check scaling failed" << std::endl;
        return 1;
    }
    else if (!check_late_transformation())
    {
        std::cerr << "Check late transformation failed" << std::endl;
        return 1;
    }
    else
        return 0;
}

