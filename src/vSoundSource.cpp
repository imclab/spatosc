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

#include "vSoundSource.h"


// *****************************************************************************
vSoundSource::vSoundSource(const std::string &nodeID) : vBaseNode(nodeID), channelID_(-1)
{
}

vSoundSource::~vSoundSource()
{
    // destructor
}

void vSoundSource::debugPrint()
{
    vBaseNode::debugPrint();

    std::cout << "    channelID\t" << channelID_ << std::endl;
}


void vSoundSource::setChannelID(int channel)
{
    channelID_ = channel;

}
