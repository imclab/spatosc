/*
 * spatdif_receiver.cpp
 *
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

#include "spatdif_receiver.h"
#include "oscreceiver.h"
#include <lo/lo.h>
#include <string>
#include <iostream>

#define UNUSED(x) ((void) (x))

namespace spatosc
{

SpatdifReceiver::SpatdifReceiver(const std::string &port) :
    receiver_(new OscReceiver(port.c_str())),
    verbose_(true)
{
    registerCallbacks();
}

void SpatdifReceiver::registerCallbacks()
{
    receiver_->addHandler("/SpatDIF/core/source/*/position", "fff", onSourcePositionChanged, this);
}

void SpatdifReceiver::poll()
{
    int bytes = receiver_->receive();
    if (bytes > 0 and verbose_)
        std::cout << "received " << bytes << " bytes" << std::endl;
}

int SpatdifReceiver::onSourcePositionChanged(const char * /*path*/, const char * /*types*/,
        lo_arg ** argv, int /*argc*/, void * /*data*/, void *user_data)
{
    SpatdifReceiver *context = static_cast<SpatdifReceiver *>(user_data);
    UNUSED(context);
    std::cout << __FUNCTION__ << std::endl;
    for (int i = 0; i != 3; ++i)
    {
        std::cout << argv[i]->f << ",";
    }
    std::cout << std::endl;
    return 0;
}

} // end namespace spatosc

