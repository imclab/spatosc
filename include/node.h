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
#include <lo/lo_osc_types.h> // for lo_arg
#include "maths.h"
#include "memory.h"
#include "properties.h"

namespace spatosc
{

class Connection;
class Scene;

/**
 * Base class for a node in the scene.
 */
class Node
{
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
        virtual ~Node();

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
         * Activates (or deactivates) the node. When deactivated, no connections
         * involving this node will be computed and sent via OSC. This is useful
         * to reduce network load and processing for large scenes (eg, when a
         * node is far away or otherwise culled).
         */
        void setActive(bool isActive);


        /**
         * Sets this node's position in the 3D cartesian space.
         *
         * Distances are in meters.
         *
         * The default coordinate space in this library is left-handed.
         * On the horizontal axis, +X is on the right.
         * On the longitudinal axis, +Y is in the front.
         * On the vertical axis, +Z is up.
         *
         * We use a strictly navigational coordinate system, where zero de- grees azimuth are due front, the y-axis points to the front, the x-axis to the right, the z-axis upwards, horizontal angles rotate in a clockwise fashion and vertical angles increase from the horizontal plane. This coordinate system is commonly used in electro-acoustic composition tools, most notably in IRCAM`s Spat and has recently become one of the supported coordi nate system of SpatDIF.
         *
         * @param x Position on the X axis for this node.
         * @param y Position on the Y axis for this node.
         * @param z Position on the Z axis for this node.
         */
        virtual void setPosition(double x, double y, double z);
        /**
         * Sets the node's position using spherical coordinates.
         * @see sphericalToCartesian()
         */
        void setPositionAED(double angle, double elevation, double distance);

        /**
         * Sets this node's orientation.
         *
         * The orientation of a Node means the direction in which it is facing.
         * For SoundSource nodes, this is quite useful if their directivity is omnidirectional.
         * For Listener nodes, it sounds like it rotates the whole world around them, of course.
         *
         * Angles are in degrees.
         * 0 degrees is in front of the node. 90 is on its right.
         *
         * The default coordinate space in this library is left-handed.
         * A rotation around the Z axis is positive when it is clockwise from a bird's eye view.
         *
         * Pitch, roll and yaw are computed in this order. These are Euler's angles.
         *
         * @param pitch Rotation on the lateral (X) axis. (saying "yes")
         * @param roll Rotation on the longitudinal (Y) axis. (saying "maybe")
         * @param yaw Rotation on the vertical (Z) axis. (saying "no")
         */
        virtual void setOrientation(double pitch, double roll, double yaw);

        /**
         * Returns this node's position, which may be transformed by a geotransform.
         * @return A Vector3 with its position as x, y and z coordinates.
         */
        Vector3 getPosition() const;

        /**
         * Returns this node's orientation.
         * @return A Vector3 with pitch, roll, and yaw (in radians)
         *
         */
        Quaternion getOrientation() const
        {
            return orientation_;
        }

        /**
         * Returns true if this node's new position must be sent
         * @return boolean
         */
        bool shouldSendNewState() const
        {
            return nodeChanged_;
        }

        /**
         * Notify this node that it's new position has been sent
         */
        void stateSent()
        {
            nodeChanged_ = false;
        }

        virtual void handleMessage(const std::string &method, int argc, lo_arg ** argv, const char *types);
        bool active() const { return active_; }
        bool hasID(const std::string &id) const;
        friend std::ostream &operator<<(std::ostream &out, const Node &n);

        /**
         * Sets a string property for this node.
         * Creates it if it does not exist.
         */
        void setStringProperty(const std::string &key, const std::string &value);

        /**
         * Retrieves a string property value for this node.
         * @return Success.
         */
        bool getStringProperty(const std::string &key, std::string &value) const;

        /**
         * Removes a property.
         * @return Whether it deleted it, or false if it was not there.
         */
        bool removeStringProperty(const std::string &key);

    protected:
        void forceNotifyScene();
        void notifyScene();
        std::string id_;
        Scene &scene_;
        Quaternion orientation_;
        bool active_;
        bool nodeChanged_;
        // FIXME: Thu Jan 27 15:03:58 EST 2011 :tmatth:
        // A source shouldn't have a connectFROM_ and a
        // sink should not have a connectTO_
    private:
        Vector3 pos_;
        virtual void onNodeChanged(bool forcedNotify=false) = 0;
        virtual bool handleMessage_(const std::string &method, int argc, lo_arg ** argv, const char *types) = 0;
        Properties<std::string> properties_;
};

} // end namespace spatosc

#endif // __NODE_H__
