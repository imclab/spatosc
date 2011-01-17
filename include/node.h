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
 * The Node class. 
 */

#ifndef __Node_H
#define __Node_H

#include <string>
#include <vector>
#include <iostream>
#include <tr1/memory>
#include "maths.h"

namespace spatosc
{
class Connection;
class Scene;

/**
 * Base class for a node in the scene.
 */
class Node
{
    private:
        // TODO: remove friend classes and provide real getter methods:

        friend class Scene;

    public:
        /**
         * Constructor for a Node object.
         * \param nodeID The identifier for this node.
         * \param scene The Scene in which this node ought to be.
         */
        Node(const std::string &nodeID, Scene& scene);

        /**
         * Returns the identifier of this node.
         */
        std::string getID() const { return id_; }

        /**
         * Prints debug info about this node to the console.
         */
        virtual void debugPrint() const;

        /**
         * Sets this node's position in the 3D cartesian space.
         * \param x Position on the X axis for this node.
         * \param y Position on the Y axis for this node.
         * \param z Position on the Z axis for this node.
         */
        virtual void setPosition(double x, double y, double z);

        /**
         * Sets this node's orientation.
         * \param pitch Rotation on the lateral axis (saying "yes")
         * \param roll Rotation on the longitudinal axis (saying "maybe")
         * \param yaw Rotation on the vertical axis (saying "no")
         */
        virtual void setRotation(double pitch, double roll, double yaw);

        /**
         * Returns this node's position.
         */
        Vector3 getPosition() const { return pos_; } // TODO: 2010-01-17:aalex: is copying this object OK?
        
        /**
         * Call this to make sure this node's position will be updated next time the scene nodes positions are calculated.
         * \param should_be_updated Wheter or not it should be updated.
         */
        void setHasChanged(bool should_be_updated) { updateFlag_ = should_be_updated; }

        /**
         * Returns whether or not this node's position should be updated.
         */
        bool hasChanged() const { return updateFlag_; }

    protected:

        std::string id_;
        Scene &scene_;

        Vector3 pos_;
        Vector3 rot_;

        bool active_;

        std::vector<std::tr1::shared_ptr<Connection> > connectTO_;
        std::vector<std::tr1::shared_ptr<Connection> > connectFROM_;

        bool updateFlag_;
};

} // end namespace spatosc

#endif
