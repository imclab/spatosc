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

#include <cassert>
#include <cstdlib> // atoi
#include <iostream>
#include <sstream>
#include <string>

#include "connection.h"
#include "fudi_sender.h"
#include "fudi_translator.h"
#include "listener.h"
#include "maths.h"
#include "node.h"
#include "soundsource.h"

namespace spatosc
{

const char *FudiTranslator::DEFAULT_SEND_PORT = "31337";

FudiTranslator::FudiTranslator(const std::string &ip,
    const std::string &port,
    bool verbose = true) :
    Translator(verbose),
    port_((unsigned int) atoi(port.c_str())),
    ip_(ip)
{
    if (verbose_)
        std::cout << "FudiTranslator sending to: " << port_ << std::endl;
}

void FudiTranslator::sendFudi(const std::string &message)
{
    FudiSender sender(ip_, port_, false);
    sender.sendFudi(message);
}

FudiMessage::FudiMessage() :
    containsSomething_(false),
    message_()
{
}

std::string FudiMessage::toString() const
{
    std::ostringstream os;
    os << message_ << ";\n";
    return os.str();
}

void FudiMessage::clear()
{
    message_ = "";
    containsSomething_ = false;
}

void FudiTranslator::sendPosition(const std::string &prefix, Node *node)
{
    Vector3 vect(node->getPosition());
    if (isVerbose())
    {
        std::cout << "    node position: " << vect.x << " " << vect.y << " " << vect.z << std::endl;
        node->debugPrint();
    }
    FudiMessage mess;
    mess.add(prefix).add(node->getID());
    mess.add("xyz").add(vect.x).add(vect.y).add(vect.z);
    if (isVerbose())
        std::cout << "    sending FUDI: " << mess.toString();
    sendFudi(mess.toString());
}

void FudiTranslator::pushOSCMessages(Connection *conn)
{
    SoundSource *src = conn->getSource();
    Listener *snk = conn->getSink();

    if (isVerbose())
    {
        std::cout << "Computation update for " << conn->getSource()->getID() << " -> " << conn->getSink()->getID() << " :" <<std::endl;
        std::cout << "  dist:\t" << conn->distance() << std::endl;
        std::cout << "  azim:\t" << conn->azimuth() << std::endl;
        std::cout << "  elev:\t" << conn->elevation() << std::endl;
        std::cout << "  gain:\t" << conn->gainDB() << " dB" << std::endl;
    }
    
    if (src->shouldSendNewState())
        sendPosition("source", src);
    if (snk->shouldSendNewState())
        sendPosition("listener", snk);
}

} // end namespace spatosc
