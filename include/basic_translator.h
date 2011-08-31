/*
 * basic_translator.h
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
 * The BasicTranslator class.
 */

#ifndef _BASIC_TRANSLATOR_H_
#define _BASIC_TRANSLATOR_H_

#include "translator.h"
#include "memory.h"
#include <string>

namespace spatosc
{

class OscSender;
class Node;

/**
 * Translator for the Basic protocol.
 */
class BasicTranslator : public Translator
{
    public:

        static const char *DEFAULT_SEND_PORT;

        BasicTranslator(
                const std::string &ip,
                const std::string &port = DEFAULT_SEND_PORT,
                bool verbose = false);

        virtual void debugPrint();
        virtual void pushConnectionChanges(Connection *conn);
        //virtual void pushSceneChange(const std::string &method, ...);
        virtual void pushSceneChange(const char *types, va_list ap);

        virtual void pushPropertyChange(Node *node, const std::string &key, const std::string &value);
        virtual void pushPropertyChange(Node *node, const std::string &key, const double &value);
        virtual void pushPropertyChange(Node *node, const std::string &key, const int &value);


    private:
        std::tr1::shared_ptr<OscSender> sender_;
        void sendPosition(const std::string &prefix, Node *node);
        void sendAED(const std::string &prefix, Connection *conn);
        void sendDelay(const std::string &prefix, Connection *conn);
        void sendGainDB(const std::string &prefix, Connection *conn);
        // not implemented
        BasicTranslator(const BasicTranslator&);
        const BasicTranslator& operator=(const BasicTranslator&);
};

} // end namespace spatosc
#endif // _BASIC_TRANSLATOR_H_
