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

#include "scene.h"
#include "config.h"
#ifdef HAVE_REGEX
#include <regex.h>
#endif

#include <iostream>
#include <cassert>
#include <algorithm>

#include "connection.h"
#include "listener.h"
#include "memory.h"
#include "node.h"
#include "soundsource.h"
#include "translator.h"
#include "oscreceiver.h"
#include "spatdif_receiver.h"

namespace spatosc
{
struct Scene::RegexHandle
{
#ifdef HAVE_REGEX
    regex_t regex;
#endif
};
}

namespace
{
    // useful to search and delete in a vector of shared_ptr
    template <typename T>
    class IsEqual
    {
        public:
            IsEqual(T *a) : a_(a) {}
            bool operator() (const std::tr1::shared_ptr<T> &b)
            {
                return a_ == b.get();
            }
        private:
            const T *a_;
    };
    /**
     * Removes an element from a vector of shared pointers if
     * the given pointer matches.
     * @return Whether it deleted some elements of not.
     */
    template <typename T>
    bool eraseFromVector(std::vector<std::tr1::shared_ptr<T> >& vec, T *a)
    {
        size_t size_before = vec.size();
        IsEqual<T> predicate(a);
        vec.erase(std::remove_if(vec.begin(), vec.end(), predicate), vec.end());
        return vec.size() < size_before;
    }
} // end of anonymous namespace

