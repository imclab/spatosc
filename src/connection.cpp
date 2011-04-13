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
#include "oscutils.h"
#include "listener.h"
#include <iostream>
#include <cassert>

namespace spatosc
{

Connection::Connection(SoundSource *source, Listener *sink) :
    id_(source->getID() + "->" + sink->getID()),
    src_(source),
    snk_(sink),
    aed_(),
    gain_(0.0),
    gainDB_(0.0),
    vdel_(0.0),
    distanceFactor_(100.0),
    rolloffFactor_(100.0),
    dopplerFactor_(100.0)
{
    // calculate and store distance, azimuth and elevation
    recomputeConnection();
}

Connection::~Connection()
{
    // TODO: there is no pointer to scene_ available, so this has now been
    // moved into the scene::disconnect method. It would be better to have it
    // here because there are several ways to disconnect things.
    //scene_.onSceneChanged("sss", "disconnect", src_->getID().c_str(), snk_->getID().c_str(), SPATOSC_ARGS_END);
}

void Connection::recomputeConnection()
{
	if (active())
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
		Quaternion q = RotationBetweenVectors(snkDir,connVec);
		std::cout << "QuatToEuler? = " << QuatToEuler(q)*TO_DEGREES << std::endl;
		*/


		// now from distance, compute gain and variable delay:
		double distanceScalar = 1 / (1.0 + pow(distance(), static_cast<double>(distanceFactor_) * 0.01));
		vdel_ = distance() * (1 / SPEED_OF_SOUND) * .01 * dopplerFactor_;
		gainDB_ = 20 * log10(distanceScalar);

	}


	else
	{
		// when connection is not active (ie, one of the nodes has been
		// deactivated, we set the gain to silence (-inf, so -100)
		gainDB_ = -100;
	}

}

void Connection::setDistanceFactor(double f)
{
	if (f<0) f = 0.0;
	distanceFactor_ = f;
	recomputeConnection();
}

void Connection::setDopplerFactor(double f)
{
	if (f<0) f = 0.0;
	dopplerFactor_ = f;
	recomputeConnection();
}

void Connection::setRolloffFactor(double f)
{
	if (f<0) f = 0.0;
	rolloffFactor_ = f;
	recomputeConnection();
}

void Connection::mute()
{
	gainDB_ = -100;
}

void Connection::unmute()
{

}

bool Connection::active() const
{
    return src_->active() && snk_->active();
}

void Connection::debugPrint() const
{
    std::cout << "  Connection " << id_ << ":" << std::endl;
    std::cout << "    azim,elev:\t" << aed_.x*TO_DEGREES <<","<< aed_.y*TO_DEGREES << std::endl;
    std::cout << "    distance:\t" << aed_.z << std::endl;
    std::cout << "    gain:\t" << gainDB_ << "dB" << std::endl;
    std::cout << "    delay:\t" << vdel_ << "ms" << std::endl;
    std::cout << "    distanceFactor:\t" << distanceFactor_ << "%" << std::endl;
    std::cout << "    rolloffFactor:\t" << rolloffFactor_ << "%" << std::endl;
    std::cout << "    dopplerFactor:\t" << dopplerFactor_ << "%" << std::endl;
}

// TODO: need to provide types as well
void Connection::handleMessage(const std::string &method, lo_arg **argv, const char *types)
{
    using namespace OSCutil; // typeTagsMatch
    if (method == "aed")
    {
        if (typeTagsMatch(types, "fff"))
        {
            aed_.x = argv[0]->f;
            aed_.y = argv[1]->f;
            aed_.z = argv[2]->f;
        }
    }
    else if (method == "delay")
    {
        if (typeTagsMatch(types, "f"))
            vdel_ = argv[0]->f;
    }
    else if (method == "gain")
    {
        if (typeTagsMatch(types, "f"))
            gain_ = argv[0]->f;
    }
    else if (method == "gainDB")
    {
        if (typeTagsMatch(types, "f"))
            gainDB_ = argv[0]->f;
    }
    // FIXME: need types!
    /*
       else if (method == "setDistanceFactor")
       {
       if (typeTagsMatch(types, "f")) setDistanceFactor(argv[0]->f);
       }
       else if (method == "setDopplerFactor")
       {
       if (typeTagsMatch(types, "f")) setDopplerFactor(argv[0]->f);
       }
       else if (method == "setRolloffFactor")
       {
       if (typeTagsMatch(types, "f")) setRolloffFactor(argv[0]->f);
       }
     */
       else
           std::cerr << "Unknown method " << method << std::endl;
}

} // end namespace spatosc
