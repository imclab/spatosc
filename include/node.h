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

#ifndef __NODE_H__
#define __NODE_H__

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
        // TODO: remove friend classes and provide real getter methods:
        friend class Scene;

    public:
        /**
         * Constructor for a Node object.
         * @param nodeID The identifier for this node.
         * @param scene The Scene in which this node ought to be.
         */
        Node(const std::string &nodeID, Scene& scene);

        /**
         * Virtual classes should have virtual destructors.
         */
        virtual ~Node() {}

        /**
         * Returns the identifier of this node.
         * @return A string to identify this Node.
         */
        std::string getID() const { return id_; }

        /**
         * Prints debug info about this node to the console.
         */
        virtual void debugPrint() const;

        /**
         * Sets this node's position in the 3D cartesian space.
         * @param x Position on the X axis for this node.
         * @param y Position on the Y axis for this node.
         * @param z Position on the Z axis for this node.
         */
        virtual void setPosition(double x, double y, double z);

        /**
         * Sets this node's orientation.
         *
         * Angles are in degrees.
         * 0 degrees is in front of you. 90 is on your right.
         * @param pitch Rotation on the lateral axis (saying "yes")
         * @param roll Rotation on the longitudinal axis (saying "maybe")
         * @param yaw Rotation on the vertical axis (saying "no")
         */
        virtual void setOrientation(double pitch, double roll, double yaw);

        /**
         * Returns this node's position.
         * @return A Vector3 with its position as x, y and z coordinates.
         */
        Vector3 getPosition() const
        {
            return pos_;
        }

    protected:
        void notifyScene();
        std::string id_;
        Scene &scene_;
        Vector3 pos_;
        Vector3 rot_;
        bool active_;
        std::vector<std::tr1::shared_ptr<Connection> > connectTO_;
        std::vector<std::tr1::shared_ptr<Connection> > connectFROM_;
};

} // end namespace spatosc

#endif // __NODE_H__
