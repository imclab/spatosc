/**
 * This file is part of spatosc.
 *
 * Copyright (c) 2010 Society for Arts and Technology <info@sat.qc.ca>
 *
 * spatosc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * spatosc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with spatosc.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <tr1/memory>

class GUI;
class AudioScene;

class Application 
{
    private:
        std::tr1::shared_ptr<AudioScene> audioScene_;
        std::tr1::shared_ptr<GUI> gui_;

    public:
        /// Initialize clutter and openal systems
        static void initSubsystems();
        Application();
        AudioScene &getAudio() { return *audioScene_; }
        void start();
};

#endif // __APPLICATION_H__
