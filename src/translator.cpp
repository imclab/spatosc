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

#include "translator.h"
#include "connection.h"
#include "soundsource.h"
#include "listener.h"

namespace spatosc
{

Translator::Translator(bool verbose) : verbose_(verbose)
{}

void Translator::pushOSCMessages(Connection *conn)
{
    if (verbose_)
        std::cout << "Computation update for " << conn->getSource()->getID() << " -> " << conn->getSink()->getID() << " :" <<std::endl;

    /*
    // SRC INCIDENCE:
    Vector3 srcDir = EulerToQuat(conn->src->rot) * Vector3(0,1,0);
    double srcIncidence = AngleBetweenVectors(srcDir, vect);
    double srcIncidenceGain = conn->src->rolloff->getValue( (srcIncidence * conn->src->spread) / M_PI );

    // SNK INCIDENCE:
    Vector3 snkDir = EulerToQuat(conn->snk->rot) * Vector3(0,1,0);
    double snkIncidence = AngleBetweenVectors(Vector3(0,0,0)-snkDir, vect);
    double snkIncidenceGain = conn->snk->rolloff->getValue( (snkIncidence * conn->snk->spread) / M_PI );

    // final incidence scalars with connection's rolloffEffect applied:
    double srcScalar = (double) (1.0 - (.01*xconn->rolloffEffect  * (1.0 - srcIncidenceGain)));
    double snkScalar = (double) (1.0 - (.01*xconn->rolloffEffect  * (1.0 - snkIncidenceGain)));
     */

    if (verbose_)
    {
        std::cout << "  dist:\t" << conn->distance() << std::endl;
        std::cout << "  azim:\t" << conn->azimuth() << std::endl;
        std::cout << "  elev:\t" << conn->elevation() << std::endl;
        std::cout << "  gain:\t" << conn->gainDB() << " dB" << std::endl;
    }
}

} // end namespace spatosc

