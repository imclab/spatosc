/*
 * spatdif_receiver.h
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
 * The SpatdifReceiver class.
 */

#ifndef _SPATDIF_RECEIVER_H_
#define _SPATDIF_RECEIVER_H_

#include <string>
#include "memory.h"
#include <lo/lo.h>

namespace spatosc
{

class OscReceiver;

class SpatdifHandler
{
    public:
        virtual ~SpatdifHandler() {};
        static int onOSCMessage(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
    private:
        virtual void xyz(const std::string &id, float x, float y, float z) = 0;
        virtual void aed(const std::string &id, float azimuth, float elevation, float distanceMeters);
        virtual void xy(const std::string &id, float x, float y);
        virtual void delay(const std::string &id, float delay);
        virtual void gainDB(const std::string &id, float gainDB);
        virtual void gain(const std::string &id, float gain);
        virtual void spread(const std::string &id, float spread);
        virtual void spreadAE(const std::string &id, float azimSpread, float elevSpread);
};

/**
 * Useful for clients to receive messages from spatosc's SpatdifTranslator.
 */
class SpatdifReceiver
{
    public:
        /**
         * Constructor.
         * Starts listening for OSC messages on the given port.
         * @param port String that must be a valid port number.
         */
        SpatdifReceiver(const std::string &port, SpatdifHandler *handler, bool verbose = false);
        /**
         * Checks for incoming OSC messages.
         */
        void poll();
    private:
        void registerCallbacks(SpatdifHandler * handler);
        std::tr1::shared_ptr<OscReceiver> receiver_;
        bool verbose_;
};

} // end namespace spatosc
#endif // _SPATDIF_RECEIVER_H_