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

#include "vPlugin.h"
#include "vSoundConn.h"
#include "vBaseNode.h"

std::string vPlugin::getTypeString() const
{
    return "CONSOLE";
}


void vPlugin::update(vSoundConn *conn)
{
    std::cout << "Computation update for " << conn->src_->id_ << " -> " << conn->snk_->id_ << " :" <<std::endl;

    Vector3 vect = conn->snk_->pos_ - conn->src_->pos_;
    double distance = (double)vect.Mag();
    double azim = atan2(vect.y, vect.x);
    double elev = atan2(sqrt(pow(vect.x,2) + pow(vect.y,2)), vect.z) / (M_PI/2);
    if (elev < 0) elev = 0.0; // for now, force sources to be above equator

    double distanceScalar = 1 / (1.0 + pow(distance, (double)conn->distanceEffect_ * 0.01));

    double vdel = distance * (1/SPEED_OF_SOUND) * .01 * conn->dopplerEffect_;  // speed of sound
    double gain = 20 * log10(distanceScalar);

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

    std::cout << "  dist:\t" << distance << std::endl;
    std::cout << "  azim:\t" << azim << std::endl;
    std::cout << "  elev:\t" << elev << std::endl;

    std::cout << "  gain:\t" << gain << " dB" << std::endl;
    std::cout << "  delay:\t" << vdel << " ms" << std::endl;

}
