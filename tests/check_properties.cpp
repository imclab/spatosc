
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

using namespace spatosc;

bool test_property()
{
    Property<std::string> property("foo", "bar");
    if (property.getValue() != "bar")
    {
        std::cout << __FUNCTION__ << ": its value should be the same as the one given." << std::endl;
        return false;
    }
    property.setValue("spam");
    if (property.getValue() != "spam")
    {
        std::cout << __FUNCTION__ << ": its value should be the same as the one given." << std::endl;
        return false;
    }
    return true;
}

bool test_properties()
{
    Properties<std::string> properties;
    if (properties.addProperty("foo", "bar")->getValue() != "bar")
    {
        std::cout << __FUNCTION__ << ": its value should be the same as the one given." << std::endl;
        return false;
    }
    if (properties.getProperty("foo")->getValue() != "bar")
    {
        std::cout << __FUNCTION__ << ": its value should be the same as the one given." << std::endl;
        return false;
    }

    properties.setPropertyValue("foo", "spam");
    std::string val;
    properties.getPropertyValue("foo", val);
    if (val != "spam")
    {
        std::cout << __FUNCTION__ << ": could not set the property's value." << std::endl;
        return false;
    }
    std::string value;
    properties.getPropertyValue("foo", value);
    if (value != "spam")
    {
        std::cout << __FUNCTION__ << ": its value should be the same as the one given." << std::endl;
        return false;
    }

    if (! properties.hasProperty("foo"))
    {
        std::cout << __FUNCTION__ << ": could not find a property it should have." << std::endl;
        return false;
    }
    if (properties.hasProperty("I don't exist"))
    {
        std::cout << __FUNCTION__ << ": could find a property it should not have." << std::endl;
        return false;
    }

    if (! properties.getPropertyValue("qwoeiuqowiueoqiweuoqwiue", value))
    {
        std::cout << __FUNCTION__ << ": should have raised an exception when trying to access a property that does not exist.." << std::endl;
        return false;
    }
    return true;
}

#define UNUSED(x) ((void) (x))

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
        virtual void pushPropertyChange(spatosc::Node *node, const std::string &key, const std::string &value)
        {
            UNUSED(node);
            UNUSED(key);
            UNUSED(value);
            updated = true;
        }
    static bool updated;
};

bool DummyTranslator::updated = false;

bool test_notification()
{
    using namespace spatosc;
    Scene scene;
    scene.addTranslator<DummyTranslator>("dummy", "address", "port");
    SoundSource *source = scene.createSoundSource("source");
    if (DummyTranslator::updated)
    {
        std::cerr <<" should not have received a property update yet.\n";
        return false;
    }
    source->setStringProperty("foo", "bar");
    if (! DummyTranslator::updated)
    {
        std::cerr <<" should have received a property update.\n";
        return false;
    }
    return true;
}

int main(int /*argc*/, char ** /*argv*/)
{
    if (! test_property())
        return 1;
    return 0;
}

