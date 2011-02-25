
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

int main(int /*argc*/, char ** /*argv*/)
{
    if (! test_property())
        return 1;
    return 0;
}

