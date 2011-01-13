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
 * The vAudioManager class.
 */

#ifndef __vAudioManager_H
#define __vAudioManager_H

#include <regex.h>
#include <string>
#include <vector>
#include <tr1/memory>

namespace spatosc
{
// forward declarations
class vListener;
class vBaseNode;
class vSoundSource;
class vSoundConn;
class Translator;

/**
 * The Audio Manager manages the nodes and their connections.
 * It also manages the plugin, that is a renderer.
 */
class vAudioManager
{
    public:
        // iterators:
        typedef std::vector<std::tr1::shared_ptr<vListener> >::iterator listenerIterator;
        typedef std::vector<std::tr1::shared_ptr<vBaseNode> >::iterator nodeIterator;
        typedef std::vector<std::tr1::shared_ptr<vSoundSource> >::iterator sourceIterator;
        typedef std::vector<std::tr1::shared_ptr<vSoundConn> >::iterator connIterator;

        /**
         *  Singleton instance reference
         */
        static vAudioManager& Instance();

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
        vSoundSource* getOrCreateSoundSource(const std::string &id);

        /**
         * Returns a node in the scene identified by its identifier.
         */
        vListener* getOrCreateListener(const std::string &id);

        /**
         * Returns a node in the scene identified by its identifier.
         */
        vBaseNode* getNode(const std::string &id);

        /**
         * Returns a sound source node in the scene identified by its identifier.
         */
        vSoundSource* getSoundSource(const std::string &id);

        /**
         * Returns a listener node in the scene identified by its identifier.
         */
        vListener* getListener(const std::string &id);

        /**
         * Returns all the audio connections in the scene.
         */
        std::vector<vSoundConn*> getConnections(const std::string &id);

        /**
         * Returns a connection in the scene identified by its identifier.
         */
        vSoundConn* getConnection(const std::string &src, const std::string &snk);
        vSoundConn* getConnection(const std::string &id);

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
         * One audio manager maintains a list of all audio sources, listeners and connections between them. Let say you write a game using a 3D library such as Ogre. You have many players. Each of those players might have an external process that is an audio renderer. For this game process, you'd have several plugins instances that would send different OSC streams to their respective renderers. For each of those plugins instances, you'd have a different connect filter, so that player A listens to player B, but not to itself. 
         */
        void setConnectFilter(std::string s);

        /** 
         * Connects two nodes together, using their identifiers.
         */
        vSoundConn* connect(const std::string &src, const std::string &snk);

        /** 
         * Connects two nodes together.
         */
        vSoundConn* connect(vBaseNode *src, vBaseNode *snk);

        /** 
         * Disconnects two nodes.
         */
        void disconnect(vSoundConn *conn);

        /**
         * Called by a node when it is changed so that the audio manager updates all its sibling nodes.
         */
        void update(vBaseNode *n);

        /**
         * Called by a connection when it is changed so that the audio manager updates all its sibling nodes.
         */
        void update(vSoundConn *conn);

    private:

        // singleton constructors & desctructor (hidden):
        vAudioManager();
        ~vAudioManager();
        vAudioManager(vAudioManager const&); // copy constructor
        // hide the assignment operator, otherwise it would be possible to
        // assign the singleton vAudioManager to itself:
        vAudioManager& operator=(vAudioManager const&);

        std::tr1::shared_ptr<Translator> translator_;

        bool autoConnect_;

        std::string connectFilter_;
        regex_t connectRegex_;

        std::vector<std::tr1::shared_ptr<vListener> >  vListenerList_;
        std::vector<std::tr1::shared_ptr<vSoundSource> > vSoundSourceList_;
        std::vector<std::tr1::shared_ptr<vSoundConn> > vSoundConnList_;
};
};

#endif
