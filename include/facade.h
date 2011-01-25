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
 * The Wrapper class. 
 */

#ifndef __FACADE_H__
#define __FACADE_H__

#include <string>
#include <map>
#include <iostream>
#include <tr1/memory>

namespace spatosc
{

class Scene;

/**
 * Wraps the whole spatosc library.
 * @warning Most of it is not implemented. Will be renamed to something like Manager, World or so.
 */
class Wrapper
{
    public:
        Wrapper();
        /** Connects two nodes. */
        bool connect(const std::string &nodeFrom, const std::string &nodeTo);
        /** Creates a listener. */
        bool createListener(const std::string &nodeName);
        /** Creates a sound source. */
        bool createSource(const std::string &nodeName);
        /** Deletes a node. */
        bool deleteNode(const std::string &nodeName);
        /** @warning Not implemented. */
        bool clearScene();
        /** Disconnects two nodes. */
        bool disconnect(const std::string &nodeFrom, const std::string &nodeTo);
        /** Enables/disables auto connection of source and sinks. */
        bool setAutoConnect(bool enabled);
        /** Sets the connection regulra expression filter. */
        bool setConnectFilter(const std::string &filterRegex);
        /** Sets a node's orientation. */
        bool setOrientation(const std::string &nodeName, double pitch, double roll, double yaw);
        /** Sets a node's position. */
        bool setPosition(const std::string &nodeName, double x, double y, double z);
        /** Sets a sound source's channel number. */
        bool setSourceChannel(const std::string &nodeName, int channel);
        /**
         * Sets the translator to use. 
         * @param translatorName Must be the name of a valid child of the Translator class.
        */
        bool setTranslator(const std::string &translatorName, const std::string &sendToAddress);
        /** @warning Not implemented. */
        bool setTranslatorProperty(const std::string &key, const std::string &value);
    private:
        std::tr1::shared_ptr<Scene> scene_;
};

} // end of namespace spatosc

#endif
