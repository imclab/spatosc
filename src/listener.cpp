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

#include "listener.h"
#include "scene.h"
#include "vSoundConn.h"

namespace spatosc
{

// *****************************************************************************
vListener::vListener(const std::string &nodeID) : Node(nodeID)
{
}

vListener::~vListener()
{
    // destructor
}

void vListener::debugPrint()
{
    Node::debugPrint();

    vAudioManager::connIterator c;
    std::cout << "    listen to:\t";
    for (c = connectFROM_.begin(); c != connectFROM_.end(); ++c)
    {
        std::cout << (*c)->src_->getID() << " ";
    }
    if (connectFROM_.empty())
        std::cout << "<NO CONNECTIONS>";
    std::cout << std::endl;
}

};

