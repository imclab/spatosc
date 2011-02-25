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
 * The Connection class.
 */
#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <string>
#include <lo/lo_osc_types.h> // for lo_arg
#include "maths.h"

namespace spatosc
{

class SoundSource;
class Listener;

/**
 * Audio connection between two nodes.
 */
class Connection
{
public:
    Connection(SoundSource *source, Listener *sink);
    /**
     * Update distance, azimuth and elevation values (usually called by Scene instance)
     */
    void recomputeConnection();

    /**
     * Sets the distance factor for this connection as a percentage, affecting
     * the attenuation of gain with respect to distance. This is an exponential
     * decay factor according to the formula:
     *
     * \f$\mbox{gain(linear)} = \frac{1}{1 - \mbox{distance}^f}\f$
     *
     * Thus, a value of 0% means no gain attenuation, a factor of 100% results
     * in linear gain (default), and 200% is the natural inverse square law.
     */
    void setDistanceFactor(double f);

    /**
     * Sets the doppler factor for this connection as a percentage. A value of
     * 0% means no Doppler, a value of 100% provides realistic / natural delays
     * according to the speed of sound. NOTE: it is possible to provide values
     * above 100% to create a hyper-doppler effect (useful for dramatic effect).
     */
    void setDopplerFactor(double f);

    /**
     * Sets the rolloff factor for this connection as a percentage, affecting
     * the attenuation of gain with respect to the angular rolloff from a the
     * source orientation. A value of 05 means no gain attenuation, while 100%
     * results in full attenuation according to the rolloff table.
     *
     * NOTE: ROLLOFFS NOT YET IMPLEMENTED
     */
    void setRolloffFactor(double f);

    /**
     * Returns the distance between the source and sink nodes.
     *
     * A distance is always positive.
     * @return Distance in meters between its two nodes.
     */
    double distance() const { return aed_.z; }

    /**
     * Returns the azimuth (horizontal rotation) between the source and the sink node.
     * @return Angle value in radians.
     */
    double azimuth() const { return aed_.x; }

    /**
     * Returns the elevation (vertical angle) between the source and the sink node.
     * @return Angle value in radians.
     */
    double elevation() const { return aed_.y; }

    /**
     * Returns the factor for the gain of the audio for the source as it should be heard by the sink node,
     * according to distance.
     *
     * Distances are in meters.
     * @return Linear gain between 0 and 1.
     */
    double gain() const { return gain_; }

    /**
     * Returns the logarithmic gain in dB.
     *
     * @return Gain in dB.
     */
    double gainDB() const { return gainDB_; }

    /**
     * Returns the delay due to travel time between the source and sink. This
     * assumes position units are in meters, and provides a delay time in
     * milliseconds.
     *
     * @return Delay in milliseconds.
     */
    double delay() const { return vdel_; }

    /**
     * Returns a pointer to its source Node.
     * @return A Node pointer. Never free this pointer. It might become invalid if the node is deleted.
     */
    SoundSource *getSource() const { return src_; }

    /**
     * Returns a pointer to its sink Node.
     * @return A Node pointer. Never free this pointer. It might become invalid if the node is deleted.
     */
    Listener *getSink() const { return snk_; }

    std::string getID() const { return id_; }
    void debugPrint() const;

    /**
     * Returns true if source and sink are active
     */
    bool active() const;

    void handleMessage(const std::string &method, int argc, lo_arg ** argv);

private:
    std::string id_;
    SoundSource *src_;
    Listener *snk_;
	Vector3 aed_;
    double gain_;
    double gainDB_;
    double vdel_;
    float distanceFactor_;
    float rolloffFactor_;
    float dopplerFactor_;
    Connection(const Connection&);
    const Connection& operator=(const Connection&);
};

} // end namespace spatosc

#endif // __CONNECTION_H__
