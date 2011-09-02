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

#ifndef __WRAPPER_H__
#define __WRAPPER_H__

#include <string>
#include "memory.h"
#include "dllexport.h"

namespace spatosc
{

class Scene;

/**
 * Wraps the whole spatosc library.
 * 
 * Implements the Facade design pattern.
 * It makes the internal details of the library opaque to the user. That means you cannot access the Scene or its nodes directly.
 * 
 * For now, this is the only class that is accessible from Windows using the spatosc dynamic library.
 */
class DLLEXPORT Wrapper
{
    public:
        /**
         * Constructor.
         */
        Wrapper();
        /**
         * Prints the current scene to the console
         */
        void debugPrint();
        /**
         * In synchronous mode, the messages are automaticlly sent evey time
         * a node changes, which may cause a lot of strain on the CPU and the
         * network. It may be desireable to manually control the update rate.
         * To do so, one puts the library into asynchronous mode by setting 
         * the following function with FALSE. Then, the programmer must
         * explicitely update the scene by calling flushMessages() every now
         * and then to flush the OSC messages to be sent.
         */
        void setSynchronous(bool synchronous);
        /**
         * When in asynchronous mode, (not synchronous) one needs to call this
         * quite often to flush the OSC messages. If not, no messages will be
         * sent, and the spatializaiton will not be updated.
         * @return Success or not.
         */
        bool flushMessages();
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
         * Adds a translator with default settings.
         * @param name Is an unique reference name so that the translator may be modified or removed in the future.
         * @param type Must be the name of a valid subclass of the Translator class (eg, "ConsoleTranslator", "BasicTranslator, "DmitriTranslator", etc.).
         * @return Success or not.
        */
        bool addTranslator(const std::string &name, const std::string &type);
        /**
         * Adds a translator and specifies a remote host and port
         * @param name Is an unique reference name so that the translator may be modified or removed in the future.
         * @param type Must be the name of a valid subclass of the Translator class (eg, "ConsoleTranslator", "BasicTranslator, "DmitriTranslator", etc.).
         * @param addr The hostname or IP address or the remote host (or "localhost").
         * @param port The port on the remote host that receives OSC messages.
         * @return Success or not.
         */
        bool addTranslator(const std::string &name, const std::string &type, const std::string &addr, const std::string &port);
        /**
         * Adds a translator and specifies a remote host and port, plus an outgoing port (important in the case of DmitriTranlator).
         * @param name Is an unique reference name so that the translator may be modified or removed in the future.
         * @param type Must be the name of a valid subclass of the Translator class (eg, "ConsoleTranslator", "BasicTranslator, "DmitriTranslator", etc.).
         * @param addr The hostname or IP address or the remote host (or "localhost").
         * @param toPort The port on the remote host that receives OSC messages.
         * @param fromPort The local port on which to create an outgoing socket for sending OSC messages.
         * @return Success or not.
        */
        bool addTranslator(const std::string &name, const std::string &type, const std::string &addr, const std::string &toPort, const std::string &fromPort);
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
         * Set verbose debug printing on a translator.
         * @return Success or not.
         */
        bool setTranslatorVerbose(const std::string &name, bool verbose);
        /**
         * Sets the value of a Node string Property.
         * @return Success or not.
         */
        bool setNodeStringProperty(const std::string &node, const std::string &key, const std::string &value);
        /**
         * Retrieves the value of a Node string Property.
         * @param value A reference to a string in which the value will be written.
         * @return Success or not.
         */
        bool getNodeStringProperty(const std::string &node, const std::string &key, std::string &value);
        /**
         * Removes a Node string Property.
         * @return Success or not.
         */
        bool removeNodeStringProperty(const std::string &node, const std::string &key);
        /**
         * Sets the value of a Node int Property.
         * @return Success or not.
         */
        bool setNodeIntProperty(const std::string &node, const std::string &key, const int &value);
        /**
         * Retrieves the value of a Node int Property.
         * @param value A reference to a int in which the value will be written.
         * @return Success or not.
         */
        bool getNodeIntProperty(const std::string &node, const std::string &key, int &value);
        /**
         * Removes a Node int Property.
         * @return Success or not.
         */
        bool removeNodeIntProperty(const std::string &node, const std::string &key);
        /**
         * Sets the value of a Node float Property.
         * @return Success or not.
         */
        bool setNodeFloatProperty(const std::string &node, const std::string &key, const double &value);
        /**
         * Retrieves the value of a Node float Property.
         * @param value A reference to a float in which the value will be written.
         * @return Success or not.
         */
        bool getNodeFloatProperty(const std::string &node, const std::string &key, double &value);
        /**
         * Removes a Node float Property.
         * @return Success or not.
         */
        bool removeNodeFloatProperty(const std::string &node, const std::string &key);
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
        /**
         * Sets a connection's distance factor.
         * See Connection::setDistanceFactor.
         */
        bool setDistanceFactor(const std::string &sourceNode, const std::string &sinkNode, double factor);
        /**
         * Sets a connection's Doppler factor.
         * See Connection::setDistanceFactor.
         */
        bool setDopplerFactor(const std::string &sourceNode, const std::string &sinkNode, double factor);
        /**
         * Enables or disables a node.
         */
        bool setNodeActive(const std::string &node, bool active);
    
    private:
        std::tr1::shared_ptr<Scene> scene_;
};

} // end of namespace spatosc

#endif // __WRAPPER_H__

