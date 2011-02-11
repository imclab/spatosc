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

#include <iostream>
#include "vectors.h"
#include "listener.h"
#include "soundsource.h"
#include "scene.h"
#include "connection.h"

namespace spatosc
{

Listener::Listener(const std::string &nodeID, Scene &scene) :
    Node(nodeID, scene),
    connectFROM_()
{
    // must be called in this derived class' constructor as it calls a pure virtual function.
    // we call this in case a geotransform has already been applied to the scene
    setPosition(0.0, 0.0, 0.0);
    setOrientation(0.0, 0.0, 0.0);
}

void Listener::debugPrint() const
{
    Node::debugPrint();

    Scene::ConnConstIterator c;
    std::cout << "    listen to:\t";
    for (c = connectFROM_.begin(); c != connectFROM_.end(); ++c)
    {
        std::cout << (*c)->getSource() << " ";
    }
    if (connectFROM_.empty())
        std::cout << "<NO CONNECTIONS>";
    std::cout << std::endl;
}

bool Listener::handleMessage_(const std::string &/*method*/, int /*argc*/, lo_arg ** /*argv*/)
{
    return false;
}

void Listener::addConnectionFrom(const std::tr1::shared_ptr<Connection> &conn)
{
    connectFROM_.push_back(conn);
}

void Listener::removeConnectionFrom(const Connection *conn)
{
    eraseFromVector(connectFROM_, conn);
}

void Listener::onNodeChanged()
{
    typedef std::vector<std::tr1::shared_ptr<Connection> >::iterator ConnIterator;
    ConnIterator c;

    for (c = connectFROM_.begin(); c != connectFROM_.end(); ++c)
        scene_.onConnectionChanged(c->get());
}
} // end namespace spatosc
