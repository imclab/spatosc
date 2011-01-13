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
	this->vListenerList_.clear();
	this->vSoundSourceList_.clear();
	this->vSoundConnList_.clear();

	// for now, create a basic (CONSOLE) plugin:
	plugin_.reset(new vPlugin());

	autoConnect_ = true;

	setConnectFilter(".*"); // match everything

}

// *****************************************************************************
// destructor
vAudioManager::~vAudioManager()
{
}


vAudioManager& vAudioManager::Instance()
{
	// Meyers' singleton design pattern
    static vAudioManager s;
    return s;
}

// *****************************************************************************
void vAudioManager::setPlugin(const std::tr1::shared_ptr<vPlugin> &p)
{
    if (plugin_ == p)
        return;
	// replace old plugin:
	plugin_ = p;
}

// *****************************************************************************
void vAudioManager::debugPrint ()
{
	listenerIterator L;
	sourceIterator n;
	connIterator c;

	std::cout << "\n=====================================================" << std::endl;

	std::cout << "[vAudioManager]:: connectFilter = " << connectFilter_ << std::endl;

	if (plugin_) std::cout << "[vAudioManager]:: using " << plugin_->getTypeString() << " plugin" << std::endl;
	else std::cout << "[vAudioManager]:: NO plugin specified" << std::endl;

	std::cout << "[vAudioManager]:: " << vListenerList_.size() << " listeners:" << std::endl;
	for (L = vListenerList_.begin(); L != vListenerList_.end(); ++L)
	{
		(*L)->debugPrint();
	}

	std::cout << "[vAudioManager]:: " << vSoundSourceList_.size() << " sources:" << std::endl;
	for (n = vSoundSourceList_.begin(); n != vSoundSourceList_.end(); ++n)
	{
		(*n)->debugPrint();
	}

	std::cout << "[vAudioManager]:: " << vSoundConnList_.size() << " connections:" << std::endl;
	for (c = vSoundConnList_.begin(); c != vSoundConnList_.end(); ++c)
	{
		std::cout << "  " << (*c)->id_ << ":" << std::endl;
		std::cout << "    distanceEffect:\t" << (*c)->distanceEffect_ << "%" << std::endl;
		std::cout << "    rolloffEffect:\t" << (*c)->rolloffEffect_ << "%" << std::endl;
		std::cout << "    dopplerEffect:\t" << (*c)->dopplerEffect_ << "%" << std::endl;
		std::cout << "    diffractionEffect:\t" << (*c)->diffractionEffect_ << "%" << std::endl;
	}

}


// *****************************************************************************
vSoundSource* vAudioManager::getOrCreateSoundSource(const std::string &id)
{
    using std::tr1::shared_ptr;
	// check if it already exists:
	vSoundSource *n = getSoundSource(id);

	if (n == 0)
	{
		// if not, create a new vSoundNode:
        shared_ptr<vSoundSource> tmp(new vSoundSource(id));

		// add it to the vSoundSourceList:
		vSoundSourceList_.push_back(tmp);
        n = tmp.get();

		if (autoConnect_)
		{
			listenerIterator L;
			for (L = vListenerList_.begin(); L != vListenerList_.end(); ++L)
			{
				connect(n, L->get());
			}
		}
	}

	return n;
}

