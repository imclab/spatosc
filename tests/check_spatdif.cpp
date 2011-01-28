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
 * This test checks a client using SpatdifReceiver
 *
 * To run it though gdb:
 * $ libtool --mode=execute gdb check_spatdif
 */

#include <iostream>
#include <map>
#include <string>
#include "scene.h"
#include "spatdif_receiver.h"
#include "soundsource.h"
#include "translator.h"
#include "oscsender.h"

#if 0
namespace spatosc {

class DummyNode : public SoundSource
{
    public:
        DummyNode(Scene &scene) : Node("dummy_node", scene), handled_(0)
        {
        }

        void poll()
        {
            receiver_.poll();
        }

        bool allMessagesHandled() const
        {
            // if the size of handled size is 8 (the number of handler methods), we know
            // that all of them have been called
            return handled_ == 1;
        }
    private:
        int handled_;

        virtual void setPosition(const std::string &id, float x, float y, float z)
        {
            std::cout << id << ":" << x << "," << y << "," << z << std::endl;
            handled_++;
        }
};
}

#endif
int main()
{
#if 0
    spatosc::Scene scene;
    spatosc::DummyNode *node(scene.createSoundSource("dummy_node"));
    spatosc::OscSender sender("127.0.0.1", spatosc::Translator::DEFAULT_SEND_PORT);
    std::string prefix("/spatosc/core/dummy_node/");
    sender.sendMessage(prefix + "xyz", "fff", 1.4, 2.4, 3.4, SPATOSC_ARGS_END);
    node.poll();

    if (not node.allMessagesHandled())
        return 1;
#endif
    return 0;
}
