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
 * The OSC-enabled Scene class.
 */

#ifndef __OSC_SCENE_H__
#define __OSC_SCENE_H__

#include <lo/lo_osc_types.h> // for lo_arg
#include "memory.h"
#include "scene.h"

namespace spatosc
{

class BasicReceiver;

/**
 * Manages the nodes and their connections.
 * It also manages the translator, who allow to render a scene with some audio engine. Can receive osc messages
 * which are then forwarded to nodes.
 */
class OscScene : public Scene
{
    public:
        /**
         * Constructor.
         *
         * Your next steps should be to set the translator and create a listener node.
         *
         * When you create a scene, there is not a single node in the scene and you must create some if you want to do anything.
         * @param receiverPort Optional port to specify if the scene's nodes should receive OSC messages
         */
        OscScene(const std::string &receiverPort);
        
        virtual ~OscScene();

        /**
         * Called when a node should stop receiving OSC messages.
         */
        void unsubscribe(Node *node);

        /**
         * Returns a sound source node in the scene identified by its identifier. Creates it if it does not exist yet.
         *
         * If a node wih this name already exists, it returns a null pointer and prints an error message.
         *
         * Once you created a SoundSource, you should probably want to set its position and orientation.
         *
         * @warning Make sure you call this node's setChannelID method after its creation.
         * @return A SoundSource pointer. Null if a node with this name already exists. Never free this pointer. It will become invalid if this node is deleted.
         */
        virtual SoundSource* createSoundSource(const std::string &id);

        /**
         * Returns a node in the scene identified by its identifier.
         *
         * The default position for any node is (0, 0, 0).
         *
         * If a node wih this name already exists, it returns a null pointer and prints an error message.
         * @return A Listener pointer. Null if a node with this name already exists. Never free this pointer. It will become invalid if this node is deleted.
         */
        virtual Listener* createListener(const std::string &id);

        /**
         * Called to check if any new OSC messages have arrived
         */
        bool poll();

        virtual void handleMessage(const std::string &method, lo_arg ** argv, const char *types);

    private:
        std::tr1::shared_ptr<BasicReceiver> receiver_;
};

} // end namespace spatosc

#endif // __SCENE_H__
