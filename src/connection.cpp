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
#include <cassert>

namespace spatosc
{

Connection::Connection(SoundSource *source, Listener *sink) :
    id_(source->getID() + "->" + sink->getID()),
    src_(source),
    snk_(sink),
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
    Vector3 connVec = src_->getPosition() - snk_->getPosition();
    Vector3 snkDir = snk_->getOrientation() * Vector3(0.0, 1.0, 0.0);

	// Rotate connVec by the negative of the rotation described by the snk's
	// orientation. To do this, just flip the sign of the quat.w:
	Quaternion negRot = snk_->getOrientation();
	negRot.w *= -1;
	Vector3 rotConnVec = negRot * connVec;
	aed_ = cartesianToSpherical(rotConnVec);

	/*
	std::cout << "src: "<<src_->getPosition() << ", snk: "<<snk_->getPosition() << std::endl;
    std::cout << "snkOrient = " << snk_->getOrientation() << std::endl;
    std::cout << "snkDir  = " << snkDir << std::endl;
	std::cout << "rotConnVec: "<<rotConnVec << ", length=" << distance() << std::endl;
	std::cout << "AED: " << azimuth()*TO_DEGREES <<","<< elevation()*TO_DEGREES <<","<< distance() << std::endl;
	*/
	/*
	Quaternion q = RotationBetweenVectors(snkDir,connVec);		
	std::cout << "QuatToEuler? = " << QuatToEuler(q)*TO_DEGREES << std::endl;
	*/


    // now from distance, compute gain and variable delay:
   	double distanceScalar = 1 / (1.0 + pow(distance(), static_cast<double>(distanceEffect_) * 0.01));
    vdel_ = distance() * (1 / SPEED_OF_SOUND) * .01 * dopplerEffect_;
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

void Connection::handleMessage(const std::string &method, int argc, lo_arg **argv)
{
    if (method == "aed")
    {
        assert(argc == 3);
        aed_.x = argv[0]->f;
        aed_.y = argv[1]->f;
        aed_.z = argv[2]->f;
    }
    else if (method == "delay")
    {
        assert(argc == 1);
        vdel_ = argv[0]->f;
    }
    else if (method == "gain")
    {
        assert(argc == 1);
        gain_ = argv[0]->f;
    }
    else if (method == "gainDB")
    {
        assert(argc == 1);
        gainDB_ = argv[0]->f;
    }
    else
        std::cerr << "Unknown method " << method << std::endl;
}

} // end namespace spatosc
