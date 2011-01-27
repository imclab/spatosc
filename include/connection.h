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
namespace spatosc
{

class SoundSource;
class Listener;

/**
 * Audio connection between two nodes.
 */
class Connection
{
private:
    // TODO: get rid of friend classes, and add proper getter methods:
    friend class Scene;
    friend class Translator;
    friend class SpatdifTranslator;
    friend class DmitriTranslator;
    friend class Listener;

    // not implemented
    Connection(const Connection&);
    const Connection& operator=(const Connection&);

public:

    Connection(SoundSource *source, Listener *sink);
    /**
     * Update distance, azimuth and elevation values (usually called by Scene instance)
     */
    void recomputeConnection();

    /**
     * Returns the distance between the source and sink nodes.
     *
     * A distance is always positive.
     * @return Distance in meters between its two nodes.
     */
    double distance() const { return distance_; }

    /**
     * Returns the azimuth (horizontal rotation) between the source and the sink node.
     * @return Angle value in radians.
     */
    double azimuth() const { return azim_; }

    /**
     * Returns the elevation (vertical angle) between the source and the sink node.
     * @return Angle value in radians.
     */
    double elevation() const { return elev_; }

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
     * Returns a reference to its source Node.
     * @return A Node.
     */
    SoundSource& getSource() const { return *src_; }

    /**
     * Returns a reference to its sink Node.
     * @return A Node.
     */
    Listener& getSink() const { return *snk_; }

protected:
    std::string id_;
    SoundSource *src_;
    Listener *snk_;
    double distance_;
    double azim_;
    double elev_;
    double gain_;
    double gainDB_;
    double vdel_;
//    FIXME:2010-01-15:aalex: Should thode effects be vector of shared_ptr to objects?
    float distanceEffect_;
    float rolloffEffect_;
    float dopplerEffect_;
    float diffractionEffect_;
};

} // end namespace spatosc

#endif // __CONNECTION_H__
