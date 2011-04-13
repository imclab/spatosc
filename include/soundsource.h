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
#include "memory.h"

namespace spatosc
{

class Connection;

/**
 * Sound source node.
 */
class SoundSource : public Node
{
    public:
        SoundSource(const std::string &nodeID, Scene &scene);
        virtual std::string getType() const { return "source"; }
        void addConnectionTo(const std::tr1::shared_ptr<Connection> &conn);
        void removeConnectionTo(const Connection *conn);
    private:
        std::vector<std::tr1::shared_ptr<Connection> > connectTO_;
        virtual void onNodeChanged(bool forcedNotify);
        virtual bool handleMessage_(const std::string &/*method*/, int argc, lo_arg ** /*argv*/, const char *types);
};

} // end namespace spatosc

#endif // __SOUNDSOURCE_H__
