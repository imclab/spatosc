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
 * The SoundSource class.
 */
#ifndef __SoundSource_H
#define __SoundSource_H

#include "node.h"

namespace spatosc
{

// FIXME: tmatth Wed Jan 12 16:05:10 EST 2011: why is this virtual?
/**
 * Sound source node.
 */
class SoundSource : /*virtual*/ public Node
{

    public:

        SoundSource(const std::string &nodeID);
        ~SoundSource();

        void debugPrint();

        void setChannelID(int channel);
        int getChannelID() const { return channelID_; }

    protected:

        int channelID_;

};

} // end namespace spatosc

#endif
