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
 * The Listener class.
 */
#ifndef __LISTENER_H__
#define __LISTENER_H__

#include "node.h"

namespace spatosc
{
/**
 * Node that can listen to sound sources.
 *
 * The listener represents a person that listens to an audio scene. There is usually only one listener in a scene.
 */
class Listener : public Node
{
    public:
        Listener(const std::string &nodeID, Scene &scene);
        virtual void debugPrint() const;
    private:
        virtual bool handleMessage_(const std::string &method, int argc, lo_arg ** argv);
};

} // end namespace spatosc

#endif // __LISTENER_H__
