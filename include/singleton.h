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
 * The singleton class.
 */

#include "scene.h"

namespace spatosc
{

class SingletonScene : public Scene
{
public:
    /**
     * Meyers Singleton design pattern
     */
    static SingletonScene& Instance();

private:
    // singleton constructors & desctructor (hidden):
    SingletonScene();
    SingletonScene(SingletonScene const&); // copy constructor
    // hide the assignment operator, otherwise it would be possible to
    // assign the singleton spinApp to itself:
    SingletonScene& operator=(SingletonScene const&);
    ~SingletonScene();
};








} // end namespace
