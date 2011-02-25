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
#include "memory.h"

namespace spatosc
{

class Scene;

/**
 * Wraps the whole spatosc library.
 */
class Wrapper
{
    public:
        /**
         * Constructor.
         */
        Wrapper();
        /**
         * Connects two nodes.
         * @return Success or not.
         */
        bool connect(const std::string &nodeFrom, const std::string &nodeTo);
        /**
         * Creates a listener.
         * @return Success or not.
         */
        bool createListener(const std::string &nodeName);
        /**
         * Creates a sound source.
         * @return Success or not.
         */
        bool createSource(const std::string &nodeName);
        /**
         * Deletes a node.
         * @return Success or not.
         */
        bool deleteNode(const std::string &nodeName);
        /**
         * Clears all nodes and connections in the scene.
         * @return Success or not.
         */
        bool clearScene();
        /**
         * Disconnects two nodes.
         * @return Success or not.
         */
        bool disconnect(const std::string &nodeFrom, const std::string &nodeTo);
        /**
         * Enables/disables auto connection of source and sinks.
         * @return Success or not.
         */
        bool setAutoConnect(bool enabled);
        /**
         * Sets the connection regulra expression filter.
         * @return Success or not.
         */
        bool setConnectFilter(const std::string &filterRegex);
        /**
         * Sets a node's orientation.
         * @return Success or not.
         */
        bool setOrientation(const std::string &nodeName, double pitch, double roll, double yaw);
        /**
         * Sets a node's position.
         * @return Success or not.
         */
        bool setPosition(const std::string &nodeName, double x, double y, double z);
        /**
         * Adds a translator to use.
         * Example of valid translators names include "SpatdifTranslator" and "DmitriTranslator".
         * @param translatorName Must be the name of a valid child of the Translator class.
         * @return Success or not.
        */
        bool addTranslator(const std::string &name, const std::string &translatorName, const std::string &sendToAddress, const std::string &port);
        /**
         * Removes a translator.
         * @return Success or not.
         */
        bool removeTranslator(const std::string &name);
        /**
         * Returns whether we have a translator with that name or not.
         * @return It exists or not.
         */
        bool hasTranslator(const std::string &name);
        /**
         * @warning Not implemented.
         * @return Success or not.
         */
        bool setTranslatorProperty(const std::string &key, const std::string &value);
        /**
         * @warning Not implemented.
         * @return Success or not.
         */
        bool setNodeProperty(const std::string &key, const std::string &value);
        /**
         * Sets the scene's translation.
         */
        void setTranslation(double tx, double ty, double tz);
        /**
         * Sets the scene's orientation.
         * Using a Euleur's angle.
         */
        void setOrientation(double pitch, double roll, double yaw);
        /**
         * Sets the scene's orientation.
         * Using a quaternion.
         */
        void setOrientation(double x, double y, double z, double w);
        /**
         * Sets the scene's scale.
         */
        void setScale(double sx, double sy, double sz);
    private:
        std::tr1::shared_ptr<Scene> scene_;
};

} // end of namespace spatosc

#endif
