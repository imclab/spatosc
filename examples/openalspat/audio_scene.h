
// AudioScene.h
//
// Copyright (c) 2010 Tristan Matthews <le.businessman@gmail.com>
//
// This file is part of spatosc.
//
// spatosc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// spatosc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with spatosc.  If not, see <http://www.gnu.org/licenses/>.

#ifndef _ALSCENE_H_
#define _ALSCENE_H_

#include <AL/al.h>
#include <AL/alut.h>

class AudioScene {
    private:
        void createSource();
        void createListener();

        ALfloat sourcePos_[3];
        ALuint source_;
    public:
        AudioScene();
        ~AudioScene();
        static void init();
};

#endif // _ALSCENE_H_