// *****************************************************************************
vListener* vAudioManager::getOrCreateListener(const std::string &id)
{
    using std::tr1::shared_ptr;
	// check if it already exists:
	vListener *L = getListener(id);

	if (!L)
	{
		// if not, create a new vSoundNode:
        shared_ptr<vListener> tmp(new vListener(id));
        L = tmp.get();

		// add it to the vListenerList:
		vListenerList_.push_back(tmp);

		if (autoConnect_)
		{
			sourceIterator n;
			for (n = vSoundSourceList_.begin(); n != vSoundSourceList_.end(); ++n)
			{
				connect(n->get(), L);
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
	if (n) 
        return n;

	n = getListener(id);
	if (n) 
        return n;

	return 0;
}

// *****************************************************************************
// return a pointer to a vSoundNode in the vSoundSourceList, given an id:
vSoundSource* vAudioManager::getSoundSource(const std::string &id)
{
	sourceIterator n;
	for (n = vSoundSourceList_.begin(); n != vSoundSourceList_.end(); ++n)
	{
		if ((*n)->id_ == id)
		{
			return n->get();
		}
	}
	
	return NULL;
}



// *****************************************************************************
// return a pointer to a vListener in the vListenerList, given an id:
vListener* vAudioManager::getListener(const std::string &id)
{
	listenerIterator L;
	for (L = vListenerList_.begin(); L != vListenerList_.end(); ++L)
	{
		if ((*L)->id_ == id)
		{
			return L->get();
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
	for (c = vSoundConnList_.begin(); c != vSoundConnList_.end(); ++c)
	{
		if (((*c)->src_->id_ == id) or ((*c)->snk_->id_ == id))
		{
			foundConnections.push_back(c->get());
		}
	}
	return foundConnections;
}

// *****************************************************************************
// return a pointer to a vSoundConn in the vSoundConnList:
vSoundConn* vAudioManager::getConnection(const std::string &src, const std::string &snk)
{
	connIterator c;
	for (c = vSoundConnList_.begin(); c != vSoundConnList_.end(); ++c)
	{
		if (((*c)->src_->id_ == src) and ((*c)->snk_->id_ == snk))
		{
			return c->get();
		}
	}

	return NULL;
}

vSoundConn* vAudioManager::getConnection(const std::string &id)
{
	connIterator c;
	for (c = vSoundConnList_.begin(); c != vSoundConnList_.end(); ++c)
	{
		if ((*c)->id_ == id)
		{
			return c->get();
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

	if (regcomp(&connectRegex_, s.c_str(), REG_EXTENDED|REG_NOSUB) != 0)
	{
		std::cout << "vAudioManager error: bad regex pattern passed to setConnectFilter(): " << s << std::endl;
	    return;
	}

	connectFilter_ = s;
}

vSoundConn* vAudioManager::connect(const std::string &src, const std::string &snk)
{
	// check if exists first:
	vSoundConn* conn = getConnection(src, snk);
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


vSoundConn* vAudioManager::connect(vBaseNode *src, vBaseNode *snk)
{
    using std::tr1::shared_ptr;
	// if the node pointers are invalid for some reason, return:
	if (!src or !snk) 
        return NULL;

	// Check src and snk id's against the connectFilter. If either match, then
	// proceed with the connection:
	int srcRegexStatus = regexec(&connectRegex_, src->id_.c_str(), (size_t)0, NULL, 0);
	int snkRegexStatus = regexec(&connectRegex_, snk->id_.c_str(), (size_t)0, NULL, 0);

	if (srcRegexStatus == 0 or snkRegexStatus == 0)
	{
		// create connection:
		shared_ptr<vSoundConn> conn(new vSoundConn(src, snk));

		// register the connection with both the vAudioManager and the
		// sink node (for backwards connectivity computation):
		vSoundConnList_.push_back(conn);
		src->connectTO_.push_back(conn);
		snk->connectFROM_.push_back(conn);

		update(conn.get());

		return conn.get();
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
	for (c = n->connectTO_.begin(); c != n->connectTO_.end(); ++c)
	{
		update(c->get());
	}
	for (c = n->connectFROM_.begin(); c != n->connectFROM_.end(); ++c)
	{
		update(c->get());
	}
}

void vAudioManager::update(vSoundConn *conn)
{
	// If one of the connected nodes has been deactivated, then there is no need
	// to compute anything. Enable the mute (and send the status change if this
	// has just happened)
	if ((conn->src_->active_) and (conn->snk_->active_))
	{
		if (plugin_)
        {
            conn->update();
            plugin_->update(conn);
        }
	}
}
