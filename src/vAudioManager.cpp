#include "vAudioManager.h"
#include <iostream>

#include "vPlugin.h"
#include "vListener.h"
#include "vBaseNode.h"
#include "vSoundSource.h"
#include "vSoundConn.h"

#if 0
// *****************************************************************************
// This is a function that can be used by std::sort to make a
// list of nodes alphabetical:
static bool nodeSortFunction (vBaseNode *n1, vBaseNode *n2)
{
	return ( n1->getID() < n2->getID() );
}
#endif


// *****************************************************************************
// constructor
vAudioManager::vAudioManager ()
{
	this->vListenerList.clear();
	this->vSoundSourceList.clear();
	this->vSoundConnList.clear();

	// for now, create a basic (CONSOLE) plugin:
	plugin = new vPlugin();

	autoConnect = true;

	setConnectFilter(".*"); // match everything

}

// *****************************************************************************
// destructor
vAudioManager::~vAudioManager()
{
	listenerIterator L = vListenerList.begin();
	while (L != vListenerList.end())
	{
		delete (*L);
		vListenerList.erase(L);
	}
	sourceIterator N = vSoundSourceList.begin();
	while (N != vSoundSourceList.end())
	{
		delete (*N);
		vSoundSourceList.erase(N);
	}
	connIterator C = vSoundConnList.begin();
	while (C != vSoundConnList.end())
	{
		delete (*C);
		vSoundConnList.erase(C);
	}
	if (plugin) delete plugin;
}


vAudioManager& vAudioManager::Instance()
{
	// Meyers' singleton design pattern
    static vAudioManager s;
    return s;
}

// *****************************************************************************
void vAudioManager::setPlugin(vPlugin *p)
{
    if (plugin == p)
        return;
	// clean up old plugin:
	if (plugin) delete plugin;

	plugin = p;
}

// *****************************************************************************
void vAudioManager::debugPrint ()
{
	listenerIterator L;
	sourceIterator n;
	connIterator c;

	std::cout << "\n=====================================================" << std::endl;

	std::cout << "[vAudioManager]:: connectFilter = " << connectFilter << std::endl;

	if (plugin) std::cout << "[vAudioManager]:: using " << plugin->getTypeString() << " plugin" << std::endl;
	else std::cout << "[vAudioManager]:: NO plugin specified" << std::endl;

	std::cout << "[vAudioManager]:: " << vListenerList.size() << " listeners:" << std::endl;
	for (L = vListenerList.begin(); L != vListenerList.end(); L++)
	{
		(*L)->debugPrint();
	}

	std::cout << "[vAudioManager]:: " << vSoundSourceList.size() << " sources:" << std::endl;
	for (n = vSoundSourceList.begin(); n != vSoundSourceList.end() ; n++)
	{
		(*n)->debugPrint();
	}

	std::cout << "[vAudioManager]:: " << vSoundConnList.size() << " connections:" << std::endl;
	for (c = vSoundConnList.begin(); c != vSoundConnList.end(); c++)
	{
		std::cout << "  " << (*c)->id << ":" << std::endl;
		std::cout << "    distanceEffect:\t" << (*c)->distanceEffect << "%" << std::endl;
		std::cout << "    rolloffEffect:\t" << (*c)->rolloffEffect << "%" << std::endl;
		std::cout << "    dopplerEffect:\t" << (*c)->dopplerEffect << "%" << std::endl;
		std::cout << "    diffractionEffect:\t" << (*c)->diffractionEffect << "%" << std::endl;
	}

}


// *****************************************************************************
vSoundSource* vAudioManager::getOrCreateSoundSource(const std::string &id)
{
	// check if it already exists:
	vSoundSource *n = getSoundSource(id);

	if (!n)
	{
		// if not, create a new vSoundNode:
		n = new vSoundSource(id);

		// add it to the vSoundSourceList:
		vSoundSourceList.push_back(n);

		if (autoConnect)
		{
			listenerIterator L;
			for (L = vListenerList.begin(); L != vListenerList.end(); L++)
			{
				connect(n,(*L));
			}
		}
	}

	return n;
}

// *****************************************************************************
vListener* vAudioManager::getOrCreateListener(const std::string &id)
{
	// check if it already exists:
	vListener *L = getListener(id);

	if (!L)
	{
		// if not, create a new vSoundNode:
		L = new vListener(id);

		// add it to the vListenerList:
		vListenerList.push_back(L);

		if (autoConnect)
		{
			sourceIterator n;
			for (n = vSoundSourceList.begin(); n != vSoundSourceList.end(); n++)
			{
				connect((*n),L);
			}
		}
	}

	return L;
}

// *****************************************************************************
// return a vBaseNode reference by looking through all storage vectors:
vBaseNode* vAudioManager::getNode(const std::string &id)
{
	vBaseNode *n = 0;

	n = getSoundSource(id);
	if (n) return n;

	n = getListener(id);
	if (n) return n;

	return NULL;
}


