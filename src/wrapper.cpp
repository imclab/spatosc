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

#include "connection.h"
#include "dmitri_translator.h"
#include "listener.h"
#include "memory.h"
#include "node.h"
#include "scene.h"
#include "soundsource.h"
#include "basic_translator.h"
#include "fudi_translator.h"
#include "translator.h"
#include "wrapper.h"
#include <iostream>

namespace spatosc
{

Wrapper::Wrapper() :
    scene_(new Scene())
{
}

void Wrapper::debugPrint()
{
    scene_->debugPrint();
}

void Wrapper::setSynchronous(bool synchronous)
{
    scene_->setSynchronous(synchronous);
}

bool Wrapper::flushMessages()
{
    return scene_->flushMessages();
}

bool Wrapper::createListener(const std::string &nodeName)
{
    Listener *node = scene_->createListener(nodeName);
    if (node == 0)
    {
        std::cerr << __FUNCTION__ << ": Error creating listener " << nodeName << std::endl;
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
        std::cerr << __FUNCTION__ << ": Error creating sound source " << nodeName << std::endl;
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
        std::cerr << __FUNCTION__ << ": No such node: " << nodeName << std::endl;
        return false;
    }
}

bool Wrapper::setConnectFilter(const std::string &filterRegex)
{
    return scene_->setConnectFilter(filterRegex);
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
        std::cerr << __FUNCTION__ << ": No such node: " << nodeFrom << std::endl;
        return false;
    }
    Listener *sink = scene_->getListener(nodeTo);
    if (! sink)
    {
        std::cerr << __FUNCTION__ << ": No such node: " << nodeTo << std::endl;
        return false;
    }
    return scene_->disconnect(source,  sink);
}

bool Wrapper::connect(const std::string &nodeFrom, const std::string &nodeTo)
{
    SoundSource *source = scene_->getSoundSource(nodeFrom);
    if (! source)
    {
        std::cerr << __FUNCTION__ << ": No such node: " << nodeFrom << std::endl;
        return false;
    }
    Listener *sink = scene_->getListener(nodeTo);
    if (! sink)
    {
        std::cerr << __FUNCTION__ << ": No such node: " << nodeTo << std::endl;
        return false;
    }
    return scene_->connect(source, sink);
}

bool Wrapper::setOrientation(const std::string &nodeName, double pitch, double roll, double yaw)
{
    Node *node = scene_->getNode(nodeName);
    if (! node)
    {
        std::cerr << __FUNCTION__ << ": No such node: " << nodeName << std::endl;
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
        std::cerr << __FUNCTION__ << ": No such node: " << nodeName << std::endl;
        return false;
    }
    else
    {
        node->setPosition(x, y, z);
        return true;
    }
}

void Wrapper::setTranslation(double tx, double ty, double tz)
{
    scene_->setTranslation(tx, ty, tz);
}
        
void Wrapper::setOrientation(double pitch, double roll, double yaw)
{
    scene_->setOrientation(pitch, roll, yaw);
}

void Wrapper::setOrientation(double x, double y, double z, double w)
{
    scene_->setOrientation(x, y, z, w);
}

void Wrapper::setScale(double sx, double sy, double sz)
{
    scene_->setScale(sx, sy, sz);
}

bool Wrapper::addTranslator(const std::string &name, const std::string &translatorName, const std::string &sendToAddress, const std::string &port, bool verbose)
{
    if (translatorName == "BasicTranslator")
        return scene_->addTranslator<BasicTranslator>(name, sendToAddress, port, verbose) != 0;
    else if (translatorName == "DmitriTranslator")
        return scene_->addTranslator<DmitriTranslator>(name, sendToAddress, port, verbose) != 0; // FIXME:4th arg to DmitriTranslator's constructor is not a bool!
    else if (translatorName == "ConsoleTranslator")
        return scene_->addTranslator<ConsoleTranslator>(name, sendToAddress, port, verbose) != 0;
    else if (translatorName == "FudiTranslator")
        return scene_->addTranslator<FudiTranslator>(name, sendToAddress, port, verbose) != 0;
    else
    {
        std::cerr << "No such translator: " << translatorName << std::endl;
        return false;
    }
}

bool Wrapper::addDmitriTranslator(const std::string &name, const std::string &ip, const std::string &toPort, bool verbose)
{
    return scene_->addTranslator(name, new DmitriTranslator(ip, toPort, verbose));
}

bool Wrapper::addDmitriTranslator(const std::string &name, const std::string &ip, const std::string &toPort, const std::string &fromPort, bool verbose)
{
    return scene_->addTranslator(name, new DmitriTranslator(ip, toPort, fromPort, verbose));
}

bool Wrapper::removeTranslator(const std::string &name)
{
    return scene_->removeTranslator(name);
}

bool Wrapper::hasTranslator(const std::string &name)
{
    return scene_->hasTranslator(name);
}

bool Wrapper::setNodeStringProperty(const std::string &node, const std::string &key, const std::string &value)
{
    Node *nodePtr = scene_->getNode(node);
    if (nodePtr)
    {
        nodePtr->setStringProperty(key, value);
        return true;
    }
    else
    {
        std::cerr << __FUNCTION__ << ": No such node: \"" << node << "\"" << std::endl;
        return false;
    }
}

