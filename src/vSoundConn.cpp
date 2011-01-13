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

#include "vSoundConn.h"
#include "vBaseNode.h"

// *****************************************************************************

vSoundConn::vSoundConn(vBaseNode *src, vBaseNode *snk) : 
    src_(src), 
    snk_(snk), 
    distanceEffect_(100.0), 
    rolloffEffect_(100.0),
    dopplerEffect_(100.0),
    diffractionEffect_(100.0)
{
    id_ = src->id_ + "-" + snk->id_ + ".conn";
    update(); // calculate and store distance, azimuth and elevation

    // set updateFlag on at least one of the nodes for initial computation:
    src->updateFlag_ = true;
}

void vSoundConn::update()
{
    if (src_->updateFlag_ or snk_->updateFlag_)
    {
        Vector3 vect = snk_->pos_ - src_->pos_;
        distance_ = static_cast<double>(vect.Mag());
        distanceScalar_ = 1 / (1.0 + pow(distance_, static_cast<double>(distanceEffect_) * 0.01));
        azim_ = atan2(vect.y, vect.x);
        elev_ = atan2(sqrt(pow(vect.x, 2) + pow(vect.y, 2)), vect.z);
    }
}

vSoundConn::~vSoundConn()
{
    // destructor
}
