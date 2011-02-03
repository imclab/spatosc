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

#include "scene.h"
#include "soundsource.h"
#include "vectors.h"

namespace spatosc
{

// *****************************************************************************
SoundSource::SoundSource(const std::string &nodeID, Scene &scene) :
    Node(nodeID, scene)
{
}

bool SoundSource::handleMessage_(const std::string & /*method*/, int /*argc*/, lo_arg ** /*argv*/)
{
    return false;
}

void SoundSource::addConnectionTo(const std::tr1::shared_ptr<Connection> &conn)
{
    connectTO_.push_back(conn);
}

void SoundSource::removeConnectionTo(const Connection *conn)
{
    eraseFromVector(connectTO_, conn);
}

void SoundSource::onNodeChanged()
{
    typedef std::vector<std::tr1::shared_ptr<Connection> >::iterator ConnIterator;
    ConnIterator c;

    for (c = connectTO_.begin(); c != connectTO_.end(); ++c)
        scene_.onConnectionChanged(c->get());
}
} // end namespace spatosc
