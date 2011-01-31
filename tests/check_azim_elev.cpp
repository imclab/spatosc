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
#include <tr1/memory>
#include "spatosc.h"

static const bool VERBOSE = true;

#define EPSILON 0.0001
static int checkAngles(spatosc::Connection *conn, double azim, double elev, const char *description="")
{
	double dAzim = abs(conn->azimuth()*TO_DEGREES - azim);
	double dElev = abs(conn->elevation()*TO_DEGREES - elev);

	bool success = ( (dAzim < EPSILON) && (dElev < EPSILON) );

	if (VERBOSE)
	{
		std::cout << "Checking azim-elev angles: " << description << std::endl;
		std::cout << "  got: "
			<< conn->azimuth()*TO_DEGREES << ","<< conn->elevation()*TO_DEGREES
			<< ", expecting: " << azim << "," << elev
			<< " ... deltas: " << dAzim << "," << dElev
			<< " so success = " << success << std::endl << std::endl;
	}

	if (success) return 0;
	else return 1;
}

int main(int /*argc*/, char ** /*argv*/)
{
    using std::tr1::shared_ptr;
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

    int fail = 0;

    source->setPosition(0.0, 1.0, 0.0);
    if (checkAngles(conn, 0, 0, "source in front")) fail = 1;

    source->setPosition(0.0, -1.0, 0.0);
    if (checkAngles(conn, 0, 180, "source behind")) fail = 1;

    source->setPosition(0.0, 0.0, 1.0);
    if (checkAngles(conn, 0, 90, "source up above")) fail = 1;

    source->setPosition(1.0, 0.0, 0.0);
    if (checkAngles(conn, 0, -90, "source to the right")) fail = 1;

    source->setPosition(1.0, 1.0, 0.0);
    if (checkAngles(conn, 0, -45, "source front-right on XY plane")) fail = 1;

    source->setPosition(1.0, 1.0, 1.0);
    if (checkAngles(conn, 45, -45, "source front-right and up")) fail = 1;

    source->setPosition(1.0, 0.0, 1.0);
    if (checkAngles(conn, 45, -90, "source right and up")) fail = 1;

    source->setPosition(1.0, -1.0, 1.0);
    if (checkAngles(conn, 45, -135, "source back-right and up")) fail = 1;


    return fail;
}

