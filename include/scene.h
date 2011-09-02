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
 * The Scene class.
 */

#ifndef __SCENE_H__
#define __SCENE_H__

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "memory.h"
#include "maths.h"
#include "translator.h"

namespace spatosc
{

class Listener;
class Node;
class SoundSource;
class Connection;
class Translator;
class GeoTransform;

/**
 * Manages the nodes and their connections.
 * It also manages the translator, who allow to render a scene with some audio engine.
 */
class Scene
{
    public:
        /**
         * This method should be only used by the Node class.
         * @warning Clients should not need to call this method.
         */
        void applyTransformation(Vector3 &vec) const;
        /**
         * This method should be only used by the Node class.
         * @warning Clients should not need to call this method.
         */
        void applyTransformation(double &x, double &y, double &z) const;
        
        /**
         * Sets the scene's translation.
         */
        void setTranslation(double x, double y, double z);
        /**
         * Sets the scene's orientation.
         * Using a Euler's angle.
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
        void setScale(double x, double y, double z);
        
        // FIXME: Thu Feb  3 12:33:48 EST 2011 : tmatth: this only belongs in osc_scene
        virtual void unsubscribe(Node * /*node*/) {};
        virtual ~Scene() {}
        // iterators:
        typedef std::vector<std::tr1::shared_ptr<Listener> >::iterator ListenerIterator;
        typedef std::vector<std::tr1::shared_ptr<Node> >::iterator NodeIterator;
        typedef std::vector<std::tr1::shared_ptr<SoundSource> >::iterator SourceIterator;
        typedef std::vector<std::tr1::shared_ptr<Connection> >::iterator ConnIterator;
        typedef std::vector<std::tr1::shared_ptr<Connection> >::const_iterator ConnConstIterator;
        typedef std::map<std::string, std::tr1::shared_ptr<Translator> >::iterator TranslatorIterator;


        /**
         * Constructor.
         *
         * Your next steps should be to set the translator and create a listener node.
         *
         * When you create a scene, there is not a single node in the scene and you must create some if you want to do anything.
         */
        Scene();

        /**
         * Prints debug info to the console.
         */
        void debugPrint();

        /**
         * Helper template method that adds a translator plugin with one IP and
         * port (most cases).
         *
         * This is also here for legacy purposes (before we realized that
         * translators may have various constructor arguments). For thos more
         * specialized cases, you have to pass a pointer (see below).
         *
         * The template argument must be derived from Translator class.
         *
         * Each translator must have an identifier, so that you can remove it
         * later. You can have many translators for a single scene.
         * 
         * Here is an example:
         * \code
         * Scene scene();
         * scene.addTranslator<BasicTranslator>("basic", "127.0.0.1", "11111");
         * \endcode
         * 
         * @return A Translator pointer. Null if there was already one with that
         * name, or if an error occurred. Never free this pointer.
         */
        /*
        template <typename T>
        Translator *addTranslator(const std::string &name, const std::string &address="", const std::string &port="", bool verbose = true)
        {
            if (hasTranslator(name))
            {
                std::cout << "Warning: Cannot add translator named " << name << ". Already exists." << std::endl;
                return 0;
            }
            else
            {
                translators_[name] = std::tr1::shared_ptr<Translator>(new T(address, port, verbose));
                return getTranslator(name);
            }
        }
        */

        /**
         * Add a translator by passing a pointer.
         *
         * Here is an example:
         * \code
         * Scene scene();
         * scene.addTranslator("basic", new BasicTranslator("127.0.0.1", "11111"));
         * \endcode
         *
         * IMPORTANT: you should never free the object for the pointer that you
         * passed to this function. Instead, use the removeTranslator() method.
         *
         * @return A Translator pointer. Null if there was already one with that
         * name, or if an error occurred. Never free this pointer.
         */
        Translator *addTranslator(const std::string &name, Translator *t);

        /**
         * Helper class for adding a translator with default arguments
         */
        Translator *addTranslator(const std::string &name, const std::string &type);