bool Wrapper::getNodeStringProperty(const std::string &node, const std::string &key, std::string &value)
{
    Node *nodePtr = scene_->getNode(node);
    if (nodePtr)
    {
        return nodePtr->getStringProperty(key, value);
    }
    else
    {
        std::cerr << __FUNCTION__ << ": No such node: \"" << node << "\"" << std::endl;
        return false;
    }
}

bool Wrapper::removeNodeStringProperty(const std::string &node, const std::string &key)
{
    Node *nodePtr = scene_->getNode(node);
    if (nodePtr)
    {
        return nodePtr->removeStringProperty(key);
    }
    else
    {
        std::cerr << __FUNCTION__ << ": No such node: \"" << node << "\"" << std::endl;
        return false;
    }
}

bool Wrapper::setNodeIntProperty(const std::string &node, const std::string &key, const int &value)
{
    Node *nodePtr = scene_->getNode(node);

    if (nodePtr)
    {
        nodePtr->setIntProperty(key, value);
        return true;
    }
    else
    {
        std::cerr << __FUNCTION__ << ": No such node: \"" << node << "\"" << std::endl;
        return false;
    }
}

bool Wrapper::getNodeIntProperty(const std::string &node, const std::string &key, int &value)
{
    Node *nodePtr = scene_->getNode(node);
    if (nodePtr)
    {
        return nodePtr->getIntProperty(key, value);
    }
    else
    {
        std::cerr << __FUNCTION__ << ": No such node: \"" << node << "\"" << std::endl;
        return false;
    }
}

bool Wrapper::removeNodeIntProperty(const std::string &node, const std::string &key)
{
    Node *nodePtr = scene_->getNode(node);
    if (nodePtr)
    {
        return nodePtr->removeIntProperty(key);
    }
    else
    {
        std::cerr << __FUNCTION__ << ": No such node: \"" << node << "\"" << std::endl;
        return false;
    }
}

bool Wrapper::setNodeFloatProperty(const std::string &node, const std::string &key, const double &value)
{
    Node *nodePtr = scene_->getNode(node);
    if (nodePtr)
    {
        nodePtr->setFloatProperty(key, value);
        return true;
    }
    else
    {
        std::cerr << __FUNCTION__ << ": No such node: \"" << node << "\"" << std::endl;
        return false;
    }
}

bool Wrapper::getNodeFloatProperty(const std::string &node, const std::string &key, double &value)
{
    Node *nodePtr = scene_->getNode(node);
    if (nodePtr)
    {
        return nodePtr->getFloatProperty(key, value);
    }
    else
    {
        std::cerr << __FUNCTION__ << ": No such node: \"" << node << "\"" << std::endl;
        return false;
    }
}

bool Wrapper::removeNodeFloatProperty(const std::string &node, const std::string &key)
{
    Node *nodePtr = scene_->getNode(node);
    if (nodePtr)
    {
        return nodePtr->removeFloatProperty(key);
    }
    else
    {
        std::cerr << __FUNCTION__ << ": No such node: \"" << node << "\"" << std::endl;
        return false;
    }
}

bool Wrapper::setDistanceFactor(const std::string &sourceNode, const std::string &sinkNode, double factor)
{
    Node *src = scene_->getNode(sourceNode);
    if (! src)
    {
        std::cerr << __FUNCTION__ << ": No such node: \"" << sourceNode << "\"" << std::endl;
        return false;
    }
    Node *sink = scene_->getNode(sinkNode);
    if (! sink)
    {
        std::cerr << __FUNCTION__ << ": No such node: \"" << sinkNode << "\"" << std::endl;
        return false;
    }
    Connection *conn = scene_->getConnection(src, sink);
    if (conn)
    {
        conn->setDistanceFactor(factor);
        return true;
    }
    else
    {
        std::cerr << __FUNCTION__ << ": No connection between \"" << sourceNode << "\" and \"" << sinkNode << "\"" << std::endl;
        return false;
    }
}

bool Wrapper::setDopplerFactor(const std::string &sourceNode, const std::string &sinkNode, double factor)
{
    Node *src = scene_->getNode(sourceNode);
    if (! src)
    {
        std::cerr << __FUNCTION__ << ": No such node: \"" << sourceNode << "\"" << std::endl;
        return false;
    }
    Node *sink = scene_->getNode(sinkNode);
    if (! sink)
    {
        std::cerr << __FUNCTION__ << ": No such node: \"" << sinkNode << "\"" << std::endl;
        return false;
    }
    Connection *conn = scene_->getConnection(src, sink);
    if (conn)
    {
        conn->setDopplerFactor(factor);
        return true;
    }
    else
    {
        std::cerr << __FUNCTION__ << ": No connection between \"" << sourceNode << "\" and \"" << sinkNode << "\"" << std::endl;
        return false;
    }
}

bool Wrapper::setNodeActive(const std::string &node, bool active)
{
    Node *n = scene_->getNode(node);
    if (! n)
    {
        std::cerr << __FUNCTION__ << ": No such node: \"" << node << "\"" << std::endl;
        return false;
    }
    n->setActive(active);
    return true;
}


} // end of namespace spatosc

