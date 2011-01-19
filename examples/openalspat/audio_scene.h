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
#ifndef _ALSCENE_H_
#define _ALSCENE_H_

#include <AL/al.h>
#include <AL/alut.h>

class AudioScene {
    private:
        void createSource();
        void createListener();
        void updatePosition();

        ALfloat sourcePos_[3];
        ALuint source_;
        double step_;
    public:
        AudioScene();
        ~AudioScene();
        void start();
        /// Initialize openal subsystem
        static void init();
        
        // movement API
        void moveSourceLeft();
        void moveSourceRight();
        void moveSourceUp();
        void moveSourceDown();
        void moveSourceRaise();
        void moveSourceLower();
        void moveSourceToOrigin();
};

#endif // _ALSCENE_H_
