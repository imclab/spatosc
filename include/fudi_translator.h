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

/** @file
 * The FudiTranslator class.
 */

#ifndef _FUDI_TRANSLATOR_H_
#define _FUDI_TRANSLATOR_H_

#include "translator.h"
#include <string>
#include <sstream>

namespace spatosc
{

class OscSender;
class Node;

/**
 * Translator to send FUDI messages to Pure Data.
 */
class FudiTranslator : public Translator
{
    public:
        FudiTranslator(const std::string &ip, const std::string &port, bool verbose);
        virtual void pushOSCMessages(Connection *conn);
        //virtual void pushPropertyChange(Node *node, const std::string &key, const std::string &value);
        static const char *DEFAULT_SEND_PORT;
        static const char *DEFAULT_RECEIVER_PORT;

    private:
        // not implemented
        FudiTranslator(const FudiTranslator&);
        const FudiTranslator& operator=(const FudiTranslator&);
        void sendFudi(const std::string &message);
        unsigned int port_;
        std::string ip_;
        void sendPosition(const std::string &prefix, Node *node);
};

/**
 * Formats a series of basic types atoms into a FUDI message.
 */
class FudiMessage
{
    public:
        FudiMessage();
        /**
         * Appends an atom to the FUDI message.
         * @return A reference to this, so that we can daisy-chain calls to this method.
         */
        template <typename T>
        FudiMessage &add(const T &atom)
        {
            std::ostringstream os;
            os << message_;
            if (containsSomething_)
                os << " ";
            os << atom;
            message_ = os.str();
            containsSomething_ = true;
            return *this;
        }
        void clear();
        std::string toString() const;
    private:
        bool containsSomething_;
        std::string message_;
};

} // end namespace spatosc
#endif // _FUDI_TRANSLATOR_H_
