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

namespace spatosc
{

SpatdifTranslator::SpatdifTranslator(const std::string &ip) :
    destAddr_(lo_address_new(ip.c_str(), DEFAULT_SEND_PORT)),
    lo_serv_(lo_server_new(DEFAULT_RECEIVER_PORT, NULL))
    {}

SpatdifTranslator::~SpatdifTranslator()
{
    // destructor
    lo_server_free(lo_serv_);
    lo_address_free(destAddr_);
}

void SpatdifTranslator::update(Connection * /*conn*/)
{
    // TODO:Wed Jan 19 14:47:57 EST 2011:tmatth: set positions, gains as needed!
}

} // end namespace spatosc
