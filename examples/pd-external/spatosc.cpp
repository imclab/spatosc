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
 * Manages a spatosc audio scene within Pure Data.
 */

#include "m_pd.h"
#include <cstdio>
#include <lo/lo.h>
#include <spatosc/spatosc.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>

#define UNUSED(x) ((void) (x))

static const bool SPATOSC_DEBUG = true;
static const float SUCCESS = 1.0;
static const float FAILURE = 0.0;

// The Pd t_class instance, and related object struct:
static t_class *spatosc_class;

typedef struct _spatosc
{
    t_object x_obj;
    spatosc::Wrapper wrapper;
	t_outlet *outlet_status;
} t_spatosc;

static void spatosc_print_usage()
{
    post("[spatosc] ERROR: Usage: [spatosc <s:translator> <f:port> <s:host>]");
}

static void *spatosc_new(t_symbol *s, int argc, t_atom *argv)
{
    UNUSED(s);
    t_spatosc *x = (t_spatosc *) pd_new(spatosc_class);
    x->wrapper = spatosc::Wrapper();
    
    int port = 0;
    t_symbol *host = gensym("NULL");
    t_symbol *translator = gensym("NULL");
    // TRANSLATOR
    if (argc >= 1) 
    {
        if (argv[0].a_type == A_SYMBOL)
            translator = argv[0].a_w.w_symbol;
        else
            spatosc_print_usage();
    }
    // PORT
    if (argc >= 2)
    {
        if (argv[1].a_type == A_FLOAT)
            port = (int) argv[1].a_w.w_float;
        else
            spatosc_print_usage();
    }
    // HOST
    if (argc >= 3) 
    {
        if (argv[2].a_type == A_SYMBOL)
            host = argv[2].a_w.w_symbol;
        else
            spatosc_print_usage();
    }
    std::string translatorName = "ConsoleTranslator";
    std::string sendToAddress = "localhost";
    std::string sendToPort = "0";
    if (std::string("NULL") != translator->s_name)
    {
        translatorName = translator->s_name;
        if (translatorName == "DmitriTranslator")
            sendToPort = spatosc::DmitriTranslator::DEFAULT_SEND_PORT;
        else if (translatorName == "SpatdifTranslator")
            sendToPort = spatosc::SpatdifTranslator::DEFAULT_SEND_PORT;
    }
    if (std::string("NULL") != host->s_name)
        sendToAddress = host->s_name;
    if (0 != port)
    {
        std::ostringstream os;
        os << port;
        sendToPort = os.str();
    }
    if (SPATOSC_DEBUG)
    {
        post("[spatosc]: translatorName=%s sendToPort=%s sendToAddress=%s", translatorName.c_str(), sendToPort.c_str(), sendToAddress.c_str());
    }
    bool success = x->wrapper.setTranslator(translatorName, sendToAddress, sendToPort);
    if (! success)
    {
        post("[spatosc]: ERROR calling setTranslator from the constructor.");
    }
    
    // create outlets
    x->outlet_status = outlet_new(&x->x_obj, 0);
    return (x);
}

static void output_success(t_spatosc *x, bool success)
{
	outlet_float(x->outlet_status, success ? SUCCESS : FAILURE);
}

static void spatosc_free(t_spatosc *x)
{
    UNUSED(x);
    if (SPATOSC_DEBUG)
        post("[spatosc] freed");
}

static void spatosc_createSource(t_spatosc *x, t_symbol *node);
static void spatosc_createListener(t_spatosc *x, t_symbol *node);
static void spatosc_deleteNode(t_spatosc *x, t_symbol *node);
static void spatosc_connect(t_spatosc *x, t_symbol *from, t_symbol *to);
static void spatosc_disconnect(t_spatosc *x, t_symbol *from, t_symbol *to);
static void spatosc_clearScene(t_spatosc *x, int argc, t_atom *argv);
static void spatosc_setConnectFilter(t_spatosc *x, t_symbol *filter);
static void spatosc_setOrientation(t_spatosc *x, t_symbol *node, t_floatarg pitch, t_floatarg roll, t_floatarg yaw);
static void spatosc_setPosition(t_spatosc *x, t_symbol *node, t_floatarg xPos, t_floatarg yPos, t_floatarg zPos);
static void spatosc_setAutoConnect(t_spatosc *x, t_floatarg enabled);
static void spatosc_setTranslator(t_spatosc *x, t_symbol *translator, t_symbol *host, t_floatarg port);

