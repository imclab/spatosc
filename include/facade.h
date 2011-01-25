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
 * The Facade class. 
 */

#ifndef __FACADE_H__
#define __FACADE_H__

#include <string>
#include <map>
#include <iostream>
#include <tr1/memory>

namespace spatosc
{

class Scene;

/**
 * Wraps the whole spatosc library.
 * @warning Most of it is not implemented. Will be renamed to something like Manager, World or so.
 */
class Facade
{
    public:
        /** @warning Not implemented. */
        bool connect(const std::string &scene, const std::string &from, const std::string &to);
        /** @warning Not implemented. */
        bool createListener(const std::string &scene, const std::string &node);
        /** @warning Not implemented. */
        bool createScene(const std::string &scene);
        /** @warning Not implemented. */
        bool createSource(const std::string &scene, const std::string &node);
        /** @warning Not implemented. */
        bool deleteNode(const std::string &scene, const std::string &node);
        /** @warning Not implemented. */
        bool deleteScene(const std::string &scene);
        /** @warning Not implemented. */
        bool disconnect(const std::string &scene, const std::string &from, const std::string &to);
        /** @warning Not implemented. */
        bool setAutoConnect(const std::string &scene, bool enabled);
        /** @warning Not implemented. We might add port arg. */
        bool setTranslator(const std::string &scene, const std::string &translator, const std::string &host);
        /** @warning Not implemented. */
        bool setConnectFilter(const std::string &scene, const std::string &filterRegex);
        /** @warning Not implemented. */
        bool setOrientation(const std::string &scene, const std::string &node, double pitch, double roll, double yaw);
        /** @warning Not implemented. */
        bool setPosition(const std::string &scene, const std::string &node, double x, double y, double z);
        /** @warning Not implemented. */
        bool setSourceChannel(const std::string &scene, const std::string &sound_source, const std::string &channel);
        /** @warning Not implemented. */
        bool setSourceGain(const std::string &scene, const std::string &sound_source, double linearGain);
        /** @warning Not implemented. */
        bool setTranslator(const std::string &scene, const std::string &translator);
    private:
        typedef std::map<std::string, std::tr1::shared_ptr<Scene> >::iterator SceneIterator;
        Scene *getScene(const std::string &scene);
        std::map<std::string, std::tr1::shared_ptr<Scene> > scenes_;
};

} // end of namespace spatosc

#endif
