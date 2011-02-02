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

#include "connection.h"
#include "soundsource.h"
#include "listener.h"
#include <iostream>

namespace spatosc
{

Connection::Connection(SoundSource *source, Listener *sink) :
    id_(source->getID() + "-" + sink->getID() + ".conn"),
    src_(source),
    snk_(sink),
    distance_(0.0),
    azim_(0.0),
    elev_(0.0),
    gain_(0.0),
    gainDB_(0.0),
    vdel_(0.0),
    distanceEffect_(100.0),
    rolloffEffect_(100.0),
    dopplerEffect_(100.0),
    diffractionEffect_(100.0)
{
    // calculate and store distance, azimuth and elevation
    recomputeConnection();
}

void Connection::recomputeConnection()
{
    Vector3 vect = src_->getPosition() - snk_->getPosition();
    distance_ = static_cast<double>(vect.Mag());
    double distanceScalar = 1 / (1.0 + pow(distance_, static_cast<double>(distanceEffect_) * 0.01));
    azim_ = atan2(vect.y, vect.x);
    elev_ = atan2(sqrt(pow(vect.x, 2) + pow(vect.y, 2)), vect.z);
    // for now, force sources to be above equator
    elev_ = std::max(elev_, 0.0);
    // now from distance, compute gain and variable delay:
    vdel_ = distance_ * (1 / SPEED_OF_SOUND) * .01 * dopplerEffect_;
    gainDB_ = 20 * log10(distanceScalar);
}

bool Connection::active() const
{
    return src_->active() && snk_->active();
}

void Connection::debugPrint() const
{
    std::cout << "  Connection " << id_ << ":" << std::endl;
    std::cout << "    distanceEffect:\t" << distanceEffect_ << "%" << std::endl;
    std::cout << "    rolloffEffect:\t" << rolloffEffect_ << "%" << std::endl;
    std::cout << "    dopplerEffect:\t" << dopplerEffect_ << "%" << std::endl;
    std::cout << "    diffractionEffect:\t" << diffractionEffect_ << "%" << std::endl;
}

} // end namespace spatosc
