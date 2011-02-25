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
#include <cassert>
#include "osc_scene.h"
#include "spatdif_receiver.h"
#include "unused.h"
#include "oscutils.h"

namespace spatosc
{

// for now, create a basic (CONSOLE) translator:
OscScene::OscScene(const std::string &receiverPort) :
    receiver_()
{
    receiver_.reset(new SpatdifReceiver(receiverPort));
    std::cout << "Scene receiving on port " << receiverPort << std::endl;
    receiver_->addHandler(NULL, NULL, SpatdifReceiver::onSceneMessage, this);
}

OscScene::~OscScene()
{
    receiver_->removeGenericHandler(this);
}

// FIXME:Sun Feb 13 12:11:29 EST 2011:tmatth:put this in a separate component
namespace {
bool correctNumberOfArguments(const std::string &method, int expected, int actual)
{
    if (actual != expected)
    {
        std::cerr << method << " expects " << expected << " arguments , got " <<
            actual << std::endl;
        return false;
    }
    else
        return true;
}
} // end anonymous namespace

void OscScene::handleMessage(const std::string &method, int argc, lo_arg **argv, const char *types)
{
    UNUSED(types);
    if (method == "create_source")
    {
        if (OSCutil::argMatchesType(argc, types, 0, 's'))
            createSoundSource(reinterpret_cast<const char*>(argv[0]));
    }
    else if (method == "create_listener")
    {
        if (OSCutil::argMatchesType(argc, types, 0, 's'))
            createListener(reinterpret_cast<const char *>(argv[0]));
    }
    else
        std::cerr << "Unknown method " << method << std::endl;
}

SoundSource* OscScene::createSoundSource(const std::string &id)
{
    SoundSource *result = Scene::createSoundSource(id);
    if (result != 0)
        receiver_->addHandler(NULL, NULL, SpatdifReceiver::onNodeMessage, result);
    return result;
}

Listener* OscScene::createListener(const std::string &id)
{
    Listener *result = Scene::createListener(id);
    if (result != 0)
        receiver_->addHandler(NULL, NULL, SpatdifReceiver::onNodeMessage, result);
    return result;
}

bool OscScene::poll()
{
    if (receiver_->poll() > 0)
        return true;
    return false;
}


void OscScene::unsubscribe(Node *node)
{
    receiver_->removeGenericHandler(node);
}
} // end namespace spatosc

