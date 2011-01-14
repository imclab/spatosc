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

#ifndef __Scene_H
#define __Scene_H

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

        /**
         *  Singleton instance reference
         */
        static Scene& Instance();

        /**
         * Prints debug info to the console.
         */
        void debugPrint();

        /**
         * Sets the renderer plugin.
         *
         * Accepts a child class of Translator.
         */
        void setTranslator(const std::tr1::shared_ptr<Translator> &p);

        /**
         * Returns a sound source node in the scene identified by its identifier. Creates it if it does not exist yet.
         */
        SoundSource* getOrCreateSoundSource(const std::string &id);

        /**
         * Returns a node in the scene identified by its identifier.
         */
        Listener* getOrCreateListener(const std::string &id);

        /**
         * Returns a node in the scene identified by its identifier.
         */
        Node* getNode(const std::string &id);

        /**
         * Returns a sound source node in the scene identified by its identifier.
         */
        SoundSource* getSoundSource(const std::string &id);

        /**
         * Returns a listener node in the scene identified by its identifier.
         */
        Listener* getListener(const std::string &id);

        /**
         * Returns all the audio connections in the scene.
         */
        std::vector<Connection*> getConnections(const std::string &id);

        /**
         * Returns a connection in the scene identified by its identifier.
         */
        Connection* getConnection(const std::string &src, const std::string &snk);
        Connection* getConnection(const std::string &id);

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
         * Connects two nodes together, using their identifiers.
         */
        Connection* connect(const std::string &src, const std::string &snk);

        /** 
         * Connects two nodes together.
         */
        Connection* connect(Node *src, Node *snk);

        /** 
         * Disconnects two nodes.
         */
        void disconnect(Connection *conn);

        /**
         * Called by a node when it is changed so that the scene updates all its sibling nodes.
         */
        void update(Node *n);

        /**
         * Called by a connection when it is changed so that the scene updates all its sibling nodes.
         */
        void update(Connection *conn);

    private:

        // singleton constructors & desctructor (hidden):
        Scene();
        ~Scene();
        Scene(Scene const&); // copy constructor
        // hide the assignment operator, otherwise it would be possible to
        // assign the singleton Scene to itself:
        Scene& operator=(Scene const&);

        // TODO: Fri Jan 14 11:14:34 EST 2011: have mulitple translators
        std::tr1::shared_ptr<Translator> translator_;

        bool autoConnect_;

        std::string connectFilter_;
        regex_t connectRegex_;

        std::vector<std::tr1::shared_ptr<Listener> >  ListenerList_;
        std::vector<std::tr1::shared_ptr<SoundSource> > SoundSourceList_;
        std::vector<std::tr1::shared_ptr<Connection> > ConnectionList_;
};

} // end namespace spatosc

#endif
