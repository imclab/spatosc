/*
 * spatdif_receiver.cpp
 *
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

#include "spatdif_receiver.h"
#include "osc_scene.h"
#include "oscreceiver.h"
#include "node.h"
#include <cstring>
#include <lo/lo.h>
#include <string>
#include <iostream>
#include <cassert>

#define UNUSED(x) ((void) (x))

namespace spatosc
{

SpatdifReceiver::SpatdifReceiver(const std::string &port, bool verbose) :
    OscReceiver(port),
    verbose_(verbose)
{
}

int SpatdifReceiver::poll()
{
    int bytes = 0;
    do
    {
        bytes = receive();
    }
    while (bytes > 0);

    if (bytes > 0 && verbose_)
        std::cout << "received " << bytes << " bytes" << std::endl;
    return bytes;
}

// Helper methods to parse OSC paths
namespace {

bool wrongNamespace(const std::string &path)
{
    static const std::string NAMESPACE("/spatosc/core/");
    static const int NAMESPACE_LENGTH = NAMESPACE.length();
    return path.substr(0, NAMESPACE_LENGTH) != NAMESPACE;
}

std::string getID(const std::string &path)
{
    size_t idx = path.find_last_of("/");
    std::string result(path.substr(0, idx));
    return result.substr(result.find_last_of("/") + 1, std::string::npos);
}

std::string getMethodName(const std::string &path)
{
    size_t idx = path.find_last_of("/") + 1;
    std::string result(path.substr(idx, std::string::npos));
    return result;
}

std::string getTypeName(const std::string &path)
{
    size_t idx = strlen("/spatosc/core/");
    std::string result(path.substr(idx, std::string::npos));
    return result.substr(0, result.find_first_of("/"));
}

} // end anonymous namespace


int SpatdifReceiver::onSceneMessage(const char * path, const char * types,
        lo_arg ** argv, int argc, void * /*data*/, void *user_data)
{
    OscScene *scene = static_cast<OscScene*>(user_data);

    // check if the path prefix is in /spatosc/core namespace
    if (wrongNamespace(path))
        return 1;

    if (getTypeName(path) != "scene")
        return 1;

    // get method name:
    std::string method(getMethodName(path));

    // then call scene's handleMessage and perhaps return true if handled?
    scene->handleMessage(method, argc, argv, types);

    return 1;
}

int SpatdifReceiver::onNodeMessage(const char * path, const char * types,
        lo_arg ** argv, int argc, void * /*data*/, void *user_data)
{
    Node *node = static_cast<Node*>(user_data);

    // check if the path prefix is in /spatosc/core namespace
    if (wrongNamespace(path))
        return 1;

    std::string id(getID(path));

    // then check that 2nd last path component matches source->getID() or wildcard
    if (id != node->getID() && id != "*")
        return 1;

    // then get method name:
    std::string method(getMethodName(path));

    // then call node's handleMessage and perhaps return true if handled?
    node->handleMessage(method, argc, argv, types);

    return 1;
}
} // end namespace spatosc
