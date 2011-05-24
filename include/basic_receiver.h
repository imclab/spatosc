/*
 * basic_receiver.h
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
 * The BasicReceiver class.
 */

#ifndef _BASIC_RECEIVER_H_
#define _BASIC_RECEIVER_H_

#include <string>
#include "oscreceiver.h"
#include "memory.h"

namespace spatosc
{

/**
 * Useful for clients to receive messages from spatosc's BasicTranslator.
 */
class BasicReceiver : public OscReceiver
{
    public:
        /**
         * Constructor.
         * Starts listening for OSC messages on the given port.
         * @param port String that must be a valid port number.
         */
        BasicReceiver(const std::string &port, bool verbose = false);
        /**
         * Checks for incoming OSC messages.
         */
        int poll();
        static int onSceneMessage(const char * path, const char * /*types*/,
        lo_arg ** argv, int argc, void * /*data*/, void *user_data);

        static int onNodeMessage(const char * path, const char * /*types*/,
        lo_arg ** argv, int argc, void * /*data*/, void *user_data);

    private:
        bool verbose_;
};

} // end namespace spatosc

#endif // _BASIC_RECEIVER_H_