        /**
         * Helper class for adding a translator with a remote host
         */

        Translator *addTranslator(const std::string &name, const std::string &type, const std::string &addr, const std::string &port);
        /**
         * Helper class for adding a translator with a remote host and specific outgoing port (important for DmitriTranslator).
         */
        Translator *addTranslator(const std::string &name, const std::string &type, const std::string &addr, const std::string &toPort, const std::string &fromPort);

        /**
         * Returns a pointer to a given Translator or a null pointer if not found.
         * @return A Translator pointer. Never free it.
         */
        Translator *getTranslator(const std::string &name);
        
        /**
         * Removes a translator given its name.
         * @return success or not.
         */
        bool removeTranslator(const std::string &name);

        /**
         * Checks for the existence of a translator given its name.
         * @return It exists or not.
         */
        bool hasTranslator(const std::string &name) const;

        /**
         * Returns a sound source node in the scene identified by its identifier. Creates it if it does not exist yet.
         *
         * If a node wih this name already exists, it returns a null pointer and prints an error message.
         *
         * Once you created a SoundSource, you should probably want to set its position and orientation.
         *
         * @warning Make sure you call this node's setChannelID method after its creation.
         * @return A SoundSource pointer. Null if a node with this name already exists. Never free this pointer. It will become invalid if this node is deleted.
         */
        virtual SoundSource* createSoundSource(const std::string &id);

        /**
         * Returns a node in the scene identified by its identifier.
         *
         * The default position for any node is (0, 0, 0).
         *
         * If a node wih this name already exists, it returns a null pointer and prints an error message.
         * @return A Listener pointer. Null if a node with this name already exists. Never free this pointer. It will become invalid if this node is deleted.
         */
        virtual Listener* createListener(const std::string &id);

        /**
         * Returns a node in the scene, given its identifier.
         * @return A Node pointer. Null if not found. Never free this pointer. It will become invalid if this node is deleted.
         */
        Node* getNode(const std::string &id);

        /**
         * Returns a sound source node in the scene identified by its identifier.
         * @return A SoundSource pointer. Null if not found. Never free this pointer. It will become invalid if this node is deleted.
         */
        SoundSource* getSoundSource(const std::string &id);

        /**
         * Returns a listener node in the scene, given its identifier.
         * @param id Identifier for the Listener node.
         * @return A Listener pointer. Null if not found. Never free this pointer. It will become invalid if this node is deleted.
         */
        Listener* getListener(const std::string &id);

        /**
         * Returns a Connection between two node in the scene.
         *
         * @param source The source node.
         * @param sink The sink node.
         * @return A Connection pointer. Null if not found. Never free this pointer. It will become invalid if this connection is deleted.
         */
        Connection* getConnection(const Node *source, const Node *sink);

        /**
         * Sets the audio filter to add to each audio connection.
         *
         * The current connectFilter affects the following connections to occur.
         * A connectFilter is a regular expression.
         * An asterisk character (*) means it's a wildcard.
         *
         * When the two nodes are connected, we check the identifiers of the source and sink nodes.
         * If either of those two id match, we proceed with the connection.
         *
         * You can use this as a blacklist to avoid connection some nodes according to their id.
         *
         * Example:
         *
         * One scene maintains a list of all audio sources, listeners and connections between them.
         * Let say you write a game using a 3D library such as Ogre. You have many players.
         * Each of those players might have an external process that is an audio renderer.
         * For this game process, you'd have several plugins instances that would send different
         * OSC streams to their respective renderers. For each of those plugins instances, you'd have
         * a different connect filter, so that player A listens to player B, but not to itself.
         *
         * @return Success or not.
         */
        bool setConnectFilter(std::string s);

        /**
         * Connects two nodes together.
         * @return A Connection pointer. Null if either of the two nodes are not found, or if they are already connected. Never free this pointer. It will become invalid if this connection is deleted.
         */
        Connection* connect(SoundSource *src, Listener *snk);

