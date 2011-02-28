/*
 * spatdif_translator.h
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

/** @file
 * The SpatdifTranslator class.
 */

#ifndef _SPATDIF_TRANSLATOR_H_
#define _SPATDIF_TRANSLATOR_H_

#include "translator.h"
#include "memory.h"
#include <string>

namespace spatosc
{

class OscSender;
class Node;

/**
 * Translator for the SpatDIF protocol.
 */
class SpatdifTranslator : public Translator
{
    public:
        SpatdifTranslator(const std::string &ip, const std::string &port, bool verbose);
        virtual void pushOSCMessages(Connection *conn);
        static const char *DEFAULT_SEND_PORT;
        static const char *DEFAULT_RECEIVER_PORT;

    private:
        std::tr1::shared_ptr<OscSender> sender_;
        void sendPosition(const std::string &prefix, Node *node);
        void sendAED(const std::string &prefix, Connection *conn);
        void sendDelay(const std::string &prefix, Connection *conn);
        void sendGainDB(const std::string &prefix, Connection *conn);
        // not implemented
        SpatdifTranslator(const SpatdifTranslator&);
        const SpatdifTranslator& operator=(const SpatdifTranslator&);
};

} // end namespace spatosc
#endif // _SPATDIF_TRANSLATOR_H_
