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

#include "dmitritranslator.h"
#include "oscutils.h"
#include "soundsource.h"
#include "connection.h"

#include <iostream>

namespace spatosc
{

const double DmitriTranslator::SPACEMAP_RADIUS = 750.0;

// *****************************************************************************
DmitriTranslator::DmitriTranslator(const std::string &ip) : Translator()
{
    destAddr_ = lo_address_new(ip.c_str(), "18033");
    lo_serv_ = lo_server_new("18099", NULL);
    // TODO: #ifdef ENABLE_DEBUG
    std::cout << "Sending to D-Mitri on: " << lo_address_get_url(destAddr_) << std::endl;
    std::cout << "Outgoing address is:   " << lo_server_get_url(lo_serv_) << std::endl;
}

DmitriTranslator::~DmitriTranslator()
{
    // destructor
    lo_server_free(lo_serv_);
    lo_address_free(destAddr_);
}

void DmitriTranslator::update(Connection *conn)
{
    std::string str;
    SoundSource *src = dynamic_cast<SoundSource*>(conn->src_);

    if (!src) return;
    if (src->getChannelID() < 0) return;

    float r = conn->elevation() / (M_PI/2);

    float spacemapX = cos(conn->azimuth()) * r * SPACEMAP_RADIUS;
    float spacemapY = sin(conn->azimuth()) * r * SPACEMAP_RADIUS;

    str = "/spacemap/" + OSCutil::stringify(src->getChannelID()) + "/x";
    lo_send_from(destAddr_, lo_serv_, LO_TT_IMMEDIATE, str.c_str(), "f", spacemapX);

    str = "/spacemap/" + OSCutil::stringify(src->getChannelID()) + "/y";
    lo_send_from(destAddr_, lo_serv_, LO_TT_IMMEDIATE, str.c_str(), "f", spacemapY);

    str = "Input " + OSCutil::stringify(src->getChannelID()) + " Level";
    lo_send_from(destAddr_, lo_serv_, LO_TT_IMMEDIATE, "/set", "sf", str.c_str(), conn->gain());
}

std::string DmitriTranslator::getTypeString() const
{
    return "DMITIRI";
}

} // end namespace spatosc

