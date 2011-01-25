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

#include "facade.h"
#include "scene.h"
#include "node.h"
#include "listener.h"
#include "soundsource.h"
#include <iostream>
#include <tr1/memory>

namespace spatosc
{

Scene* Facade::getScene(const std::string &scene)
{
    SceneIterator iter = scenes_.find(scene);
    if (iter == scenes_.end())
        return 0;
    else
        return iter->second.get();
}

bool Facade::createScene(const std::string &scene)
{
    using std::tr1::shared_ptr;
    Scene *obj = getScene(scene);
    if (obj)
    {
        std::cerr << "There is already such a scene: " << scene << std::endl;
        return false;
    }
    else
    {
        scenes_[scene] = shared_ptr<Scene>(new Scene);
        return true;
    }
}

}

