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
#include "listener.h"
#include "connection.h"

#include <iostream>
#include <string>

namespace spatosc
{

const char *DmitriTranslator::DEFAULT_SEND_PORT = "18033";
const char *DmitriTranslator::DEFAULT_RECEIVER_PORT = "18099";

// ************************************************

DmitriTranslator::DmitriTranslator(const std::string &ip, const std::string &toPort, const std::string &fromPort) :
Translator(),
sender_(new OscSender(ip, toPort, fromPort))
{
}

/*
DmitriTranslator::DmitriTranslator(const std::string &ip, const std::string &toPort) :
Translator(),
sender_(new OscSender(ip, toPort, DmitriTranslator::DEFAULT_RECEIVER_PORT))
{
}
*/

void DmitriTranslator::debugPrint()
{
    std::cout << "  D-Mitri Translator -- " << sender_->toString() << std::endl;
}

void DmitriTranslator::pushConnectionChanges(Connection *conn)
{
    std::ostringstream ostr;

    int busID = 1; // default bus 1
    conn->getSource()->getIntProperty("bus", busID);

    int enableGain = 1;
    conn->getSource()->getIntProperty("enableGain", enableGain);

    int enableDelay = 0;
    conn->getSource()->getIntProperty("enableDelay", enableDelay);

    double spacemapEquator = 700.0;
    conn->getSink()->getFloatProperty("spacemapEquator", spacemapEquator);
    
    double spacemapPole = 1000.0;
    conn->getSink()->getFloatProperty("spacemapPole", spacemapPole);


    float r = 1.0 - fabs( conn->elevation() / (M_PI/2) );
    float spacemapX, spacemapY;
    if (conn->elevation() >= 0)
    {
        spacemapX = -sin(conn->azimuth()) * r * spacemapEquator;
        spacemapY =  cos(conn->azimuth()) * r * spacemapEquator;
    } else {
        spacemapX = -sin(conn->azimuth()) * (spacemapEquator + ((1-r) * (spacemapPole-spacemapEquator)));
        spacemapY =  cos(conn->azimuth()) * (spacemapEquator + ((1-r) * (spacemapPole-spacemapEquator)));
    }

    //std::cout << "azim="<<conn->azimuth()<<" elev="<<conn->elevation()<<" r="<<r<<std::endl;

    ostr.str("");
    ostr << "/spacemap/" << busID << "/x";
    sender_->sendMessage(ostr.str().c_str(), "f", spacemapX, SPATOSC_ARGS_END);
    //std::cout << "sent " << ostr.str() <<" "<< spacemapX << std::endl;

    ostr.str("");
    ostr << "/spacemap/" << busID << "/y";
    sender_->sendMessage(ostr.str().c_str(), "f", spacemapY, SPATOSC_ARGS_END);
    //std::cout << "sent " << ostr.str() <<" "<< spacemapY << std::endl;

    if (enableGain)
    {
        ostr.str("");
        ostr << "Bus " << busID << " Level";
        sender_->sendMessage("/set", "sf", ostr.str().c_str(), conn->gainDB(), SPATOSC_ARGS_END);
        //std::cout << "sent " << ostr.str() <<" "<< conn->gainDB() << std::endl;
    }

    if (enableDelay)
    {
        // NOTE: Dmitri expects delay as nanoseconds (INT64)
        ostr.str("");
        ostr << "Bus " << busID << " Delay";
        sender_->sendMessage("/set", "sh", ostr.str().c_str(), (long int)(conn->delay()*1000000), SPATOSC_ARGS_END);
        //std::cout << "sent " << ostr.str() <<" "<< conn->delay() << std::endl;
    }
}

OscSender &DmitriTranslator::getSender() const
{
    return *sender_;
}

} // end namespace spatosc

