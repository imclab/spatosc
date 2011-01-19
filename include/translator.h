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
 * The Translator class.
 */
#ifndef __TRANSLATOR_H__
#define __TRANSLATOR_H__

namespace spatosc
{

class Connection;

/**
 * Base class for translator which send OSC messages to audio rendering engines.
 */
class Translator
{
    public:
        /**
         * The main work of the translator is done by the update() method, which is
         * called whenever there is a change to some parameters within a connection.
         * 
         * For each pair of nodes in the scene that are connected, it might calculate the 
         * distance between those two, their relative position, gain and other attributes.
         * It should then make sure there is some audio rendering going, either by sending interprocess
         * messages to control some audio engine, or by rendering audio by itself.
         */
        virtual void update(Connection *conn);

        /**
         * Virtual classes should have virtual destructors.
         */
        virtual ~Translator() {}

        static const char *DEFAULT_SEND_PORT;
        static const char *DEFAULT_RECEIVER_PORT;
};

} // end namespace spatosc

#endif // __TRANSLATOR_H__
