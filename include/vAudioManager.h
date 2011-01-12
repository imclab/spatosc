/** @file
 * The vAudioManager class.
 */

#ifndef __vAudioManager_H
#define __vAudioManager_H

#include <regex.h>
#include <string>
#include <vector>

// forward declarations
class vListener;
class vBaseNode;
class vSoundSource;
class vSoundConn;
class vPlugin;

/**
 * The Audio Manager manages the nodes and their connections.
 * It also manages the plugin, that is a renderer.
 */
class vAudioManager
{
    public:
        // iterators:
        typedef std::vector<vListener*>::iterator listenerIterator;
        typedef std::vector<vBaseNode*>::iterator nodeIterator;
        typedef std::vector<vSoundSource*>::iterator sourceIterator;
        typedef std::vector<vSoundConn*>::iterator connIterator;

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
         * Accepts a child class of vPlugin.
         */
        void setPlugin(vPlugin *p);

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
         * Updates all the connected nodes.
         */
        void update(vBaseNode *n);

        /**
         * Updates the renderer plugin.
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

        vPlugin *plugin_;

        bool autoConnect_;

        std::string connectFilter_;
        regex_t connectRegex_;

        std::vector<vListener*>  vListenerList_;
        std::vector<vSoundSource*> vSoundSourceList_;
        std::vector<vSoundConn*> vSoundConnList_;
};

#endif
