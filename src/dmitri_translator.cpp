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
#include "oscutils.h"
#include "oscsender.h"
#include "soundsource.h"
#include "connection.h"

#include <iostream>

namespace spatosc
{

const double DmitriTranslator::SPACEMAP_RADIUS = 750.0;

// ************************************************
// FIXME: Wed Jan 19 14:12:24 EST 2011: tmatth
// could these port values be defined in translator.h/cpp?
// 2011-01-21:aalex:I think this default port number is D-Mitri-specific.
DmitriTranslator::DmitriTranslator(const std::string &ip,
        const std::string &toPort,
        bool verbose = false) :
    Translator(verbose),
    sender_(new OscSender(ip, toPort))
    {
		if (verbose)
            std::cout << "spatdif translator sending to: " << sender_->toString() << std::endl;
    }

DmitriTranslator::DmitriTranslator(const std::string &ip,
        const std::string &toPort,
        const std::string &fromPort,
        bool verbose = false) :
    Translator(verbose),
    sender_(new OscSender(ip, toPort, fromPort))
    {
        if (verbose_)
        {
            //std::cout << "Sending to D-Mitri on: " << lo_address_get_url(destAddr_) << std::endl;
            //std::cout << "Outgoing address is:   " << lo_server_get_url(lo_serv_) << std::endl;
        }
    }

void DmitriTranslator::pushOSCMessages(Connection *conn)
{
    std::string str;
    SoundSource *src = dynamic_cast<SoundSource*>(conn->src_);

    if (!src)
        return;
    if (src->getChannelID() < 0)
        return;

    float r = conn->elevation() / (M_PI / 2);

    float spacemapX = cos(conn->azimuth()) * r * SPACEMAP_RADIUS;
    float spacemapY = sin(conn->azimuth()) * r * SPACEMAP_RADIUS;

    str = "/spacemap/" + OSCutil::stringify(src->getChannelID()) + "/x";
    //lo_send_from(destAddr_, lo_serv_, LO_TT_IMMEDIATE, str.c_str(), "f", spacemapX);
    sender_->sendMessage(str.c_str(), "f", spacemapX, SPATOSC_ARGS_END);

    str = "/spacemap/" + OSCutil::stringify(src->getChannelID()) + "/y";
    //lo_send_from(destAddr_, lo_serv_, LO_TT_IMMEDIATE, str.c_str(), "f", spacemapY);
    sender_->sendMessage(str.c_str(), "f", spacemapY, SPATOSC_ARGS_END);

    str = "Input " + OSCutil::stringify(src->getChannelID()) + " Level";
    //lo_send_from(destAddr_, lo_serv_, LO_TT_IMMEDIATE, "/set", "sf", str.c_str(), conn->gain());
    sender_->sendMessage("/set", "sf", str.c_str(), conn->gain(), SPATOSC_ARGS_END);
}

} // end namespace spatosc

