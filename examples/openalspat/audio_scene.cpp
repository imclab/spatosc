
// AudioScene.cpp
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

#include "audio_scene.h"
#include <stdexcept>
#include <iostream>

void AudioScene::init()
{
    alutInit(0, 0);
    alGetError(); // clear the error bit
}

AudioScene::AudioScene() : step_(0.5)
{
    createSource();
    createListener();
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
    // position of listener
    ALfloat listenerPos[] = {0.0, 0.0, 0.0};
    // listener velocity
    ALfloat listenerVel[] = {0.0, 0.0, 0.0};
    // orientation of the listener (first 3 elements are "at", second 3 are "up"
    ALfloat listenerOri[] = {0.0, 0.0, -1.0, 0.0, 1.0, 0.0};
    
    if (alGetError() != AL_NO_ERROR)
        throw(std::runtime_error("OpenAL error")); 
    
    // set listener values
    alListenerfv(AL_POSITION, listenerPos);
    alListenerfv(AL_VELOCITY, listenerVel);
    alListenerfv(AL_ORIENTATION, listenerOri);
}

AudioScene::~AudioScene()
{
    alDeleteSources(1, &source_);
    alutExit();
}

void AudioScene::updatePosition()
{
    alSourcefv(source_, AL_POSITION,
            sourcePos_);
}

void AudioScene::moveSourceRaise()
{
    sourcePos_[2] += step_;
    updatePosition();
}

void AudioScene::moveSourceLower()
{
    sourcePos_[2] -= step_;
    updatePosition();
}

void AudioScene::moveSourceUp()
{
    std::cout << "UP\n";
    sourcePos_[1] += step_;
    updatePosition();
}

void AudioScene::moveSourceDown()
{
    std::cout << "DOWN\n";
    sourcePos_[1] -= step_;
    updatePosition();
}

void AudioScene::moveSourceLeft()
{
    std::cout << "LEFT\n";
    sourcePos_[0] -= step_;
    updatePosition();
}

void AudioScene::moveSourceRight()
{
    std::cout << "RIGHT\n";
    sourcePos_[0] += step_;
    updatePosition();
}

void AudioScene::moveSourceToOrigin()
{
    for (int i = 0; i != 3; ++i)
        sourcePos_[i] = 0.0;
    updatePosition();
}
