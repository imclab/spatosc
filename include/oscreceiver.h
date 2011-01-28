/*
 * oscreceiver.h
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
 * The OscReceiver class.
 */

#ifndef __OSC_RECEIVER_H__
#define __OSC_RECEIVER_H__

#include <lo/lo.h>
#include <string>

namespace spatosc
{

/** General-purpose wrapper around liblo to receive OSC messages.
 */
class OscReceiver
{
    public:
        /**
         * Constructor. Note that the port is a string, not an int.
         */
        OscReceiver(const std::string &port);
        ~OscReceiver();
        /**
         * Returns the port as a C string.
         */
        const char * port() const { return port_.c_str(); }
        /**
         * Add a callback for a given OSC path.
         */
        void addHandler(const char *path,
                const char *types, lo_method_handler handler,
                void *user_data);
        /**
         * Polls the liblo server for received messages. Clients
         * may want to repeatedly call it while it's still
         * receiving bytes. 
         * 
         * \code
         * do
         * {
         *   bytes = receiver->receive();
         * }
         * while (bytes > 0)
         * \endcode
         */
        int receive();

        static int onSourceMessage(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);

    private:
        // not implemented
        OscReceiver(const OscReceiver&);
        const OscReceiver& operator=(const OscReceiver&);

        std::string toString() const;
        std::string port_;
        lo_server server_;
#ifdef CONFIG_DEBUG
        static int genericHandler(const char *path,
                const char *types, lo_arg **argv,
                int argc, void *data, void *user_data);
#endif
        static void error(int num, const char *msg, const char *path);
};

} // end namespace spatosc

#endif // __OSC_RECEIVER_H__
