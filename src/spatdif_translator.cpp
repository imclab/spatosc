/*
 * spatdif_translator.cpp
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

#include "spatdif_translator.h"
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

const char *SpatdifTranslator::DEFAULT_SEND_PORT = "18032";
const char *SpatdifTranslator::DEFAULT_RECEIVER_PORT = "18098";

SpatdifTranslator::SpatdifTranslator(const std::string &ip,
        const std::string &port,
        bool verbose = true) :
    Translator(verbose),
    sender_(new OscSender(ip, port))
    {
		if (verbose_)
            std::cout << "SpatdifTranslator sending to: " << sender_->toString() << std::endl;
    }

void SpatdifTranslator::sendPosition(const std::string &prefix, Node *node)
{
    std::string path = prefix +  "/xyz";
    Vector3 vect(node->getPosition());
    sender_->sendMessage(path, "fff", vect.x, vect.y, vect.z, SPATOSC_ARGS_END);
}

void SpatdifTranslator::sendAED(const std::string &prefix, Connection *conn)
{
    std::string path = prefix +  "/aed";
    sender_->sendMessage(path, "fff", conn->azimuth(), conn->elevation(), conn->distance(), SPATOSC_ARGS_END);
}

void SpatdifTranslator::sendDelay(const std::string &prefix, Connection *conn)
{
    std::string path = prefix +  "/delay";
    sender_->sendMessage(path, "f", conn->delay(), SPATOSC_ARGS_END);
}

void SpatdifTranslator::sendGainDB(const std::string &prefix, Connection *conn)
{
    std::string path = prefix +  "/gainDB";
    sender_->sendMessage(path, "f", conn->gainDB(), SPATOSC_ARGS_END);
}

void SpatdifTranslator::pushOSCMessages(Connection * conn)
{
    SoundSource *src = conn->getSource();
    Listener *snk = conn->getSink();
    assert(src);
    assert(snk);

    // FIXME:Wed Jan 19 16:22:42 EST 2011:tmatth
    // do we want node-type/node-id or just node-id?
    //
    bool newPositions = false;
    if (snk->sendNewPosition())
    {
        sendPosition("/spatosc/core/listener/" + snk->getID(), snk);
        snk->positionSent();
        newPositions = true;
    }

    std::string srcPath = "/spatosc/core/source/" + src->getID();

    if (src->sendNewPosition())
    {
        sendPosition(srcPath, src);
        src->positionSent();
        newPositions = true;
    }

    // FIXME: Thu Jan 27 15:35:29 EST 2011:tmatth
    // maybe this should be in connection? Probably not though.
    if (newPositions)
    {
        sendAED(srcPath, conn);
        sendDelay(srcPath, conn);
        sendGainDB(srcPath, conn);
    }
}

} // end namespace spatosc
