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

#include <cassert>
#include <iostream>
#include <lo/lo.h>

#include "connection.h"
#include "geotransform.h"
#include "node.h"
#include "oscutils.h"
#include "scene.h"
#include "unused.h"

namespace spatosc
{

Node::Node(const std::string &nodeID, Scene &scene) :
    id_(nodeID),
    scene_(scene),
    orientation_(),
    active_(true),
    nodeChanged_(true),
    pos_(),
    radius_(0.0)
{}

Node::~Node()
{
    // send a scene change, telling the rendere that this node's memory can be
    // cleared:
    scene_.onSceneChanged("ss", "deleteNode", id_.c_str(), SPATOSC_ARGS_END);

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
    std::cout << "    radius:\t" << radius_ << std::endl;
    std::cout << "    active?\t" << active_ << std::endl;
}

void Node::setActive(bool isActive)
{
	active_ = isActive;
	forceNotifyScene();
}

void Node::setPosition(double x, double y, double z)
{
    std::cout << "Node::setPosition " << x << " " << y << " " << z << std::endl;
    if (x != pos_.x || y != pos_.y || z != pos_.z)
    {
        pos_.x = x;
        pos_.y = y;
        pos_.z = z;
        std::cout << x << " " << y << " " << z << std::endl;
        notifyScene();
    }
    else
    {
        std::cout << "same as before!!!!!!!!!!!!!!!" << std::endl;
        std::cout << pos_.x << " " << pos_.y << " " << pos_.z << std::endl;
    }
}

Vector3 Node::getPosition() const
{
    // TODO: Thu Feb 10 14:51:02 EST 2011: tmatth: cache these values
    Vector3 result(pos_);
    scene_.applyTransformation(result);
    return result;
}

void Node::setPositionAED(double angle, double elevation, double distance)
{
    Vector3 xyz = sphericalToCartesian(Vector3(angle, elevation, distance));
    std::cout << "Node::setPositionAED " << angle << " " << elevation << " " << distance << std::endl;
    setPosition(xyz.x, xyz.y, xyz.z);
}

void Node::setRadius(double r)
{
    if (r != radius_)
    {
        radius_ = r;
        notifyScene();
    }
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
    nodeChanged_ = true;
    onNodeChanged(); // let subclasses decide what to do
}

void Node::forceNotifyScene()
{
    nodeChanged_ = true;
    onNodeChanged(true); // let subclasses decide what to do
}

// FIXME:Sun Feb 13 12:11:29 EST 2011:tmatth:put this in a separate component
namespace {
bool correctNumberOfArguments(const std::string &method, int expected, int actual)
{
    if (actual != expected)
    {
        std::cerr << method << " expects " << expected << " arguments , got " <<
            actual << std::endl;
        return false;
    }
    else
        return true;
}
} // end anonymous namespace

void Node::handleMessage(const std::string &method, int argc, lo_arg **argv, const char *types)
{
    using namespace OSCutil; // typeTagsMatch
    if (method == "setActive")
    {
        if (typeTagsMatch(types, "i"))
            setActive((bool)argv[0]->i);
    }
    else if (method == "xyz")
    {
        if (typeTagsMatch(types, "fff"))
            setPosition(argv[0]->f, argv[1]->f, argv[2]->f);
    }
    else if (method == "setStringProperty")
    {
    	if (typeTagsMatch(types, "ss"))
            setStringProperty(std::string(static_cast<const char *>(&argv[0]->s)), std::string(static_cast<const char *>(&argv[1]->s)));
    }
    else if (method == "setIntProperty")
    {
        if (argc==2)
        {
            int i = (int)lo_hires_val((lo_type)types[1], argv[1]);
            setIntProperty(std::string(static_cast<const char *>(&argv[0]->s)), i);
        }
    }
    else if (method == "setFloatProperty")
    {
        if (argc==2)
        {
            double f = (double)lo_hires_val((lo_type)types[1], argv[1]);
            setFloatProperty(std::string(static_cast<const char *>(&argv[0]->s)), f);
        }
    }
    else if (method == "aed")
    {
    	if (typeTagsMatch(types, "fff"))
            setPositionAED(argv[0]->f, argv[1]->f, argv[2]->f);
    }
    else if (method == "xy")
        std::cerr << method << " NOT IMPLEMENTED" << std::endl;
    else if (method == "delay")
        std::cerr << method << " NOT IMPLEMENTED" << std::endl;
    else if (method == "gain")
        std::cerr << method << " NOT IMPLEMENTED" << std::endl;
    else if (method == "gainDB")
        std::cerr << method << " NOT IMPLEMENTED" << std::endl;
    else if (method == "spread")
        std::cerr << method << " NOT IMPLEMENTED" << std::endl;
    else if (method == "spreadAE")
        std::cerr << method << " NOT IMPLEMENTED" << std::endl;
    else
    {
        // delegate to derived classes
        if (! handleMessage_(method, argc, argv, types))
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

void Node::setStringProperty(const std::string &key, const std::string &value)
{
    string_properties_.setPropertyValue(key, value);
    scene_.onPropertyChanged<std::string>(this, key, value);
}

bool Node::getStringProperty(const std::string &key, std::string &value) const
{
    return string_properties_.getPropertyValue(key, value);
}

bool Node::removeStringProperty(const std::string &key)
{
    return string_properties_.removeProperty(key);
}

void Node::setFloatProperty(const std::string &key, const double &value)
{
    float_properties_.setPropertyValue(key, value);
    scene_.onPropertyChanged<double>(this, key, value);
}

bool Node::getFloatProperty(const std::string &key, double &value) const
{
    return float_properties_.getPropertyValue(key, value);
}

bool Node::removeFloatProperty(const std::string &key)
{
    return float_properties_.removeProperty(key);
}

void Node::setIntProperty(const std::string &key, const int &value)
{
    int_properties_.setPropertyValue(key, value);
    scene_.onPropertyChanged<int>(this, key, value);
}

bool Node::getIntProperty(const std::string &key, int &value) const
{
    return int_properties_.getPropertyValue(key, value);
}

bool Node::removeIntProperty(const std::string &key)
{
    return int_properties_.removeProperty(key);
}

} // end namespace spatosc
