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

SpatdifReceiver::SpatdifReceiver(const std::string &port, SpatdifHandler * handler, bool verbose) :
    receiver_(new OscReceiver(port.c_str())),
    verbose_(verbose)
{
    registerCallbacks(handler);
}

void SpatdifReceiver::registerCallbacks(SpatdifHandler *handler)
{
    receiver_->addHandler(NULL, NULL, SpatdifHandler::onOSCMessage, handler);
}

void SpatdifReceiver::poll()
{
    int bytes = 0;
    do 
    {
        bytes = receiver_->receive();
    }
    while (bytes > 0);
            
    if (bytes > 0 && verbose_)
        std::cout << "received " << bytes << " bytes" << std::endl;
}

namespace {
std::string getID(const std::string &path)
{
    static const int NAMESPACE_LENGTH = strlen("/spatosc/core/");
    size_t idx = NAMESPACE_LENGTH;
    std::string result(path.substr(idx, std::string::npos));
    result = result.substr(0, result.find_first_of("/"));
    return result;
}

std::string getMethodName(const std::string &path)
{
    size_t idx = path.find_last_of("/") + 1;
    std::string result(path.substr(idx, std::string::npos));
    return result;
}
} // end anonymous namespace

int SpatdifHandler::onOSCMessage(const char * path, const char * /*types*/,
        lo_arg ** argv, int argc, void * /*data*/, void *user_data)
{
    SpatdifHandler *handler = static_cast<SpatdifHandler*>(user_data);

    // FIXME: Thu Jan 27 14:36:07 EST 2011 : tmatth: use regexes/improve path,
    // reduce string comparisons
    std::string id(getID(path));
    std::string method(getMethodName(path));
    //std::cout << "id=" << id << ", method " << method << std::endl;

    // FIXME: Wed Jan 26 15:25:04 EST 2011: tmatth: refactor!!!
    // only core methods for now
    if (method == "xyz")
    {
        assert(argc == 3);
        handler->xyz(id, argv[0]->f, argv[1]->f, argv[2]->f);
    }
    else if (method == "aed")
    {
        assert(argc == 3);
        handler->aed(id, argv[0]->f, argv[1]->f, argv[2]->f);
    }
    else if (method == "xy")
    {
        assert(argc == 2);
        handler->xy(id, argv[0]->f, argv[1]->f);
    }
    else if (method == "delay")
    {
        assert(argc == 1);
        handler->delay(id, argv[0]->f);
    }
    else if (method == "gain")
    {
        assert(argc == 1);
        handler->gain(id, argv[0]->f);
    }
    else if (method == "gainDB")
    {
        assert(argc == 1);
        handler->gainDB(id, argv[0]->f);
    }
    else if (method == "spread")
    {
        assert(argc == 1);
        handler->spread(id, argv[0]->f);
    }
    else if (method == "spreadAE")
    {
        assert(argc == 2);
        handler->spreadAE(id, argv[0]->f, argv[1]->f);
    }
    else
    {
        std::cerr << "Unknown method " << method << std::endl;
        return 1;
    }
    return 1;
}



int SpatdifReceiver::onNodeMessage(const char * path, const char * /*types*/,
        lo_arg ** argv, int argc, void * /*data*/, void *user_data)
{
    Node *node = static_cast<Node*>(user_data);

    // TODO: (mikewoz) check if the path prefix is in /spatosc/core namespace

    // TODO: (mikewoz) then check that 2nd last path component matches source->getID()

    // then get method:
    std::string pathStr = std::string(path);
    size_t idx = pathStr.find_last_of("/") + 1;
    std::string method(pathStr.substr(idx, std::string::npos));

    // then call node's handleMessage and perhaps return true if handled?
    node->handleMessage(method, argc, argv);

    /*
    if (method == "xyz")
    {
        assert (argc == 3);
        node->setPosition(argv[0]->f, argv[1]->f, argv[2]->f);
    }
    else if (method == "aed")
    {
        assert(argc == 3);
        node->setOrientation(argv[0]->f, argv[1]->f, argv[2]->f);
    }
    else
    {
        std::cerr << "Unknown method " << method << std::endl;
        return 1;
    }
     */

    return 1;
}

// FIXME: these shouldn't have empty implementations
void SpatdifHandler::aed(const std::string &/*id*/, float /*azimuth*/, float /*elevation*/, float /*distanceMeters*/)
{}
void SpatdifHandler::xy(const std::string &/*id*/, float /*x*/, float /*y*/){};
void SpatdifHandler::delay(const std::string &/*id*/, float /*delay*/){};
void SpatdifHandler::gainDB(const std::string &/*id*/, float /*gainDB*/){};
void SpatdifHandler::gain(const std::string &/*id*/, float /*gain*/){};
void SpatdifHandler::spread(const std::string &/*id*/, float /*spread*/){};
void SpatdifHandler::spreadAE(const std::string &/*id*/, float /*azimSpread*/, float /*elevSpread*/){};

} // end namespace spatosc

