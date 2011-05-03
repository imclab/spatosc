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
#include "vectors.h"
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
#include "geotransform.h"
#include "oscutils.h"

namespace spatosc
{
struct Scene::RegexHandle
{
#ifdef HAVE_REGEX
    regex_t regex;
    bool isMatch(const std::string &str) const
    {
        return regexec(&regex, str.c_str(), (size_t)0, 0, 0) == 0;
    }
    bool setPattern(const std::string &str)
    {
        return regcomp(&regex, str.c_str(), REG_EXTENDED|REG_NOSUB) == 0;
    }
#endif
};
}

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
    transform_(new GeoTransform),
    translators_(),
    autoConnect_(true),
    connectFilter_(),
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
        (*c)->debugPrint();
    }
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

        // send a message to any interested translators
        onSceneChanged("ss", "createSoundSource", id.c_str(), SPATOSC_ARGS_END);
        /*
        std::map<std::string, std::tr1::shared_ptr<Translator> >::iterator iter;
        for (iter = translators_.begin(); iter != translators_.end(); ++iter)
            iter->second->pushSceneChange("createSoundSource", id.c_str());
         */

        if (autoConnect_)
        {
            ListenerIterator iter;
            for (iter = listeners_.begin(); iter != listeners_.end(); ++iter)
                connect(node, iter->get());
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

        // send a message to any interested translators
        onSceneChanged("ss", "createListener", id.c_str(), SPATOSC_ARGS_END);
        /*
        std::map<std::string, std::tr1::shared_ptr<Translator> >::iterator iter;
        for (iter = translators_.begin(); iter != translators_.end(); ++iter)
            iter->second->pushSceneChange("createListener", id.c_str());
        */

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
        if ((*n)->hasID(id))
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
        if ((*L)->hasID(id))
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
        if (((*c)->getSource() == node) || ((*c)->getSink() == node))
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
        if (((*c)->getSource() == source) && ((*c)->getSink() == sink))
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
    if (!connectRegex_->setPattern(s))
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
        std::cerr << "Nodes " << *src << " and " << *snk << " are already connected." << std::endl;
        return 0;
    }
#ifdef HAVE_REGEX
    // Check src and snk id's against the connectFilter. If either match, then
    // proceed with the connection:
    bool srcRegexStatus = connectRegex_->isMatch(src->getID());
    bool snkRegexStatus = connectRegex_->isMatch(snk->getID());
    if (srcRegexStatus || snkRegexStatus)
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
        src->addConnectionTo(conn);
        snk->addConnectionFrom(conn);

        // send a message to any interested translators
        onSceneChanged("sss", "connect", src->getID().c_str(), snk->getID().c_str(), SPATOSC_ARGS_END);
        /*
        std::map<std::string, std::tr1::shared_ptr<Translator> >::iterator iter;
        for (iter = translators_.begin(); iter != translators_.end(); ++iter)
            iter->second->pushSceneChange("connect", src->getID().c_str(), snk->getID().c_str());
*/

        onConnectionChanged(conn.get());
        return conn.get();
    }
    else
        return 0;
}

bool Scene::disconnect(SoundSource *source, Listener *sink)
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
        std::cerr << "Cannot disconnect nodes " << *source << " and " << *sink << ": They are not connected." << std::endl;
        return false;
    }
    source->removeConnectionTo(conn);
    sink->removeConnectionFrom(conn);

    // send a message that it has been deleted:
    onSceneChanged("sss", "disconnect", source->getID().c_str(), sink->getID().c_str(), SPATOSC_ARGS_END);

    return eraseFromVector(connections_, conn);
}

void Scene::onConnectionChanged(Connection *conn, bool forcedNotify)
{
    // If one of the connected nodes has been deactivated, then there is no need
    // to send messages.

	conn->recomputeConnection();
	if (conn->active() || forcedNotify)
    {
		if (synchronous_ || forcedNotify)
			pushConnectionChangesViaAllTranslators(conn, forcedNotify);
    }
}

void Scene::forceRefresh()
{
    // FIXME: should re-create all nodes here.
    // TODO: send createSoundSource, createListener, connect, messages
    // ie, call pushSceneChanged() for all translators
    ConnConstIterator iter;
    for (iter = connections_.begin(); iter != connections_.end(); ++iter)
    {
    	(*iter)->recomputeConnection();
        pushConnectionChangesViaAllTranslators((*iter).get(), true);
    }
}

