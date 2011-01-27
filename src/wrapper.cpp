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

#include "dmitri_translator.h"
#include "listener.h"
#include "memory.h"
#include "node.h"
#include "scene.h"
#include "soundsource.h"
#include "spatdif_translator.h"
#include "translator.h"
#include "wrapper.h"
#include <iostream>

namespace spatosc
{


Wrapper::Wrapper() :
    scene_(new Scene())
{
}

bool Wrapper::createListener(const std::string &nodeName)
{
    Listener *node = scene_->createListener(nodeName);
    if (node == 0)
    {
        std::cerr << "Error creating listener " << nodeName << std::endl;
        return false;
    }
    else
        return true;
}

bool Wrapper::createSource(const std::string &nodeName)
{
    SoundSource *node = scene_->createSoundSource(nodeName);
    if (node == 0)
    {
        std::cerr << "Error creating sound source " << nodeName << std::endl;
        return false;
    }
    else
        return true;
}

bool Wrapper::deleteNode(const std::string &nodeName)
{
    Listener *listener = scene_->getListener(nodeName);
    if (listener != 0)
        return scene_->deleteNode(listener);
    SoundSource *source = scene_->getSoundSource(nodeName);
    if (source != 0)
        return scene_->deleteNode(source);
    else
    {
        std::cerr << "No such node: " << nodeName << std::endl;
        return false;
    }
}

bool Wrapper::setConnectFilter(const std::string &filterRegex)
{
    scene_->setConnectFilter(filterRegex);
    return true;
}

bool Wrapper::clearScene()
{
    scene_->deleteAllNodes();
    return true;
}

bool Wrapper::setAutoConnect(bool enabled)
{
    scene_->setAutoConnect(enabled);
    return true;
}

bool Wrapper::disconnect(const std::string &nodeFrom, const std::string &nodeTo)
{
    SoundSource *source = scene_->getSoundSource(nodeFrom);
    if (! source)
    {
        std::cerr << "No such node: " << nodeFrom << std::endl;
        return false;
    }
    Listener *sink = scene_->getListener(nodeTo);
    if (! sink)
    {
        std::cerr << "No such node: " << nodeTo << std::endl;
        return false;
    }
    return scene_->disconnect(source,  sink);
}

bool Wrapper::connect(const std::string &nodeFrom, const std::string &nodeTo)
{
    SoundSource *source = scene_->getSoundSource(nodeFrom);
    if (! source)
    {
        std::cerr << "No such node: " << nodeFrom << std::endl;
        return false;
    }
    Listener *sink = scene_->getListener(nodeTo);
    if (! sink)
    {
        std::cerr << "No such node: " << nodeTo << std::endl;
        return false;
    }
    return scene_->connect(source, sink);
}

bool Wrapper::setOrientation(const std::string &nodeName, double pitch, double roll, double yaw)
{
    Node *node = scene_->getNode(nodeName);
    if (! node)
    {
        std::cerr << "No such node: " << nodeName << std::endl;
        return false;
    }
    else
    {
        node->setOrientation(pitch, roll, yaw);
        return true;
    }
}

bool Wrapper::setPosition(const std::string &nodeName, double x, double y, double z)
{
    Node *node = scene_->getNode(nodeName);
    if (! node)
    {
        std::cerr << "No such node: " << nodeName << std::endl;
        return false;
    }
    else
    {
        node->setPosition(x, y, z);
        return true;
    }
}

bool Wrapper::setSourceChannel(const std::string &nodeName, int channel)
{
    SoundSource *node = scene_->getSoundSource(nodeName);
    if (! node)
    {
        std::cerr << "No such node: " << nodeName << std::endl;
        return false;
    }
    else
    {
        node->setChannelID(channel);
        return true;
    }
}

bool Wrapper::setTranslator(const std::string &translatorName, const std::string &sendToAddress, const std::string &port)
{
    if (translatorName == "SpatdifTranslator")
        scene_->setTranslator<SpatdifTranslator>(sendToAddress, port);
    else if (translatorName == "DmitriTranslator")
        scene_->setTranslator<DmitriTranslator>(sendToAddress, port);
    else
    {
        std::cerr << "No such translator: " << translatorName << std::endl;
        return false;
    }
    return true;
}

} // end of namespace spatosc

