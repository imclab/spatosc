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

#include "oscutils.h"

int main(int /*argc*/, char ** /*argv*/)
{
    using namespace spatosc::OSCutil;

    if (typeTagsMatch(0, "fff"))
        return 1;
    if (typeTagsMatch("fff", 0))
        return 1;
    if (typeTagsMatch("ff", "fff"))
        return 1;
    if (typeTagsMatch("fff", "ff"))
        return 1;
    if (!typeTagsMatch("s", "s"))
        return 1;
    if (!typeTagsMatch("ss", "ss"))
        return 1;
    if (!typeTagsMatch("ssf", "ssf"))
        return 1;

    return 0;
}