        /**
         * Disconnects two nodes.
         * @return True if it successfully disconnected the nodes. False if they were not connected or if the nodes are invalid.
         */
        bool disconnect(SoundSource *source, Listener* sink);

        /**
         * Sets whether source and sink nodes should be automatically connected as soon as they are created.
         * Default is true.
         * @param enabled Enabled or not.
         */
        void setAutoConnect(bool enabled);

        /**
         * Returns whether source and sink nodes should be automatically connected as soon as they are created.
         * Default is true.
         * @return Enabled or not.
         */
        bool getAutoConnect() const;

        /**
         * Deletes a Listener node
         * @param node Listener pointer.
         * @return Whether it deleted a node or not.
         */
        bool deleteNode(const Listener *node);

        /**
         * Deletes a SoundSource node
         * @param node SoundSource pointer.
         * @return Whether it deleted a node or not.
         */
        bool deleteNode(const SoundSource *node);
        /**
         * Sets the console verbosity for info messages.
         * @param Whether to print a lot of messages or not.
         */
        void setVerbose(bool verbose);

        /**
         * Delete all nodes in the scene.
         * Useful, so that you don't need to set the translator again.
         */
        void deleteAllNodes();

        /**
         * In synchronous mode, the messages are automaticlly sent evey time a node changes, otherwise the programmer needs to call flushMessages() every now and then to flush the OSC messages to be sent.
         * Default is to be in the synchronous mode. (true)
         * @param synchronous Whether it should be in the synchronous mode or not.
         */
        void setSynchronous(bool synchronous);

        bool isSynchronous() { return synchronous_; }

        /**
         * When in asynchronous mode, (not synchronous) one needs to call this quite often to flush the OSC messages.
         * To be in the asynchronous mode, one should setSynchronous with false as parameter.
         * @return Success or not. Returns false if in synchronous mode. (that would be an error to call this if this is the case)
         */
        bool flushMessages();

        /**
         * Called when a new connection is made or when its node position change, so that the scene updates all its sibling nodes, and the translator may push some OSC messages.
         */
        void onConnectionChanged(Connection *conn, bool forcedNotify=false);
        /**
         * Called when a Node Property value has changed.
         * Tells all the translators that the given property has changed.
         * @warning Clients should not call this directly.
         */
        template <typename T>
        void onPropertyChanged(Node *node, const std::string &key, const T &value)
        {
            std::map<std::string, std::tr1::shared_ptr<Translator> >::iterator iter;
            for (iter = translators_.begin(); iter != translators_.end(); ++iter)
                iter->second->pushPropertyChange(node, key, value);
        }

        void onSceneChanged(const char *types, ...);

        /**
         * Allows the scene to be completely refreshed. For example, if the
         * remote spatializer crashes and restarts, this function can be called
         * to send updates for all nodes.
         */
        void forceRefresh();

    private:
        // private handle class
        struct RegexHandle;
        std::tr1::shared_ptr<RegexHandle> connectRegex_;
        std::tr1::shared_ptr<GeoTransform> transform_;

        /**
         * Returns a list of all connections that "directly involve" a node (ie, as the source or the sink):
         * @param Node pointer for which we want its connections.
         * @return A vector of Connection pointers. Never free these pointers. They will become invalid if these connections are deleted.
         */
        std::vector<Connection*> getConnectionsForNode(const Node *node);
        bool disconnectNodeConnections(const Node *node);
        void onTransformChanged();

        std::map<std::string, std::tr1::shared_ptr<Translator> > translators_;
        bool autoConnect_;
        std::string connectFilter_;

        //FIXME:2011-01-25:aalex:Would maps be faster?
        std::vector<std::tr1::shared_ptr<Listener> >  listeners_;
        std::vector<std::tr1::shared_ptr<SoundSource> > soundSources_;
        std::vector<std::tr1::shared_ptr<Connection> > connections_;
        bool verbose_;
        bool synchronous_;
        void pushConnectionChangesViaAllTranslators(Connection *conn, bool forcedNotify=false);
};

} // end namespace spatosc

#endif // __SCENE_H__
