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
#ifndef _AudioScene_H_
#define _AudioScene_H_

#include <AL/al.h>
#include <AL/alut.h>
#include <tr1/memory>
#include <glib/gtypes.h>
#include "lo/lo.h"

#include <spatosc/spatdif_receiver.h>


class AudioScene;
namespace spatosc {
    class SpatdifReceiver;
}
    
class MyHandler : public spatosc::SpatdifHandler 
{
    private:
        AudioScene *owner_;
        virtual void xyz(const std::string &id, float x, float y, float z);
    public:
        MyHandler(AudioScene *owner);

};

class AudioScene {
    private:
        friend class MyHandler;
        void createSource();
        void createListener();
        void updatePosition();
        void bindCallbacks();
        void updateSourcePosition();
        void updateListenerPosition();
        static gboolean pollReceiver(gpointer data);

        ALfloat sourcePos_[3];
        ALuint source_;
        ALfloat listenerPos_[3];
        ALuint listener_;
        std::tr1::shared_ptr<MyHandler> handler_;
        std::tr1::shared_ptr<spatosc::SpatdifReceiver> receiver_;
        static const char* RX_PORT;

    public:
        AudioScene();
        ~AudioScene();
        void start();
        /// Initialize openal subsystem
        static void init();
};

#endif // _AUDIO_SCENE_H_
