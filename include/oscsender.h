/*
 * oscsender.h
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
 * The OscSender class.
 */
#ifndef __OSC_SENDER_H__
#define __OSC_SENDER_H__

#include <lo/lo.h>
#include <string>
#include "oscutils.h"



namespace spatosc
{

/** General-purpose wrapper around liblo to send OSC messages.
 */
class OscSender
{
    public:
        /**
         * Constructor that will create server on fromPort for sending messages to host:toPort.
         */
        OscSender(const std::string &host, const std::string &toPort, const std::string &fromPort);
        /**
         * Constructor that will create a random server for sending messages to host:toPort.
         */
        OscSender(const std::string &host, const std::string &toPort);
        std::string toString() const;
        const char * host() { return host_.c_str(); }
        const char * port() { return toPort_.c_str(); }
         /**
          * Send a message with one or more string, float and/or int args.
          * The list of arguments MUST be terminated with SPATOSC_ARGS_END.
          * Here is an example:
          * \code
          * sendMessage("/oscpath", "fsi", 5.5, "foo", 4, SPATOSC_ARGS_END);
          * \endcode
          */
        void sendMessage(const std::string &OSCpath, const char *types, ...) const;

        void sendMessage(const std::string &OSCpath, const char *types, va_list ap) const;

    private:
        // not implemented
        OscSender(const OscSender&);
        const OscSender& operator=(const OscSender&);
        void sendMessage(const std::string &OSCpath, lo_message msg) const;
        std::string host_;
        std::string toPort_;
        std::string fromPort_;
        lo_address address_;
        lo_server server_;
};

} // end of namespace spatosc

#endif // _OSC_SENDER_H_

