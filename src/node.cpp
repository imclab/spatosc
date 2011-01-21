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
#include "scene.h"
#include "connection.h"

namespace spatosc
{

Node::Node(const std::string &nodeID, Scene &scene) :
    id_(nodeID),
    scene_(scene),
    pos_(),
    rot_(),
    active_(true),
    connectTO_(),
    connectFROM_(),
    updateFlag_(true)
{
    connectTO_.clear();
    connectFROM_.clear();
}

void Node::debugPrint() const
{
    std::cout << "  " << id_ << ":" << std::endl;
    std::cout << "    pos:\t" << pos_.x << "," << pos_.y << "," << pos_.z << std::endl;
    std::cout << "    rot:\t" << rot_.x << "," << rot_.y << "," << rot_.z << std::endl;
    std::cout << "    active?\t" << active_ << std::endl;
}

void Node::setPosition(double x, double y, double z)
{
    if (x != pos_.x or y != pos_.y or z != pos_.z)
    {
        pos_ = Vector3(x,y,z);
        updateFlag_ = true;
        scene_.update(this);
    }
}

void Node::setOrientation(double pitch, double roll, double yaw)
{
    if (pitch != rot_.x or roll != rot_.y or yaw != rot_.z)
    {
        rot_ = Vector3(pitch, roll, yaw);
        updateFlag_ = true;
        scene_.update(this);
    }
}

} // end namespace spatosc

