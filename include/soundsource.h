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
#ifndef __SOUNDSOURCE_H__
#define __SOUNDSOURCE_H__

#include "node.h"

namespace spatosc
{

// FIXME: tmatth Wed Jan 12 16:05:10 EST 2011: why is this virtual?
/**
 * Sound source node.
 */
class SoundSource : public Node
{
    public:
        SoundSource(const std::string &nodeID, Scene &scene);

        virtual void debugPrint() const;

        /**
         * Sets the input channel number - for when using live sound sources.
         * @param channel The channel number.
         */
        void setChannelID(int channel);

        /**
         * Returns the input channel number - for when using live sound sources.
         * @return An audio channel number which can be used to represent the actual audio channel of an audio interface.
         */
        int getChannelID() const { return channelID_; }

    protected:

        int channelID_;
};

} // end namespace spatosc

#endif // __SOUNDSOURCE_H__
