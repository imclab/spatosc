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

/**
 * Tests that there can only be unique node names.
 */

#include <iostream>
#include <string>
#include "spatosc.h"
#include "unused.h"

class DummyTranslator : public spatosc::Translator
{
    public:
        DummyTranslator(const std::string &ip,
                const std::string &toPort,
                bool verbose = false) :
            spatosc::Translator(verbose)
        {
            UNUSED(ip);
            UNUSED(toPort);
        }
        virtual void pushConnectionChanges(spatosc::Connection *conn)
        {
            UNUSED(conn);
            pushed = true;
        }
    static bool pushed;
};

bool DummyTranslator::pushed = false;

bool test_async()
{
    using namespace spatosc;
    Scene scene;
    scene.addTranslator<DummyTranslator>("dummy", "address", "port");
    scene.setAutoConnect(false);
    scene.setSynchronous(false);
    SoundSource *source = scene.createSoundSource("source");
    Listener *listener = scene.createListener("listener");
    scene.connect(source, listener);

    // In synchronous mode, changing a position results in a connection update.
    // We want it to happen only after flushMessages():

    DummyTranslator::pushed = false;
    source->setPosition(1.0, 2.0, 3.0);

    if (DummyTranslator::pushed)
    {
        std::cerr <<" should not have pushed yet.\n";
        return false;
    }
    scene.flushMessages();
    if (! DummyTranslator::pushed)
    {
        std::cerr <<" should have pushed.\n";
        return false;
    }
    return true;
}

int main(int /*argc*/, char ** /*argv*/)
{
    if (! test_async())
        return 1;
    return 0;
}

