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

#include <regex.h>
#include <string>
#include <vector>
#include <tr1/memory>

namespace spatosc
{

// forward declarations
class Listener;
class Node;
class SoundSource;
class Connection;
class Translator;

/**
 * Manages the nodes and their connections.
 * It also manages the translator, who allow to render a scene with some audio engine.
 */
class Scene
{
    public:
        // iterators:
        typedef std::vector<std::tr1::shared_ptr<Listener> >::iterator listenerIterator;
        typedef std::vector<std::tr1::shared_ptr<Node> >::iterator nodeIterator;
        typedef std::vector<std::tr1::shared_ptr<SoundSource> >::iterator sourceIterator;
        typedef std::vector<std::tr1::shared_ptr<Connection> >::iterator connIterator;
        typedef std::vector<std::tr1::shared_ptr<Connection> >::const_iterator connConstIterator;

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
         * Template method that sets the renderer plugin.
         *
         * The template argument must be a child of Translator. 
         * Here is an example: 
         * \code
         * Scene scene();
         * scene.setTranslator<SpatdifTranslator>("127.0.0.1");
         * \endcode
         */
        template <typename T>
        void setTranslator(const std::string &address)
        {
            translator_.reset(new T(address));
        }

        /**
         * Returns a sound source node in the scene identified by its identifier. Creates it if it does not exist yet.
         * 
         * If a node wih this name already exists, it returns a null pointer and prints an error message.
         * @return A SoundSource pointer. Null if a node with this name already exists. Never free this pointer.
         */
        SoundSource* createSoundSource(const std::string &id);

        /**
         * Returns a node in the scene identified by its identifier.
         * 
         * If a node wih this name already exists, it returns a null pointer and prints an error message.
         * @return A Listener pointer. Null if a node with this name already exists. Never free this pointer.
         */
        Listener* createListener(const std::string &id);

        /**
         * Returns a node in the scene, given its identifier.
         * @return A Node pointer. Null if not found. Never free this pointer.
         */
        Node* getNode(const std::string &id);

        /**
         * Returns a sound source node in the scene identified by its identifier.
         * @return A SoundSource pointer. Null if not found. Never free this pointer.
         */
        SoundSource* getSoundSource(const std::string &id);

        /**
         * Returns a listener node in the scene, given its identifier.
         * @param id Identifier for the Listener node.
         * @return A Listener pointer. Null if not found. Never free this pointer.
         */
        Listener* getListener(const std::string &id);

        /**
         * Returns a list of all connections that "directly involve" a node (ie, as the source or the sink): 
         * @param id Identifier of the node for which we want its connections.
         * @return A vector of Connection pointers. Never free these pointers.
         */
        std::vector<Connection*> getConnectionsForNode(const std::string &id);

        /**
         * Returns a Connection between two node in the scene.
         * 
         * @param source The source node.
         * @param sink The sink node.
         * @return A Connection pointer. Null if not found. Never free this pointer.
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
         */
        void setConnectFilter(std::string s);

        /** 
         * Connects two nodes together.
         * @return A Connection pointer. Null if not found. Never free this pointer.
         */
        Connection* connect(Node *src, Node *snk);

        /** 
         * Disconnects two nodes.
         * @warning Not implemented yet.
         */
        void disconnect(Node *source, Node* sink);

        /**
         * Called by a node when it is changed so that the scene updates all its sibling nodes.
         */
        void update(Node *n);

        /**
         * Called by a connection when it is changed so that the scene updates all its sibling nodes.
         */
        void update(Connection *conn);

    private:

        std::tr1::shared_ptr<Translator> translator_;

        bool autoConnect_;

        std::string connectFilter_;
        regex_t connectRegex_;

        std::vector<std::tr1::shared_ptr<Listener> >  ListenerList_;
        std::vector<std::tr1::shared_ptr<SoundSource> > SoundSourceList_;
        std::vector<std::tr1::shared_ptr<Connection> > ConnectionList_;
};

} // end namespace spatosc

#endif // __SCENE_H__
