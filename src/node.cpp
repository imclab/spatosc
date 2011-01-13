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
#include "vSoundConn.h"

namespace spatosc
{

// *****************************************************************************
Node::Node(const std::string &nodeID)
{
    this->id_ = nodeID;
    this->active_ = true;
    this->updateFlag_ = true;
    
    connectTO_.clear();
    connectFROM_.clear();
}

Node::~Node()
{
    // destructor
}

void Node::debugPrint()
{
    std::cout << "  " << id_ << ":" << std::endl;
    std::cout << "    pos:\t" << pos_.x << "," << pos_.y << "," << pos_.z << std::endl;
    std::cout << "    rot:\t" << rot_.x << "," << rot_.y << "," << rot_.z << std::endl;
    std::cout << "    active?\t" << active_ << std::endl;
}

void Node::setPosition(double x, double y, double z)
{
    pos_ = Vector3(x,y,z);
    updateFlag_ = true;
    vAudioManager::Instance().update(this);
}

void Node::setRotation(double pitch, double roll, double yaw)
{
    rot_ = Vector3(pitch, roll, yaw);
    updateFlag_ = true;
    vAudioManager::Instance().update(this);
}

};

