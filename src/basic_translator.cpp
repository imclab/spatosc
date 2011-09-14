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

#include "basic_translator.h"
#include <string>
#include <iostream>
#include <cassert>
#include "oscutils.h"
#include "oscsender.h"
#include "connection.h"
#include "soundsource.h"
#include "listener.h"

namespace spatosc
{

const char *BasicTranslator::DEFAULT_SEND_PORT = "18032";

BasicTranslator::BasicTranslator(const std::string &ip, const std::string &port) :
    Translator(),
    sender_(new OscSender(ip, port))
{
}

void BasicTranslator::debugPrint()
{
    std::cout << "  Basic Translator -- \t" << sender_->toString() << std::endl;
}

void BasicTranslator::sendPosition(const std::string &prefix, Node *node)
{

	
	// no need to send this to the renderer
    std::string path = prefix +  "/xyz";
   Vector3 vect(node->getPosition());
    // sender_->sendMessage(path, "fff", vect.x, vect.y, vect.z, SPATOSC_ARGS_END);
}

void BasicTranslator::sendAED(const std::string &prefix, Connection *conn)
{
    std::string path = prefix +  "/aed";
    sender_->sendMessage(path, "fff", conn->azimuth(), conn->elevation(), conn->distance(), SPATOSC_ARGS_END);
}

void BasicTranslator::sendDelay(const std::string &prefix, Connection *conn)
{
    std::string path = prefix +  "/delay";
    sender_->sendMessage(path, "f", conn->delay(), SPATOSC_ARGS_END);
}

void BasicTranslator::sendGainDB(const std::string &prefix, Connection *conn)
{
    std::string path = prefix +  "/gainDB";
    sender_->sendMessage(path, "f", conn->gainDB(), SPATOSC_ARGS_END);
}

void BasicTranslator::pushConnectionChanges(Connection * conn)
{
    SoundSource *src = conn->getSource();
    Listener *snk = conn->getSink();
    
    sendPosition("/spatosc/core/listener/" + snk->getID(), snk);

    std::string srcPath = "/spatosc/core/source/" + src->getID();
    sendPosition(srcPath, src);

    std::string connectionPath = "/spatosc/core/connection/" + conn->getID();
    sendAED(connectionPath, conn);
    sendDelay(connectionPath, conn);
    sendGainDB(connectionPath, conn);
}

void BasicTranslator::pushSceneChange(const char *types, va_list ap)
{
    // just forwards all scene changes over OSC
    // (includes messages like createListener, createSoundSource, connect, etc)
    sender_->sendMessage("/spatosc/core", types, ap);
}
/*
void BasicTranslator::pushSceneChange(const std::string &method, ...)
{
    va_list ap;
    va_start(ap, method);

    if ((method=="createSoundSource") || (method=="createListener"))
        //std::cout << "got createSoundSource message " << method << " " << va_arg(ap,const char*) << std::endl;
        sender_->sendMessage("/spatosc/core", "ss", method.c_str(), va_arg(ap,const char*), SPATOSC_ARGS_END);
    else if (method=="connect")
        sender_->sendMessage("/spatosc/core", "sss", method.c_str(), va_arg(ap,const char*), va_arg(ap,const char*), SPATOSC_ARGS_END);
    else
        std::cout << "ERROR: Got pushSceneChange for unknown method: " << method << std::endl;
}
*/

void BasicTranslator::pushPropertyChange(Node *node, const std::string &key, const std::string &value)
{
    //std::cout << "pushing property for node " << node->getID() << ": " << key << value << std::endl;
    std::string path = "/spatosc/core/" + node->getType() + "/" + node->getID() + "/prop";
	
	//std::cout << "pushPropertyChange";
    sender_->sendMessage(path, "ss", key.c_str(), value.c_str(), SPATOSC_ARGS_END);
}

void BasicTranslator::pushPropertyChange(Node *node, const std::string &key, const double &value)
{
    //std::cout << "pushing property for node " << node->getID() << ": " << key << value << std::endl;
    std::string path = "/spatosc/core/" + node->getType() + "/" + node->getID() + "/prop";
    sender_->sendMessage(path, "sf", key.c_str(), (float)value, SPATOSC_ARGS_END);
}

void BasicTranslator::pushPropertyChange(Node *node, const std::string &key, const int &value)
{
    //std::cout << "pushing property for node " << node->getID() << ": " << key << value << std::endl;
    std::string path = "/spatosc/core/" + node->getType() + "/" + node->getID() + "/prop";
    sender_->sendMessage(path, "sf", key.c_str(), (float)value, SPATOSC_ARGS_END);
}

} // end namespace spatosc