extern "C" void spatosc_setup(void)
{
    spatosc_class = class_new(gensym("spatosc"), (t_newmethod) spatosc_new, (t_method) spatosc_free, sizeof(t_spatosc), CLASS_DEFAULT, A_GIMME, 0);
	class_addmethod(spatosc_class, (t_method) spatosc_createSource, gensym("createSource"), A_SYMBOL, 0);
	class_addmethod(spatosc_class, (t_method) spatosc_createListener, gensym("createListener"), A_SYMBOL, 0);
	class_addmethod(spatosc_class, (t_method) spatosc_deleteNode, gensym("deleteNode"), A_SYMBOL, 0);
	class_addmethod(spatosc_class, (t_method) spatosc_connect, gensym("connect"), A_SYMBOL, A_SYMBOL, 0);
	class_addmethod(spatosc_class, (t_method) spatosc_disconnect, gensym("disconnect"), A_SYMBOL, A_SYMBOL, 0);
	class_addmethod(spatosc_class, (t_method) spatosc_clearScene, gensym("clearScene"), A_GIMME, 0);
	class_addmethod(spatosc_class, (t_method) spatosc_setAutoConnect, gensym("setAutoConnect"), A_FLOAT, 0);
	class_addmethod(spatosc_class, (t_method) spatosc_setConnectFilter, gensym("setConnectFilter"), A_SYMBOL, 0);
	class_addmethod(spatosc_class, (t_method) spatosc_setOrientation, gensym("setOrientation"), A_SYMBOL, A_FLOAT, A_FLOAT, A_FLOAT, 0);
	class_addmethod(spatosc_class, (t_method) spatosc_setPosition, gensym("setPosition"), A_SYMBOL, A_FLOAT, A_FLOAT, A_FLOAT, 0);
    if (SPATOSC_DEBUG)
        post("[spatosc] loaded");
}

static void spatosc_createSource(t_spatosc *x, t_symbol *node)
{
    output_success(x, x->wrapper.createSource(node->s_name));
}

static void spatosc_createListener(t_spatosc *x, t_symbol *node)
{
    output_success(x, x->wrapper.createListener(node->s_name));
}

static void spatosc_deleteNode(t_spatosc *x, t_symbol *node)
{
    output_success(x, x->wrapper.deleteNode(node->s_name));
}

static void spatosc_connect(t_spatosc *x, t_symbol *from, t_symbol *to)
{
    output_success(x, x->wrapper.connect(from->s_name, to->s_name));
}

static void spatosc_disconnect(t_spatosc *x, t_symbol *from, t_symbol *to)
{
    output_success(x, x->wrapper.disconnect(from->s_name, to->s_name));
}

static void spatosc_clearScene(t_spatosc *x, int argc, t_atom *argv)
{
    UNUSED(argc);
    UNUSED(argv);
    output_success(x, x->wrapper.clearScene());
}

static void spatosc_setConnectFilter(t_spatosc *x, t_symbol *filter)
{
    output_success(x, x->wrapper.setConnectFilter(filter->s_name));
}

static void spatosc_setOrientation(t_spatosc *x, t_symbol *node, t_floatarg pitch, t_floatarg roll, t_floatarg yaw)
{
    output_success(x, x->wrapper.setOrientation(node->s_name, pitch, roll, yaw));
}

static void spatosc_setPosition(t_spatosc *x, t_symbol *node, t_floatarg xPos, t_floatarg yPos, t_floatarg zPos)
{
    output_success(x, x->wrapper.setPosition(node->s_name, xPos, yPos, zPos));
}

static void spatosc_setAutoConnect(t_spatosc *x, t_floatarg enabled)
{
    output_success(x, x->wrapper.setAutoConnect(enabled != 0.0f));
}

static void spatosc_setTranslator(t_spatosc *x, t_symbol *translator, t_symbol *host, t_floatarg port)
{
    std::string translatorName = "ConsoleTranslator";
    std::string sendToAddress = "localhost";
    std::string sendToPort = spatosc::SpatdifTranslator::DEFAULT_SEND_PORT;
    if (std::string("NULL") != translator->s_name)
    {
        translatorName = translator->s_name;
        if (translatorName == "DmitriTranslator")
            sendToPort = spatosc::DmitriTranslator::DEFAULT_SEND_PORT;
    }
    if (std::string("NULL") != host->s_name)
        sendToAddress = host->s_name;
    if (0 != port)
    {
        std::ostringstream os;
        os << port;
        sendToPort = os.str();
    }
    if (SPATOSC_DEBUG)
    {
        printf("[spatosc]: translatorName=%s sendToPort=%s sendToAddress=%s", translatorName.c_str(), sendToPort.c_str(), sendToAddress.c_str());
        post("[spatosc]: translatorName=%s sendToPort=%s sendToAddress=%s", translatorName.c_str(), sendToPort.c_str(), sendToAddress.c_str());
    }
    output_success(x, x->wrapper.setTranslator(translatorName, sendToAddress, sendToPort));
}
