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
    Vector3 connVec = src_->getPosition() - snk_->getPosition();
    distance_ = static_cast<double>(connVec.Mag());
    double distanceScalar = 1 / (1.0 + pow(distance_, static_cast<double>(distanceEffect_) * 0.01));

    Quaternion snkQuat = EulerToQuat(snk_->getOrientation());
    Vector3 snkDir = snkQuat * Vector3(0.0, 1.0, 0.0);

    elev_ = AngleBetweenVectors(snkDir, connVec, 2);
    azim_ = AngleBetweenVectors(snkDir, connVec, 3);

    // ensure elevation is in range [-pi,pi]
	/*
    if (elev_ < -M_PI/2)
    {
    	elev_ += M_PI;
    	if (azim_>0) azim_ -= M_PI;
    	else azim_ += M_PI;
    }
    else if (elev_ > M_PI/2)
    {
    	elev_ -= M_PI;
    	if (azim_>0) azim_ -= M_PI;
    	else azim_ += M_PI;
    }
    */

    /*
	std::cout << "src: "<<src_->getPosition() << ", snk: "<<snk_->getPosition() << std::endl;
	std::cout << "connVec: "<<connVec << ", length=" << distance_ << std::endl;
	std::cout << "snkOrient = " << snk_->getOrientation() << std::endl;
	std::cout << "snkQuat = " << snkQuat << std::endl;
	std::cout << "QuatToEuler = " << QuatToEuler(RotationBetweenVectors(snkDir,connVec)) << std::endl;
	std::cout << "snkDir  = " << snkDir << std::endl;
	std::cout << "azim = " << azim_*TO_DEGREES << " elev = " << elev_*TO_DEGREES << std::endl;
    */

    // for now, force sources to be above equator
    //elev_ = std::max(elev_, 0.0);

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

void Connection::handleMessage(const std::string &method, int argc, lo_arg **argv)
{
    if (method == "aed")
    {
        assert(argc == 3);
        azim_ = argv[0]->f;
        elev_ = argv[1]->f;
        distance_ = argv[2]->f;
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