void Scene::pushConnectionChangesViaAllTranslators(Connection *conn, bool forcedNotify)
{
    SoundSource *src = conn->getSource();
    Listener *sink = conn->getSink();

    if (! src)
    {
        std::cerr << __FUNCTION__ << "This connection does not have a valid source node." << std::endl;
        return;
    }
    if (! sink)
    {
        std::cerr << __FUNCTION__ << "This connection does not have a valid sink node." << std::endl;
        return;
    }
    if (src->shouldSendNewState() || sink->shouldSendNewState() || forcedNotify)
    {
        std::map<std::string, std::tr1::shared_ptr<Translator> >::iterator iter;
        for (iter = translators_.begin(); iter != translators_.end(); ++iter)
            iter->second->pushConnectionChanges(conn);

        // !!! can't do this here, because node may be needed for other
        // connections.
        //src->stateSent();
        //sink->stateSent();
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
        ConnConstIterator c;
        for (c = connections_.begin(); c != connections_.end(); ++c)
        {
            Connection* conn = (*c).get();
            if (conn->active())
                pushConnectionChangesViaAllTranslators(conn);
        }

        // now reset all changed flags on nodes:
        for (ListenerIterator L = listeners_.begin(); L != listeners_.end(); ++L)
            (*L)->stateSent();
        for (SourceIterator n = soundSources_.begin(); n != soundSources_.end(); ++n)
            (*n)->stateSent();

        return true;
    }
}

bool Scene::disconnectNodeConnections(const Node *node)
{
    std::vector<Connection*> nodeConnections = getConnectionsForNode(node);
    std::vector<Connection*>::iterator iter;
    bool did_disconnect_some = false;
    for (iter = nodeConnections.begin(); iter != nodeConnections.end(); ++iter)
    {
        Connection* conn = (*iter);
        if (disconnect(conn->getSource(), conn->getSink()))
            did_disconnect_some = true;
    }
    return did_disconnect_some;
}

bool Scene::deleteNode(const SoundSource *node)
{
    if (! node)
    {
        std::cerr << "Invalid source node." << std::endl;
        return false;
    }

    disconnectNodeConnections(node);
    return eraseFromVector(soundSources_, node);
}

bool Scene::deleteNode(const Listener *node)
{
    if (! node)
    {
        std::cerr << "Invalid listener node." << std::endl;
        return false;
    }

    disconnectNodeConnections(node);
    return eraseFromVector(listeners_, node);
}

namespace
{
template <typename T>
void clearVector(std::vector<T> &vec)
{
    std::vector<T>().swap(vec);
}
} // end of anonymous namespace

void Scene::deleteAllNodes()
{
    // we swap them with emtpy vectors to make sure their size is 0.
    // see http://www.gotw.ca/gotw/054.htm
    clearVector(connections_);
    clearVector(listeners_);
    clearVector(soundSources_);
    //std::vector<std::tr1::shared_ptr<Connection> >().swap(connections_);
    //std::vector<std::tr1::shared_ptr<Listener> >().swap(listeners_);
    //std::vector<std::tr1::shared_ptr<SoundSource> >().swap(soundSources_);

    onSceneChanged("s", "clear", SPATOSC_ARGS_END);
}

void Scene::onTransformChanged()
{
    for (ConnIterator iter = connections_.begin(); iter != connections_.end(); ++iter)
        (*iter)->recomputeConnection();
}
        
void Scene::setTranslation(double x, double y, double z)
{
    transform_->translate(x, y, z);
    onTransformChanged();
}

void Scene::setOrientation(double pitch, double roll, double yaw)
{
    transform_->rotate(pitch, roll, yaw);
    onTransformChanged();
}

void Scene::setOrientation(double x, double y, double z, double w)
{
    transform_->rotate(x, y, z, w);
    onTransformChanged();
}

void Scene::setScale(double x, double y, double z)
{
    transform_->scale(x, y, z);
    onTransformChanged();
}

void Scene::applyTransformation(Vector3 &vec) const
{
    transform_->apply(vec);
}

void Scene::applyTransformation(double &x, double &y, double &z) const
{
    transform_->apply(x, y, z);
}

Translator *Scene::getTranslator(const std::string &name)
{
    if (! hasTranslator(name))
        return 0;
    else
        return translators_.find(name)->second.get();
}

bool Scene::removeTranslator(const std::string &name)
{
    if (hasTranslator(name))
    {
        translators_.erase(name);
        return true;
    }
    else
        return false;
}

bool Scene::hasTranslator(const std::string &name) const
{
    return translators_.find(name) != translators_.end();
}

void Scene::onSceneChanged(const char *types, ...)
{
    va_list ap;
    va_start(ap, types);

    std::map<std::string, std::tr1::shared_ptr<Translator> >::iterator iter;
    for (iter = translators_.begin(); iter != translators_.end(); ++iter)
        iter->second->pushSceneChange(types, ap);
}

} // end namespace spatosc