// *****************************************************************************
// return a pointer to a vSoundNode in the vSoundSourceList, given an id:
vSoundSource* vAudioManager::getSoundSource(const std::string &id)
{
	sourceIterator n;
	for (n = vSoundSourceList.begin(); n != vSoundSourceList.end(); n++)
	{
		if ((*n)->id == id)
		{
			return (*n);
		}
	}
	
	return NULL;
}



// *****************************************************************************
// return a pointer to a vListener in the vListenerList, given an id:
vListener* vAudioManager::getListener(const std::string &id)
{
	listenerIterator L;
	for (L = vListenerList.begin(); L != vListenerList.end(); L++)
	{
		if ((*L)->id == id)
		{
			return (*L);
		}
	}

	return NULL;
}

// *****************************************************************************
// return a list of all connections that "directly involve" a node (ie, as the
// source or the sink):
std::vector<vSoundConn*> vAudioManager::getConnections(const std::string &id)
{
	std::vector<vSoundConn*> foundConnections;
	
	connIterator c;
	for (c = vSoundConnList.begin(); c != vSoundConnList.end(); c++)
	{
		if (((*c)->src->id == id) || ((*c)->snk->id == id))
		{
			foundConnections.push_back(*c);
		}
	}
	return foundConnections;
}

// *****************************************************************************
// return a pointer to a vSoundConn in the vSoundConnList:
vSoundConn* vAudioManager::getConnection(const std::string &src, const std::string &snk)
{
	connIterator c;
	for (c = vSoundConnList.begin(); c != vSoundConnList.end(); c++)
	{
		if (((*c)->src->id == src) && ((*c)->snk->id == snk))
		{
			return (*c);
		}
	}

	return NULL;
}

vSoundConn* vAudioManager::getConnection(const std::string &id)
{
	connIterator c;
	for (c = vSoundConnList.begin(); c != vSoundConnList.end(); c++)
	{
		if ((*c)->id == id)
		{
			return (*c);
		}
	}
	
	return NULL;
}



// *****************************************************************************


void vAudioManager::setConnectFilter(std::string s)
{
	// we like specifying just one asterisk ( * ), so we need to convert to a
	// regular expression:
	if (s=="*")
        s = ".*";

	if (regcomp(&connectRegex, s.c_str(), REG_EXTENDED|REG_NOSUB) != 0)
	{
		std::cout << "vAudioManager error: bad regex pattern passed to setConnectFilter(): " << s << std::endl;
	    return;
	}

	connectFilter = s;
}

vSoundConn* vAudioManager::connect(const std::string &src, const std::string &snk)
{
	// check if exists first:
	vSoundConn* conn = getConnection(src,snk);
	if (!conn)
	{
		// check if both nodes exist
		vBaseNode *srcNode = getNode(src);
		vListener *listener = getListener(snk);
		vBaseNode *snkNode;

		bool isNormalConnection = true;
		if (listener)
		{
			snkNode = listener;
			isNormalConnection = false;
		}
		else snkNode = getNode(snk);

		conn = connect(srcNode, snkNode);
	}

	return conn;
}


vSoundConn* vAudioManager::connect (vBaseNode *src, vBaseNode *snk)
{
	// if the node pointers are invalid for some reason, return:
	if (!src || !snk) return NULL;


	// Check src and snk id's against the connectFilter. If either match, then
	// proceed with the connection:
	int srcRegexStatus = regexec(&connectRegex, src->id.c_str(), (size_t)0, NULL, 0);
	int snkRegexStatus = regexec(&connectRegex, snk->id.c_str(), (size_t)0, NULL, 0);

	if ((srcRegexStatus==0) || (snkRegexStatus==0))
	{
		// create connection:
		vSoundConn *conn = new vSoundConn(src, snk);

		// register the connection with both the vAudioManager and the
		// sink node (for backwards connectivity computation):
		vSoundConnList.push_back(conn);
		src->connectTO.push_back(conn);
		snk->connectFROM.push_back(conn);

		update(conn);

		return conn;
	}

	else return NULL;
}

void vAudioManager::disconnect(vSoundConn * /*conn*/)
{
	std::cout << "vAudioManager::disconnect NOT IMPLEMENTED YET" << std::endl;
}

void vAudioManager::update(vBaseNode *n)
{
	connIterator c;
	for (c = n->connectTO.begin(); c != n->connectTO.end(); c++)
	{
		update(*c);
	}
	for (c = n->connectFROM.begin(); c != n->connectFROM.end(); c++)
	{
		update(*c);
	}
}

void vAudioManager::update(vSoundConn *conn)
{
	// If one of the connected nodes has been deactivated, then there is no need
	// to compute anything. Enable the mute (and send the status change if this
	// has just happened)
	if ((conn->src->active) && (conn->snk->active))
	{
		if (plugin) plugin->update(conn);
	}
}
