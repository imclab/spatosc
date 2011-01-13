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
 * The vSoundSource class.
 */
#ifndef __vSoundSource_H
#define __vSoundSource_H

#include "vBaseNode.h"

namespace spatosc
{

// FIXME: tmatth Wed Jan 12 16:05:10 EST 2011: why is this virtual?
/**
 * Sound source node.
 */
class vSoundSource : /*virtual*/ public vBaseNode
{

    public:

        vSoundSource(const std::string &nodeID);
        ~vSoundSource();

        void debugPrint();

        void setChannelID(int channel);
        int getChannelID() const { return channelID_; }

    protected:

        int channelID_;

};

};

#endif
