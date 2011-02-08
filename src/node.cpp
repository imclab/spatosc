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

#include "node.h"
#include <cassert>
#include <iostream>
#include <lo/lo.h>
#include "scene.h"
#include "connection.h"
#include "geotransform.h"


namespace spatosc
{

Node::Node(const std::string &nodeID, Scene &scene) :
    id_(nodeID),
    scene_(scene),
    pos_(),
    orientation_(),
    active_(true),
    sendNewPosition_(true)
{
    // we call this in case a geotransform has already been applied to the scene
    setPosition(0.0, 0.0, 0.0);
    setOrientation(0.0, 0.0, 0.0);
}

Node::~Node()
{
    // FIXME: Tue Feb  8 11:56:52 EST 2011:tmatth
    // only osc-enabled scenes should have to be unsubscribed.
    // remove osc handler for this node
    scene_.unsubscribe(this);
}

void Node::debugPrint() const
{
    std::cout << "  " << id_ << ":" << std::endl;
    std::cout << "    pos:\t" << pos_.x << "," << pos_.y << "," << pos_.z << std::endl;
    std::cout << "    rot:\t" << orientation_.x << "," << orientation_.y << "," << orientation_.z << std::endl;
    std::cout << "    active?\t" << active_ << std::endl;
}

void Node::setPosition(double x, double y, double z)
{
    scene_.getTransform().apply(x, y, z);
    if (x != pos_.x || y != pos_.y || z != pos_.z)
    {
        pos_.x = x;
        pos_.y = y;
        pos_.z = z;
        notifyScene();
    }
}

void Node::setPositionAED(double angle, double elevation, double distance)
{
    Vector3 xyz = sphericalToCartesian(Vector3(angle, elevation, distance));
    setPosition(xyz.x, xyz.y, xyz.z);
}

void Node::setOrientation(double pitch, double roll, double yaw)
{
    Quaternion quat(EulerToQuat(Vector3(pitch, roll, yaw)));
    if (quat.x != orientation_.x || quat.y != orientation_.y || quat.z != orientation_.z || quat.w != orientation_.w)
    {
        orientation_ = quat;
        notifyScene();
    }
}

void Node::notifyScene()
{
    sendNewPosition_ = true;
    onNodeChanged(); // let subclasses decide what to do
}


void Node::handleMessage(const std::string &method, int argc, lo_arg **argv)
{
    if (method == "xyz")
    {
        assert(argc == 3);
        setPosition(argv[0]->f, argv[1]->f, argv[2]->f);
    }
    else if (method == "aed")
    {
        //assert(argc == 3);
        //aed(argv[0]->f, argv[1]->f, argv[2]->f);
        std::cerr << method << " NOT IMPLEMENTED" << std::endl;
    }
    else if (method == "xy")
    {
        //assert(argc == 2);
        //xy(argv[0]->f, argv[1]->f);
        std::cerr << method << " NOT IMPLEMENTED" << std::endl;
    }
    else if (method == "delay")
    {
        //assert(argc == 1);
        //delay(argv[0]->f);
        std::cerr << method << " NOT IMPLEMENTED" << std::endl;
    }
    else if (method == "gain")
    {
        //assert(argc == 1);
        //gain(argv[0]->f);
        std::cerr << method << " NOT IMPLEMENTED" << std::endl;
    }
    else if (method == "gainDB")
    {
        //assert(argc == 1);
        //gainDB(argv[0]->f);
        std::cerr << method << " NOT IMPLEMENTED" << std::endl;
    }
    else if (method == "spread")
    {
        //assert(argc == 1);
        //spread(argv[0]->f);
        std::cerr << method << " NOT IMPLEMENTED" << std::endl;
    }
    else if (method == "spreadAE")
    {
        //assert(argc == 2);
        //spreadAE(argv[0]->f, argv[1]->f);
        std::cerr << method << " NOT IMPLEMENTED" << std::endl;
    }
    else
    {
        // delegate to derived classes
        if (not handleMessage_(method, argc, argv))
            std::cerr << "Unknown method " << method << std::endl;
    }
}

bool Node::hasID(const std::string &id) const
{
    return id_ == id;
}

std::ostream &operator<<(std::ostream &out, const spatosc::Node &n)
{
    return out << n.id_;
}

} // end namespace spatosc
