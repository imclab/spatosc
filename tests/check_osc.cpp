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
 * This test checks OSC sender and receiver
 *
 * To run it though gdb:
 * $ libtool --mode=execute gdb check_spatdif_osc
 */

#include "oscsender.h"
#include "oscreceiver.h"
#include <cassert>
#include <string>
#include <iostream>

#define UNUSED(x) ((void) (x))
static const bool VERBOSE = false;

struct Data
{
    bool ok;
    int argc;
    std::string arg_0;
    int arg_1;
    float arg_2;
};

static int ping_cb(const char *path, const char *types, lo_arg **argv, int argc, void *blob, void *user_data)
{
    UNUSED(path);
    UNUSED(types);
    UNUSED(blob);
    Data *data = static_cast<Data*>(user_data);
    assert(argc == data->argc);
    assert(data->arg_0 == std::string((char *) argv[0]));
    assert(data->arg_1 == argv[1]->i);
    assert(data->arg_2 == argv[2]->f);
    data->ok = true;
    if (VERBOSE)
        std::cout << "Got /ping" << std::endl;
    return 0;
}

int main()
{
    // test's scene's receiving facilities
    Data data;
    data.ok = false;
    data.argc = 3;
    data.arg_0 = "hello";
    data.arg_1 = 2;
    data.arg_2 = 3.14159f;

    const char *TEST_PORT = "11112";
    if (VERBOSE)
        std::cout << "Start receiver on port " << TEST_PORT << std::endl;
    spatosc::OscReceiver receiver(TEST_PORT);
    if (VERBOSE)
        std::cout << "Start sender on port " << TEST_PORT << std::endl;
    spatosc::OscSender sender(std::string("osc.udp://127.0.0.1:")+TEST_PORT);
    receiver.addHandler("/ping", "sif", ping_cb, &data);
    sender.sendMessage("/ping", "sif", data.arg_0.c_str(), data.arg_1, data.arg_2, LO_ARGS_END);
    receiver.receive();
    if (! data.ok)
    {
        std::cout << "Did not receive any message" << std::endl;
        return 1;
    }
    return 0;
}

