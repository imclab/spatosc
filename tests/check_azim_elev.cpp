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
 * This test checks that azimuth and elevation measurements between two nodes
 * are OK (especially when the listener has rotated)
 *
 * To run it though gdb:
 * $ libtool --mode=execute gdb check_distance
 */
#include <iostream>
#include <cmath>
#include "spatosc.h"

static const bool VERBOSE = true;
static const double EPSILON = 0.0001;

/**
 * 
 */
static bool checkAngles(spatosc::Connection *conn, double azim, double elev, const char *description="")
{
	double dAzim = std::abs(conn->azimuth() * spatosc::TO_DEGREES - azim);
	double dElev = std::abs(conn->elevation() * spatosc::TO_DEGREES - elev);

	bool success = ((dAzim < EPSILON) && (dElev < EPSILON));

	if (VERBOSE)
	{
		std::cout << description << ": " << std::endl;
		std::cout << "  got: "
			<< conn->azimuth() * spatosc::TO_DEGREES << "," << (conn->elevation() * spatosc::TO_DEGREES)
			<< ", expecting: " << azim << "," << elev
			<< " deltas: " << dAzim << "," << dElev
			<< (success ? " passed!" : " failed!") << std::endl;
	}
    return success;
}

int main(int /*argc*/, char ** /*argv*/)
{
    using namespace spatosc;
    if (VERBOSE)
        std::cout << std::endl << "Running test: check_azim_elev" << std::endl;
    Scene scene;
    scene.setTranslator<DmitriTranslator>("127.0.0.1", Translator::DEFAULT_SEND_PORT);
    SoundSource *source = scene.createSoundSource("source");
    Listener *listener = scene.createListener("listener");
    Connection *conn = scene.getConnection(source, listener);
    if (! conn)
    {
        std::cout << "Could not find a connection between the two nodes." << std::endl;
        return 1;
    }

    listener->setPosition(0.0, 0.0, 0.0);
    source->setPosition(0.0, 1.0, 0.0);
    if (! checkAngles(conn, 0, 0, "source in front"))
        return 1;
    source->setPosition(0.0, -1.0, 0.0);
    if (! checkAngles(conn, 0, 180, "source behind"))
        return 1;
    source->setPosition(0.0, 0.0, 1.0);
    if (! checkAngles(conn, 0, 90, "source up above"))
        return 1;
    source->setPosition(1.0, 0.0, 0.0);
    if (! checkAngles(conn, 0, -90, "source to the right"))
        return 1;
    source->setPosition(1.0, 1.0, 0.0);
    if (! checkAngles(conn, 0, -45, "source front-right on XY plane"))
        return 1;
    source->setPosition(1.0, 1.0, 1.0);
    if (! checkAngles(conn, 45, -45, "source front-right and up"))
        return 1;
    source->setPosition(1.0, 0.0, 1.0);
    if (! checkAngles(conn, 45, -90, "source right and up"))
        return 1;
    source->setPosition(1.0, -1.0, 1.0);
    if (! checkAngles(conn, 45, -135, "source back-right and up"))
        return 1;
    return 0;
}

