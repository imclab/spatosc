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
#include <string>

namespace spatosc
{

const char *DmitriTranslator::DEFAULT_SEND_PORT = "18033";
const char *DmitriTranslator::DEFAULT_RECEIVER_PORT = "18099";
double DmitriTranslator::SPACEMAP_EQUATOR_RADIUS = 700.0;
const double DmitriTranslator::SPACEMAP_POLE_RADIUS = 1000.0;

// ************************************************

DmitriTranslator::DmitriTranslator(const std::string &ip, const std::string &toPort, const std::string &fromPort, bool verbose) :
Translator(verbose),
sender_(new OscSender(ip, toPort, fromPort)),
verbose_(verbose)
{
    if (verbose_)
    {
        std::cout << "D-Mitri translator sending to: " << sender_->toString() << std::endl;
    }
}

DmitriTranslator::DmitriTranslator(const std::string &ip, const std::string &toPort, bool verbose) :
Translator(verbose),
sender_(new OscSender(ip, toPort, DmitriTranslator::DEFAULT_RECEIVER_PORT)),
verbose_(verbose)
{
    if (verbose_)
    {
        std::cout << "D-Mitri translator sending to: " << sender_->toString() << std::endl;
    }
}

void DmitriTranslator::pushConnectionChanges(Connection *conn)
{
    std::string str;
    SoundSource *src = conn->getSource();

    float r = 1.0 - fabs( conn->elevation() / (M_PI/2) );
    float spacemapX, spacemapY;
    if (conn->elevation() >= 0)
    {
        spacemapX = -sin(conn->azimuth()) * r * SPACEMAP_EQUATOR_RADIUS;
        spacemapY =  cos(conn->azimuth()) * r * SPACEMAP_EQUATOR_RADIUS;
    } else {
        spacemapX = -sin(conn->azimuth()) * (SPACEMAP_EQUATOR_RADIUS + ((1-r) * (SPACEMAP_POLE_RADIUS-SPACEMAP_EQUATOR_RADIUS)));
        spacemapY =  cos(conn->azimuth()) * (SPACEMAP_EQUATOR_RADIUS + ((1-r) * (SPACEMAP_POLE_RADIUS-SPACEMAP_EQUATOR_RADIUS)));
    }

    //std::cout << "azim="<<conn->azimuth()<<" elev="<<conn->elevation()<<" r="<<r<<std::endl;
    //std::cout << "Sending spacemap: " << spacemapX << "," << spacemapY << std::endl;

    str = "/spacemap/" +  src->getID() + "/x";
    //lo_send_from(destAddr_, lo_serv_, LO_TT_IMMEDIATE, str.c_str(), "f", spacemapX);
    sender_->sendMessage(str.c_str(), "f", spacemapX, SPATOSC_ARGS_END);

    str = "/spacemap/" + src->getID() + "/y";
    //lo_send_from(destAddr_, lo_serv_, LO_TT_IMMEDIATE, str.c_str(), "f", spacemapY);
    sender_->sendMessage(str.c_str(), "f", spacemapY, SPATOSC_ARGS_END);

    str = "Bus " + src->getID() + " Level";
    //lo_send_from(destAddr_, lo_serv_, LO_TT_IMMEDIATE, "/set", "sf", str.c_str(), conn->gain());
    sender_->sendMessage("/set", "sf", str.c_str(), conn->gainDB(), SPATOSC_ARGS_END);
}

OscSender &DmitriTranslator::getSender() const
{
    return *sender_;
}

void DmitriTranslator::setEquatorRadius(double radius)
{
	SPACEMAP_EQUATOR_RADIUS = radius;
	// TODO: need to recompute all connections and push them
}

} // end namespace spatosc

