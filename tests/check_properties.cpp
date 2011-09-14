
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

using namespace spatosc;
/**
 * Tests the Property template class.
 */
bool test_property()
{
    Property<std::string> property("foo", "bar");
    if (property.getValue() != "bar")
    {
        std::cout << __FUNCTION__ << ": its value should be the same as the one given." << std::endl;
        return false;
    }

    if (property.getName() != "foo")
    {
        std::cout << __FUNCTION__ << ": its name should be the same as the one given." << std::endl;
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

/**
 * Tests the Properties template class.
 */
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

class DummyTranslator : public spatosc::Translator
{
    public:
        DummyTranslator() :
            spatosc::Translator()
        {
        }
        virtual void pushPropertyChange(spatosc::Node *node, const std::string &key, const std::string &value)
        {
            UNUSED(node);
            UNUSED(key);
            UNUSED(value);
            string_updated = true;
        }
        virtual void pushPropertyChange(spatosc::Node *node, const std::string &key, const int &value)
        {
            UNUSED(node);
            UNUSED(key);
            UNUSED(value);
            int_updated = true;
        }
        virtual void pushPropertyChange(spatosc::Node *node, const std::string &key, const double &value)
        {
            UNUSED(node);
            UNUSED(key);
            UNUSED(value);
            float_updated = true;
        }
    static bool string_updated;
    static bool float_updated;
    static bool int_updated;
};

bool DummyTranslator::string_updated = false;
bool DummyTranslator::int_updated = false;
bool DummyTranslator::float_updated = false;

bool test_notification()
{
    using namespace spatosc;
    Scene scene;
    scene.addTranslator("dummy", new DummyTranslator());
    SoundSource *source = scene.createSoundSource("source");
    if (DummyTranslator::string_updated || DummyTranslator::float_updated || DummyTranslator::int_updated)
    {
        std::cerr <<" should not have received a property update yet.\n";
        return false;
    }

    // String property:
    source->setStringProperty("foo", "bar");
    if (! DummyTranslator::string_updated)
    {
        std::cerr <<" should have received a string property update.\n";
        return false;
    }
    std::string s_value("");
    source->getStringProperty("foo", s_value);
    if (s_value != "bar")
    {
        std::cerr <<" string property has not been set.\n";
        return false;
    }

    // Float property:
    source->setFloatProperty("bar", 3.14159);
    if (! DummyTranslator::float_updated)
    {
        std::cerr <<" should have received a float property update.\n";
        return false;
    }
    double f_value(0.0);
    source->getFloatProperty("bar", f_value);
    if (f_value != 3.14159)
    {
        std::cerr <<" float property has not been set.\n";
        return false;
    }

    // Int property:
    source->setIntProperty("egg", 2);
    if (! DummyTranslator::int_updated)
    {
        std::cerr <<" should have received an int property update.\n";
        return false;
    }
    int i_value(0);
    source->getIntProperty("egg", i_value);
    if (i_value != 2)
    {
        std::cerr <<" int property has not been set.\n";
        return false;
    }
    source->removeIntProperty("egg");
    i_value = 0;
    source->getIntProperty("egg", i_value);
    if (i_value != 0)
    {
        std::cerr <<" int property has not been removed.\n";
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

