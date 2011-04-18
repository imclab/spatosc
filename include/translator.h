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

#include <string>
#include <iostream>
#include <cstdarg> // for va_list
#include "unused.h" // FIXME: we should not define this macro in a public header

namespace spatosc
{

class Connection;
class Node;

/**
 * Base class for translator which send OSC messages to audio rendering engines.
 */
class Translator
{
    public:
        /**
         * Constructor.
         * @param verbose Whether it should print info to the console or not.
         */
        Translator(bool verbose = false);

        /**
         * The main work of the translator is done by the pushConnectionChanges() method, which is
         * called whenever there is a change to some parameters within a connection.
         *
         * For each pair of nodes in the scene that are connected, it might calculate the
         * distance between those two, their relative position, gain and other attributes.
         * It should then make sure there is some audio rendering going, either by sending interprocess
         * messages to control some audio engine, or by rendering audio by itself.
         */
        virtual void pushConnectionChanges(Connection *conn);

        /**
         * This is called whenever the scene changes (ie, a node is created or,
         * deleted, a connection is made, etc). The change is available to any
         * translator so that the proper resources can be allocated in the audio
         * rendering process.
         */
        //virtual void pushSceneChange(const std::string &method, ...)
        virtual void pushSceneChange(const char *types, va_list ap)
        {
            UNUSED(types);
            UNUSED(ap);
        }

        /**
         * Called when it's time to push OSC messages when a Node property has changed.
         */
        virtual void pushPropertyChange(Node *node, const std::string &key, const std::string &value)
        {
            UNUSED(node);
            UNUSED(key);
            UNUSED(value);
            std::cout << "Warning: Translator::" << __FUNCTION__ << "(string) not overriden.\n";
        }

        /**
         * Called when it's time to push OSC messages when a Node property has changed.
         */
        virtual void pushPropertyChange(Node *node, const std::string &key, const double &value)
        {
            UNUSED(node);
            UNUSED(key);
            UNUSED(value);
            std::cout << "Warning: Translator::" << __FUNCTION__ << "(double) not overriden.\n";
        }

        /**
         * Called when it's time to push OSC messages when a Node property has changed.
         */
        virtual void pushPropertyChange(Node *node, const std::string &key, const int &value)
        {
            UNUSED(node);
            UNUSED(key);
            UNUSED(value);
            std::cout << "Warning: Translator::" << __FUNCTION__ << "(int) not overriden.\n";
        }

        /**
         * Virtual classes should have virtual destructors.
         */
        virtual ~Translator() {}

        /**
         * Returns whether this translator is verbose or not.
         */
        bool isVerbose() const;

    protected:
        bool verbose_;
};

/**
 * Translator that only prints messages to the console.
 */
class ConsoleTranslator : public Translator
{
    public:
        ConsoleTranslator(const std::string &ip, const std::string &port, bool verbose);
};

} // end namespace spatosc

#endif // __TRANSLATOR_H__
