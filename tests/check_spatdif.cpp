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
#include "spatdif_receiver.h"
#include "translator.h"
#include "oscsender.h"

class DummyAudioSpatializer : public spatosc::SpatdifHandler
{
    private:
        spatosc::SpatdifReceiver receiver_;
        std::map<std::string, bool> handled_;

        virtual void xyz(const std::string &id, float x, float y, float z)
        {
            std::cout << id << ":" << x << "," << y << "," << z << std::endl;
            handled_[__FUNCTION__] = true;
        }

        virtual void aed(const std::string &id, float azimuth, float elevation, float distanceMeters)
        {
            std::cout << id << ":" << azimuth << "," << elevation << "," << distanceMeters << std::endl;
            handled_[__FUNCTION__] = true;
        }
        virtual void xy(const std::string &id, float x, float y)
        {
            std::cout << id << ":" << x << "," << y << "," << std::endl;
            handled_[__FUNCTION__] = true;
        }
        virtual void delay(const std::string &id, float delay)
        {
            std::cout << id << ":" << delay << std::endl;
            handled_[__FUNCTION__] = true;
        }
        virtual void gainDB(const std::string &id, float gainDB)
        {
            std::cout << id << ":" << gainDB << std::endl;
            handled_[__FUNCTION__] = true;
        }
        virtual void gain(const std::string &id, float gain)
        {
            std::cout << id << ":" << gain << std::endl;
            handled_[__FUNCTION__] = true;
        }
        virtual void spread(const std::string &id, float spread)
        {
            std::cout << id << ":" << spread << std::endl;
            handled_[__FUNCTION__] = true;
        }
        virtual void spreadAE(const std::string &id, float azimSpread, float elevSpread)
        {
            std::cout << id << ":" << azimSpread << "," << elevSpread << std::endl;
            handled_[__FUNCTION__] = true;
        }
    public:
        DummyAudioSpatializer() : receiver_(spatosc::Translator::DEFAULT_SEND_PORT, this)
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
            return handled_.size() == 8;
        }
};

int main()
{
    DummyAudioSpatializer spat;
    spatosc::OscSender sender("127.0.0.1", spatosc::Translator::DEFAULT_SEND_PORT);
    std::string prefix("/spatosc/core/my_node/");
    sender.sendMessage(prefix + "xyz", "fff", 1.4, 2.4, 3.4, SPATOSC_ARGS_END);
    spat.poll();
    sender.sendMessage(prefix + "aed", "fff", 3.4, 2.4, 1.4, SPATOSC_ARGS_END);
    spat.poll();
    sender.sendMessage(prefix + "xy", "ff", 1.2, 2.2, SPATOSC_ARGS_END);
    spat.poll();
    sender.sendMessage(prefix + "delay", "f", 1.4, SPATOSC_ARGS_END);
    spat.poll();
    sender.sendMessage(prefix + "gain", "f", 1.1, SPATOSC_ARGS_END);
    spat.poll();
    sender.sendMessage(prefix + "gainDB", "f", 1.1, SPATOSC_ARGS_END);
    spat.poll();
    sender.sendMessage(prefix + "spread", "f", 1.1, SPATOSC_ARGS_END);
    spat.poll();
    sender.sendMessage(prefix + "spreadAE", "ff", 1.1, 2.2, SPATOSC_ARGS_END);
    spat.poll();

    if (not spat.allMessagesHandled())
        return 1;
    return 0;
}
