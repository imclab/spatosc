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
#include <cassert>
#include "oscutils.h"
#include "oscsender.h"
#include "connection.h"
#include "soundsource.h"
#include "listener.h"

namespace spatosc
{
SpatdifTranslator::SpatdifTranslator(const std::string &ip,
        const std::string &port,
        bool verbose = false) :
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
    // FIXME: Downcasts are evil
    SoundSource *src = dynamic_cast<SoundSource*>(conn->src_);
    Listener *snk = dynamic_cast<Listener*>(conn->snk_);
    // TODO:Wed Jan 19 14:47:57 EST 2011:tmatth: set positions, gains as needed!
    assert(src);
    assert(snk);

    if (src->getChannelID() < 0)
        return;


    // FIXME:Wed Jan 19 16:22:42 EST 2011:tmatth should listener have channel ID? SpatDIF thinks so.
    // update sink position
    sendPosition("/SpatDIF/core/listener/", snk);


    std::string srcPath = "/SpatDIF/core/source/" + OSCutil::stringify(src->getChannelID());

    sendPosition(srcPath, src);
    sendAED(srcPath, conn);
    sendDelay(srcPath, conn);
    sendGainDB(srcPath, conn);
}

} // end namespace spatosc
