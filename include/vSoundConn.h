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
 * The vSoundConn class.
 */
#ifndef __vSoundConn_H
#define __vSoundConn_H

#include <string>
namespace spatosc
{

class vBaseNode;

/**
 * Audio connection between two nodes.
 */
class vSoundConn
{
private:
    // TODO: get rid of friend classes, and add proper getter methods:
    friend class vAudioManager;
    friend class Translator;
    friend class DmitriTranslator;
    friend class vListener;

public:
    
    vSoundConn(vBaseNode *src, vBaseNode *snk);
    /**
     * Update distance, azimuth and elevation values
     */
    void update();

    double distance() const { return distance_; }
    double azimuth() const { return azim_; }
    double elevation() const { return elev_; }
    double gain() const { return gain_; }
    
protected:

    std::string id_;
    
    vBaseNode *src_;
    vBaseNode *snk_;
    
    double distance_;
    double azim_;
    double elev_;
    double gain_;
//    double vdel_;
    float distanceEffect_;
    float rolloffEffect_;
    float dopplerEffect_;
    float diffractionEffect_;
};

};

#endif
