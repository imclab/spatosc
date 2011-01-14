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
#include <iostream>

#include "translator.h"
#include "listener.h"
#include "node.h"
#include "soundsource.h"
#include "connection.h"

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


// *****************************************************************************
// constructor
Scene::Scene ()
{
    this->ListenerList_.clear();
    this->SoundSourceList_.clear();
    this->ConnectionList_.clear();

    // for now, create a basic (CONSOLE) translator:
    translator_.reset(new Translator());
    autoConnect_ = true;
    setConnectFilter(".*"); // match everything
}

// *****************************************************************************
void Scene::setTranslator(const std::tr1::shared_ptr<Translator> &p)
{
    if (translator_ == p)
        return;
    // replace old translator:
    translator_ = p;
}

// *****************************************************************************
void Scene::debugPrint ()
{
    listenerIterator L;
    sourceIterator n;
    connIterator c;

    std::cout << "\n=====================================================" << std::endl;
    std::cout << "[Scene]:: connectFilter = " << connectFilter_ << std::endl;

    if (translator_)
        std::cout << "[Scene]:: using " << translator_->getTypeString() << " translator" << std::endl;
    else std::cout << "[Scene]:: NO translator specified" << std::endl;

    std::cout << "[Scene]:: " << ListenerList_.size() << " listeners:" << std::endl;
    for (L = ListenerList_.begin(); L != ListenerList_.end(); ++L)
    {
        (*L)->debugPrint();
    }

    std::cout << "[Scene]:: " << SoundSourceList_.size() << " sources:" << std::endl;
    for (n = SoundSourceList_.begin(); n != SoundSourceList_.end(); ++n)
    {
        (*n)->debugPrint();
    }

    std::cout << "[Scene]:: " << ConnectionList_.size() << " connections:" << std::endl;
    for (c = ConnectionList_.begin(); c != ConnectionList_.end(); ++c)
    {
        std::cout << "  " << (*c)->id_ << ":" << std::endl;
        std::cout << "    distanceEffect:\t" << (*c)->distanceEffect_ << "%" << std::endl;
        std::cout << "    rolloffEffect:\t" << (*c)->rolloffEffect_ << "%" << std::endl;
        std::cout << "    dopplerEffect:\t" << (*c)->dopplerEffect_ << "%" << std::endl;
        std::cout << "    diffractionEffect:\t" << (*c)->diffractionEffect_ << "%" << std::endl;
    }
}


// *****************************************************************************
SoundSource* Scene::getOrCreateSoundSource(const std::string &id)
{
    using std::tr1::shared_ptr;
    // check if it already exists:
    SoundSource *n = getSoundSource(id);

    if (n == 0)
    {
        // if not, create a new vSoundNode:
        shared_ptr<SoundSource> tmp(new SoundSource(id, *this));

        // add it to the SoundSourceList:
        SoundSourceList_.push_back(tmp);
        n = tmp.get();

        if (autoConnect_)
        {
            listenerIterator L;
            for (L = ListenerList_.begin(); L != ListenerList_.end(); ++L)
            {
                connect(n, L->get());
            }
        }
    }

    return n;
}

// *****************************************************************************
Listener* Scene::getOrCreateListener(const std::string &id)
{
    using std::tr1::shared_ptr;
    // check if it already exists:
    Listener *L = getListener(id);

    if (!L)
    {
        // if not, create a new vSoundNode:
        shared_ptr<Listener> tmp(new Listener(id, *this));
        L = tmp.get();

        // add it to the ListenerList:
        ListenerList_.push_back(tmp);

        if (autoConnect_)
        {
            sourceIterator n;
            for (n = SoundSourceList_.begin(); n != SoundSourceList_.end(); ++n)
            {
                connect(n->get(), L);
            }
        }
    }

    return L;
}

// *****************************************************************************
// return a Node reference by looking through all storage vectors:
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
    sourceIterator n;
    for (n = SoundSourceList_.begin(); n != SoundSourceList_.end(); ++n)
    {
        if ((*n)->id_ == id)
        {
            return n->get();
        }
    }
    return NULL;
}

// *****************************************************************************
// return a pointer to a Listener in the ListenerList, given an id:
Listener* Scene::getListener(const std::string &id)
{
    listenerIterator L;
    for (L = ListenerList_.begin(); L != ListenerList_.end(); ++L)
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
std::vector<Connection*> Scene::getConnections(const std::string &id)
{
    std::vector<Connection*> foundConnections;
    
    connIterator c;
    for (c = ConnectionList_.begin(); c != ConnectionList_.end(); ++c)
    {
        if (((*c)->src_->id_ == id) or ((*c)->snk_->id_ == id))
        {
            foundConnections.push_back(c->get());
        }
    }
    return foundConnections;
}

// *****************************************************************************
// return a pointer to a Connection in the ConnectionList:
Connection* Scene::getConnection(const std::string &src, const std::string &snk)
{
    connIterator c;
    for (c = ConnectionList_.begin(); c != ConnectionList_.end(); ++c)
    {
        if (((*c)->src_->id_ == src) and ((*c)->snk_->id_ == snk))
        {
            return c->get();
        }
    }
    return NULL;
}

Connection* Scene::getConnection(const std::string &id)
{
    connIterator c;
    for (c = ConnectionList_.begin(); c != ConnectionList_.end(); ++c)
    {
        if ((*c)->id_ == id)
        {
            return c->get();
        }
    }
    return NULL;
}

// *****************************************************************************

void Scene::setConnectFilter(std::string s)
{
    // we like specifying just one asterisk ( * ), so we need to convert to a
    // regular expression:
    if (s=="*")
        s = ".*";

    // TODO: Fri Jan 14 11:14:11 EST 2011: connectRegex_ should belong to translator
    if (regcomp(&connectRegex_, s.c_str(), REG_EXTENDED|REG_NOSUB) != 0)
    {
        std::cout << "Scene error: bad regex pattern passed to setConnectFilter(): " << s << std::endl;
        return;
    }

    connectFilter_ = s;
}

Connection* Scene::connect(const std::string &src, const std::string &snk)
{
    // check if exists first:
    Connection* conn = getConnection(src, snk);
    if (!conn)
    {
        // check if both nodes exist
        Node *srcNode = getNode(src);
        Listener *listener = getListener(snk);
        Node *snkNode;

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

Connection* Scene::connect(Node *src, Node *snk)
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
        shared_ptr<Connection> conn(new Connection(src, snk));

        // register the connection with both the Scene and the
        // sink node (for backwards connectivity computation):
        ConnectionList_.push_back(conn);
        src->connectTO_.push_back(conn);
        snk->connectFROM_.push_back(conn);

        update(conn.get());

        return conn.get();
    }

    else return NULL;
}

void Scene::disconnect(Connection * /*conn*/)
{
    std::cout << "Scene::disconnect NOT IMPLEMENTED YET" << std::endl;
}

void Scene::update(Node *n)
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

void Scene::update(Connection *conn)
{
    // If one of the connected nodes has been deactivated, then there is no need
    // to compute anything. Enable the mute (and send the status change if this
    // has just happened)
    if ((conn->src_->active_) and (conn->snk_->active_))
    {
        if (translator_)
        {
            conn->update();
            translator_->update(conn);
        }
    }
}

} // end namespace spatosc