namespace spatosc
{
#if 0
// *****************************************************************************
// This is a function that can be used by std::sort to make a
// list of nodes alphabetical:
static bool nodeSortFunction (Node *n1, Node *n2)
{
    return ( n1->getID() < n2->getID() );
}
#endif

// for now, create a basic (CONSOLE) translator:
Scene::Scene() :
    connectRegex_(new Scene::RegexHandle),
    translator_(new Translator(false)),
    autoConnect_(true),
    connectFilter_(),
    receiver_(0),
    listeners_(),
    soundSources_(),
    connections_(),
    verbose_(false),
    synchronous_(true)
{
    this->listeners_.clear();
    this->soundSources_.clear();
    this->connections_.clear();
    setConnectFilter(".*"); // match everything
}

void Scene::setVerbose(bool verbose)
{
    verbose_ = verbose;
}

void Scene::setSynchronous(bool synchronous)
{
    synchronous_ = synchronous;
}

void Scene::setAutoConnect(bool enabled)
{
    autoConnect_ = enabled;
}

bool Scene::getAutoConnect() const
{
    return autoConnect_;
}

void Scene::debugPrint ()
{
    ListenerIterator L;
    SourceIterator n;
    ConnIterator c;

    std::cout << "\n=====================================================" << std::endl;
    std::cout << "[Scene]:: connectFilter = " << connectFilter_ << std::endl;

    std::cout << "[Scene]:: " << listeners_.size() << " listeners:" << std::endl;
    for (L = listeners_.begin(); L != listeners_.end(); ++L)
    {
        (*L)->debugPrint();
    }

    std::cout << "[Scene]:: " << soundSources_.size() << " sources:" << std::endl;
    for (n = soundSources_.begin(); n != soundSources_.end(); ++n)
    {
        (*n)->debugPrint();
    }

    std::cout << "[Scene]:: " << connections_.size() << " connections:" << std::endl;
    for (c = connections_.begin(); c != connections_.end(); ++c)
    {
        std::cout << "  " << (*c)->id_ << ":" << std::endl;
        std::cout << "    distanceEffect:\t" << (*c)->distanceEffect_ << "%" << std::endl;
        std::cout << "    rolloffEffect:\t" << (*c)->rolloffEffect_ << "%" << std::endl;
        std::cout << "    dopplerEffect:\t" << (*c)->dopplerEffect_ << "%" << std::endl;
        std::cout << "    diffractionEffect:\t" << (*c)->diffractionEffect_ << "%" << std::endl;
    }
}

void Scene::setReceiver(const std::string &port)
{
	// TODO: (mikewoz) It's probably not good to have a public method for this.
	// Ideally, we want the user to set the receiver at the start (right when
	// the Scene is created), and never change it. You can't change a liblo
	// server (socket) once it's created anyway, and you don't want to loose all
	// the registered handlers!
	//
	// The receiver should thus be created in the Scene constructor... but, it's
	// not always needed, and we don't want to occupy a socket for nothing.

	if (receiver_) delete receiver_;
	//receiver_ = new SpatdifReceiver(port, NULL, verbose_);
	receiver_ = new OscReceiver(port);

}


SoundSource* Scene::createSoundSource(const std::string &id)
{
    using std::tr1::shared_ptr;
    // check if it already exists (as a source or listener):
    if (getListener(id) != 0)
    {
        std::cerr << "Cannot create sound source " << id <<
            ", there is already a listener with that id\n";
        return 0;
    }
    SoundSource *node = getSoundSource(id);

    if (node == 0)
    {
        // if not, create a new node:
        shared_ptr<SoundSource> tmp(new SoundSource(id, *this));

        // add it to soundSources_:
        soundSources_.push_back(tmp);
        node = tmp.get();

        if (autoConnect_)
        {
            ListenerIterator iter;
            for (iter = listeners_.begin(); iter != listeners_.end(); ++iter)
            {
                connect(node, iter->get());
            }
        }

        // register a callback for the sound source with the oscReceiver:
        if (receiver_)
        {
        	receiver_->addHandler(NULL,NULL,SpatdifReceiver::onNodeMessage,(void*)node);
        }

        return node;
    }
    else
    {
        std::cerr << "A node named " << id << " of type " << typeid(node).name() << " already exists." << std::endl;
        return 0;
    }
}

Listener* Scene::createListener(const std::string &id)
{
    using std::tr1::shared_ptr;
    // check if it already exists (as a source or listener:
    if (getSoundSource(id) != 0)
    {
        std::cerr << "Cannot create listener " << id <<
            ", there is already a sound source with that id\n";
        return 0;
    }
    Listener *node = getListener(id);

    if (! node)
    {
        // if not, create a new vSoundNode:
        shared_ptr<Listener> tmp(new Listener(id, *this));
        node = tmp.get();

        // add it to the ListenerList:
        listeners_.push_back(tmp);

        if (autoConnect_)
        {
            SourceIterator iter;
            for (iter = soundSources_.begin(); iter != soundSources_.end(); ++iter)
            {
                connect(iter->get(), node);
            }
        }
        return node;
    }
    else
    {
        std::cerr << "A node named " << id << " of type " << typeid(node).name() << " already exists." << std::endl;
        return 0;
    }
}

Node* Scene::getNode(const std::string &id)
{
    Node *n = 0;

    n = getSoundSource(id);
    if (n)
        return n;

    n = getListener(id);
    if (n)
        return n;
    return 0;
}

// *****************************************************************************
// return a pointer to a vSoundNode in the SoundSourceList, given an id:
SoundSource* Scene::getSoundSource(const std::string &id)
{
    SourceIterator n;
    for (n = soundSources_.begin(); n != soundSources_.end(); ++n)
    {
        if ((*n)->id_ == id)
        {
            return n->get();
        }
    }
    return 0;
}

Listener* Scene::getListener(const std::string &id)
{
    ListenerIterator L;
    for (L = listeners_.begin(); L != listeners_.end(); ++L)
    {
        if ((*L)->id_ == id)
        {
            return L->get();
        }
    }
    return 0;
}

std::vector<Connection*> Scene::getConnectionsForNode(const Node *node)
{
    std::vector<Connection*> foundConnections;

    ConnIterator c;
    for (c = connections_.begin(); c != connections_.end(); ++c)
    {
        if (((*c)->src_ == node) || ((*c)->snk_ == node))
        {
            foundConnections.push_back(c->get());
        }
    }
    return foundConnections;
}

Connection* Scene::getConnection(const Node *source, const Node *sink)
{
    ConnIterator c;
    for (c = connections_.begin(); c != connections_.end(); ++c)
    {
        if (((*c)->src_ == source) && ((*c)->snk_ == sink))
        {
            return c->get();
        }
    }
    return 0;
}

bool Scene::setConnectFilter(std::string s)
{
    // we like specifying just one asterisk ( * ), so we need to convert to a
    // regular expression:
    if (s == "*")
        s = ".*";
#ifdef HAVE_REGEX
    // TODO: Fri Jan 14 11:14:11 EST 2011: connectRegex_ should belong to translator
    if (regcomp(&connectRegex_->regex, s.c_str(), REG_EXTENDED|REG_NOSUB) != 0)
    {
        std::cout << "Scene error: bad regex pattern passed to setConnectFilter(): " << s << std::endl;
        return false;
    }
    connectFilter_ = s;
    return true;
#else
    std::cerr << __FUNCTION__ << ": Compiled with no regex support." << std::endl;
    return false;
#endif
}

Connection* Scene::connect(SoundSource *src, Listener *snk)
{
    using std::tr1::shared_ptr;
    // if the node pointers are invalid for some reason, return:
    if (!src || !snk)
        return 0;
    Connection* conn = getConnection(src, snk);
    if (conn)
    {
        std::cerr << "Nodes " << src->getID() << " and " << snk->getID() << " are already connected." << std::endl;
        return 0;
    }
#ifdef HAVE_REGEX
    // Check src and snk id's against the connectFilter. If either match, then
    // proceed with the connection:
    int srcRegexStatus = regexec(&connectRegex_->regex, src->id_.c_str(), (size_t)0, 0, 0);
    int snkRegexStatus = regexec(&connectRegex_->regex, snk->id_.c_str(), (size_t)0, 0, 0);
    if (srcRegexStatus == 0 || snkRegexStatus == 0)
    {
#else
    if (true)
    {
#endif
        // create connection:
        shared_ptr<Connection> conn(new Connection(src, snk));
        // register the connection with both the Scene and the
        // sink node (for backwards connectivity computation):
        connections_.push_back(conn);
        src->connectTO_.push_back(conn);
        snk->connectFROM_.push_back(conn);
        onConnectionChanged(conn.get());
        return conn.get();
    }
    else
        return 0;
}

bool Scene::disconnect(Node *source, Node *sink)
{
    if (! source)
    {
        std::cerr << "Invalid source node." << std::endl;
        return false;
    }
    if  (! sink)
    {
        std::cerr << "Invalid sink node." << std::endl;
        return false;
    }
    Connection* conn = getConnection(source, sink);
    if (! conn)
    {
        std::cerr << "Cannot disconnect nodes " << source->getID() << " and " << sink->getID() << ": They are not connected." << std::endl;
        return false;
    }
    eraseFromVector(source->connectTO_, conn);
    eraseFromVector(sink->connectFROM_, conn);
    return eraseFromVector(connections_, conn);
}

// FIXME: Source node shouldn't have connectFROM_
// FIXME: Sink node shouldn't have connectFROM_
void Scene::onNodeChanged(Node *n)
{
    ConnIterator c;
    for (c = n->connectTO_.begin(); c != n->connectTO_.end(); ++c)
    {
        onConnectionChanged(c->get());
    }
    for (c = n->connectFROM_.begin(); c != n->connectFROM_.end(); ++c)
    {
        onConnectionChanged(c->get());
    }
}

void Scene::onConnectionChanged(Connection *conn)
{
    // If one of the connected nodes has been deactivated, then there is no need
    // to compute anything. Enable the mute (and send the status change if this
    // has just happened)
    if (conn->src_->active_ && conn->snk_->active_)
    {
        assert(translator_);
        conn->recomputeConnection();
        if (synchronous_)
            translator_->pushOSCMessages(conn);
    }
}

bool Scene::flushMessages()
{
    if (synchronous_)
    {
        std::cerr << "Should not call " << __FUNCTION__ << " if in synchronous mode.\n";
        return false;
    }
    else
    {
        ConnConstIterator iter;
        for (iter = connections_.begin(); iter != connections_.end(); ++iter)
        {
            Connection* conn = (*iter).get();
            translator_->pushOSCMessages(conn);
        }
        return true;
    }
}

bool Scene::disconnectNodeConnections(Node *node)
{
    std::vector<Connection*> nodeConnections = getConnectionsForNode(node);
    std::vector<Connection*>::iterator iter;
    bool did_disconnect_some = false;
    for (iter = nodeConnections.begin(); iter != nodeConnections.end(); ++iter)
    {
        Connection* conn = (*iter);
        if (disconnect(&conn->getSource(), &conn->getSink()))
            did_disconnect_some = true;
    }
    return did_disconnect_some;
}

bool Scene::deleteNode(SoundSource *node)
{
    if (! node)
    {
        std::cerr << "Invalid source node." << std::endl;
        return false;
    }
    disconnectNodeConnections(node);
    return eraseFromVector(soundSources_, node);
}

bool Scene::deleteNode(Listener *node)
{
    if (! node)
    {
        std::cerr << "Invalid listener node." << std::endl;
        return false;
    }
    disconnectNodeConnections(node);
    return eraseFromVector(listeners_, node);
}

void Scene::deleteAllNodes()
{
    // we swap them with emtpy vectors to make sure their size is 0.
    // see http://www.gotw.ca/gotw/054.htm
    std::vector<std::tr1::shared_ptr<Connection> >().swap(connections_);
    std::vector<std::tr1::shared_ptr<Listener> >().swap(listeners_);
    std::vector<std::tr1::shared_ptr<SoundSource> >().swap(soundSources_);
}

} // end namespace spatosc

