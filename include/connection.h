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

class Node;

/**
 * Audio connection between two nodes.
 */
class Connection
{
private:
    // TODO: get rid of friend classes, and add proper getter methods:
    friend class Scene;
    friend class Translator;
    friend class DmitriTranslator;
    friend class Listener;

public:
    
    Connection(Node *source, Node *sink);
    /**
     * Update distance, azimuth and elevation values (usually called by Scene instance)
     */
    void update();

    /**
     * Returns the distance between the source and sink nodes.
     * 
     * A distance is always positive.
     */
    double distance() const { return distance_; }

    /**
     * Returns the azimuth (horizontal rotation) between the source and the sink node.
     */
    double azimuth() const { return azim_; }

    /**
     * Returns the elevation (vertical angle) between the source and the sink node.
     */
    double elevation() const { return elev_; }

    /**
     * Returns the factor for the gain of the audio for the source as it should be heard by the sink node, 
     * according to distance. 
     * 
     * Are distances in meters? Good question!
     */
    double gain() const { return gain_; }
    
protected:
    std::string id_;
    Node *src_;
    Node *snk_;
    double distance_;
    double azim_;
    double elev_;
    double gain_;
//    double vdel_;
//    FIXME:2010-01-15:aalex: Should thode effects be vector of shared_ptr to objects?
    float distanceEffect_;
    float rolloffEffect_;
    float dopplerEffect_;
    float diffractionEffect_;
};

} // end namespace spatosc

#endif // __CONNECTION_H__
