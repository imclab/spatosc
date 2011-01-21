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

/**
 * Tests that there can only be unique node names.
 */

#include <iostream>
#include "spatosc.h"

using namespace spatosc;

bool test_connect()
{
    Scene scene;
    scene.setAutoConnect(false);
    Node *source = dynamic_cast<Node*>(scene.createSoundSource("source"));
    Node *listener = dynamic_cast<Node*>(scene.createListener("listener"));
    Connection *connection = scene.connect(source, listener);
    if (connection == 0)
    {
        std::cout << "Could not connect a source to a listener" << std::endl;
        return false;
    }
    Connection *connection2 = scene.getConnection(source, listener);
    if (connection != connection2)
    {
        std::cout << "Connection pointer got with getConnection dont match." << std::endl;
        return false;
    }
    connection = scene.connect(source, listener);
    if (connection != 0)
    {
        std::cout << "We should not be able to connect them twice." << std::endl;
        return false;
    }
    return true;
}

bool test_disconnect()
{
    Scene scene;
    scene.setAutoConnect(false);
    Node *source = scene.createSoundSource("source");
    Node *listener = scene.createListener("listener");
    Connection *connection = scene.connect(source, listener);
    bool success = scene.disconnect(source, listener);
    if (! success)
    {
        std::cout << "Could not disconnect" << std::endl;
        return false;
    }
    connection = scene.getConnection(source, listener);
    if (connection != 0)
    {
        std::cout << "Connection was not deleted." << std::endl;
        return false;
    }
    success = scene.disconnect(source, listener);
    if (success)
    {
        std::cout << "Should not be able to disconnect twice" << std::endl;
        return false;
    }
    return true;
}

bool test_delete_node()
{
    Scene scene;
    SoundSource *source = scene.createSoundSource("source");
    Listener *listener = scene.createListener("listener");
    scene.deleteNode(source);
    if (scene.getConnection(source, listener))
    {
        std::cout << "Should not be able to find connection anymore." << std::endl;
        return false;
    }
    SoundSource *orig = scene.getSoundSource("source");
    if (orig != 0)
    {
        std::cout << "Should not be able to find source anymore." << std::endl;
        return false;
    }
    return true;
}

int main(int /*argc*/, char ** /*argv*/)
{
    if (! test_connect())
        return 1;
    if (! test_disconnect())
        return 1;
    if (! test_delete_node())
        return 1;
    return 0;
}

