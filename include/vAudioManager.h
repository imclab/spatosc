#ifndef __vAudioManager_H
#define __vAudioManager_H

#include <regex.h>
#include <string>
#include <vector>

#include "vBaseNode.h"
#include "vListener.h"
#include "vSoundSource.h"
#include "vSoundConn.h"
#include "vPlugin.h"

// iterators:
typedef std::vector<vListener*>::iterator listenerIterator;
typedef std::vector<vBaseNode*>::iterator nodeIterator;
typedef std::vector<vSoundSource*>::iterator sourceIterator;
typedef std::vector<vSoundConn*>::iterator connIterator;


class vAudioManager
{
	
	public:


		// Singleton instance reference:
		static vAudioManager& Instance();

		void debugPrint();

		void setPlugin(vPlugin *p);

		vSoundSource* getOrCreateSoundSource(std::string id);
		vListener* getOrCreateListener(std::string id);

		vBaseNode* getNode(std::string id);
		vSoundSource* getSoundSource(std::string id);
		vListener* getListener(std::string id);

		std::vector<vSoundConn*> getConnections(std::string id);
		vSoundConn* getConnection(std::string src, std::string snk);
		vSoundConn* getConnection(std::string id);
		
		void setConnectFilter (std::string s);

		vSoundConn* connect (std::string src, std::string snk);
		vSoundConn* connect (vBaseNode *src, vBaseNode *snk);

		void disconnect(vSoundConn *conn);

		void update(vBaseNode *n);
		void update(vSoundConn *conn);

	private:
		
		// singleton constructors & desctructor (hidden):
		vAudioManager();
		~vAudioManager();
		vAudioManager(vAudioManager const&); // copy constructor
		// hide the assignment operator, otherwise it would be possible to
		// assign the singleton vAudioManager to itself:
		vAudioManager& operator=(vAudioManager const&);
		
		vPlugin *plugin;

		bool autoConnect;

		std::string connectFilter;
		regex_t connectRegex;

		std::vector<vListener*>  vListenerList;
		std::vector<vSoundSource*> vSoundSourceList;
	 	std::vector<vSoundConn*> vSoundConnList;

};

#endif
