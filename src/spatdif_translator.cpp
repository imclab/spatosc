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
#include "oscutils.h"
#include "oscsender.h"
#include "connection.h"
#include "soundsource.h"
#include "listener.h"

namespace spatosc
{
// TODO:2011-01-19:aalex:Allow to specify the sending port
SpatdifTranslator::SpatdifTranslator(const std::string &ip, bool verbose) :
    Translator(verbose),
    oscSender_(ip, DEFAULT_SEND_PORT)
    {}

SpatdifTranslator::~SpatdifTranslator()
{
}

void SpatdifTranslator::sendPosition(const std::string &prefix, Node *node)
{
    std::string path = prefix +  "/position";
    Vector3 vect(node->getPosition());
    oscSender_.sendMessage(path, "fff", vect.x, vect.y, vect.z, LO_ARGS_END);
}

void SpatdifTranslator::update(Connection * conn)
{
    // FIXME: Downcasts are evil
    SoundSource *src = dynamic_cast<SoundSource*>(conn->src_);
    Listener *snk = dynamic_cast<Listener*>(conn->snk_);
    // TODO:Wed Jan 19 14:47:57 EST 2011:tmatth: set positions, gains as needed!
    if (src == 0)
    {
        std::cerr << "Sound source is NULL!\n";
        return;
    }
    if (snk == 0)
    {
        std::cerr << "Sound source is NULL!\n";
        return;
    }
    if (src->getChannelID() < 0)
        return;

    // update source position
    sendPosition("/SpatDIF/core/source/" + 
            OSCutil::stringify(src->getChannelID()) + "/position", src);

    // FIXME:Wed Jan 19 16:22:42 EST 2011:tmatth should listener have channel ID? SpatDIF thinks so.
    // update sink position
    sendPosition("/SpatDIF/core/listener/1/position", snk);
}

} // end namespace spatosc
