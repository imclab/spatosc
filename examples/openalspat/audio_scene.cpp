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

#include "audio_scene.h"
#include <stdexcept>
#include <iostream>
#include <spatosc/translator.h>
#include <spatosc/spatdif_receiver.h>
#include <glib/gmain.h> // for gtimeout

void AudioScene::init()
{
    alutInit(0, 0);
    alGetError(); // clear the error bit
}

MyHandler::MyHandler(AudioScene *owner) : owner_(owner)
{
}

void MyHandler::xyz(const std::string &id, float x, float y, float z)
{
    if (id == "source1")
    {
        owner_->sourcePos_[0] = x;
        owner_->sourcePos_[1] = y;
        owner_->sourcePos_[2] = z;
        owner_->updateSourcePosition();
    }
    else if (id == "listener")
    {
        owner_->listenerPos_[0] = x;
        owner_->listenerPos_[1] = y;
        owner_->listenerPos_[2] = z;
        owner_->updateListenerPosition();
    }
    else
        std::cerr << "Unknown id " << id << std::endl;
}


#if 0
int AudioScene::onListenerPositionChanged(const char *path, const char *types,
        lo_arg ** argv, int argc, void *data, void *user_data)
{
    //std::cout << __FUNCTION__ << std::endl;
    AudioScene *context = static_cast<AudioScene*>(user_data);
    for (int i = 0; i != 3; ++i)
        context->listenerPos_[i] = argv[i]->f;
    context->updateListenerPosition();
    return 0;
}
#endif

gboolean AudioScene::pollReceiver(gpointer data)
{
    AudioScene* context = static_cast<AudioScene*>(data);
    context->receiver_->poll();

    return TRUE;
}

void AudioScene::bindCallbacks()
{
    // add a timeout to poll our oscreceiver
    g_timeout_add(5 /*ms*/, pollReceiver, this);
}

// receive messages from the spatosc plugin
AudioScene::AudioScene() : 
    handler_(new MyHandler(this)), 
    receiver_(new spatosc::SpatdifReceiver(spatosc::Translator::DEFAULT_SEND_PORT, handler_.get()))
{
    createSource();
    createListener();
    bindCallbacks();
}

void AudioScene::start()
{
    // begin the source playing
    alSourcePlay(source_);
}

void AudioScene::createSource()
{
    // position of source sound
    for (int i = 0; i != 3; ++i)
        sourcePos_[i] = 0.0;
    // velocity of source sound
    ALfloat sourceVel[] = {0.0, 0.0, 0.0};
    // if argv[1] is present, it's a filename
    ALuint buffer = alutCreateBufferFromFile("/usr/share/sounds/alsa/Noise.wav");

    // sources are points emitting sound

    alGenSources(1, &source_);
    alSourcei(source_, AL_BUFFER, buffer);
    alSourcef(source_, AL_PITCH, 1.0f);
    alSourcef(source_, AL_GAIN, 1.0f);
    alSourcefv(source_, AL_POSITION, sourcePos_);
    alSourcefv(source_, AL_VELOCITY, sourceVel);
    alSourcei(source_, AL_LOOPING, AL_TRUE);

    if (alGetError() != AL_NO_ERROR)
        throw(std::runtime_error("OpenAL error")); 
}

void AudioScene::createListener()
{
    for (int i = 0; i != 3; ++i)
        listenerPos_[i] = 0.0;
    // listener velocity
    ALfloat listenerVel[] = {0.0, 0.0, 0.0};
    // orientation of the listener (first 3 elements are "at", second 3 are "up"
    ALfloat listenerOri[] = {0.0, 0.0, -1.0, 0.0, 1.0, 0.0};

    if (alGetError() != AL_NO_ERROR)
        throw(std::runtime_error("OpenAL error")); 

    // set listener values
    alListenerfv(AL_POSITION, listenerPos_);
    alListenerfv(AL_VELOCITY, listenerVel);
    alListenerfv(AL_ORIENTATION, listenerOri);
}

AudioScene::~AudioScene()
{
    alDeleteSources(1, &source_);
    alutExit();
}

void AudioScene::updateSourcePosition()
{
    alSourcefv(source_, AL_POSITION, sourcePos_);
}

void AudioScene::updateListenerPosition()
{
    alSourcefv(listener_, AL_POSITION, listenerPos_);
}
