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
 * The vBaseNode class. 
 */

#ifndef __vBaseNode_H
#define __vBaseNode_H

#include <string>
#include <vector>
#include <iostream>
#include <tr1/memory>
#include "vMath.h"

class vSoundConn;

/**
 * Base class for a node in the scene.
 */
class vBaseNode
{
    private:
        // TODO: remove friend classes and provide real getter methods:

        friend class Translator;
        friend class DmitriTranslator;
        friend class vAudioManager;
        friend class vSoundConn;

    public:

        vBaseNode(const std::string &nodeID);
        ~vBaseNode();

        /**
         * Returns the identifier of this node.
         */
        std::string getID() { return id_; }

        /**
         * Prints debug info about this node to the console.
         */
        virtual void debugPrint();

        /**
         * Sets this node's position in the 3D space.
         */
        virtual void setPosition(double x, double y, double z);

        /**
         * Sets this node's orientation.
         */
        virtual void setRotation(double pitch, double roll, double yaw);

    protected:

        std::string id_;

        Vector3 pos_;
        Vector3 rot_;

        bool active_;

        std::vector<std::tr1::shared_ptr<vSoundConn> > connectTO_;
        std::vector<std::tr1::shared_ptr<vSoundConn> > connectFROM_;

        bool updateFlag_;
};


#endif
