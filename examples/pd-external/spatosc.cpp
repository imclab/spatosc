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
#include <string>

//static const bool SPATOSC_DEBUG = true;
static const bool SPATOSC_DEBUG = false;
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
    // arg 0: TRANSLATOR
    if (argc >= 1) 
    {
        if (argv[0].a_type == A_SYMBOL)
            translator = argv[0].a_w.w_symbol;
        else
            spatosc_print_usage();
    }
    // arg 1: PORT
    if (argc >= 2)
    {
        if (argv[1].a_type == A_FLOAT)
            port = (int) argv[1].a_w.w_float;
        else
            spatosc_print_usage();
    }
    // arg 2: HOST
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
        else if (translatorName == "BasicTranslator")
            sendToPort = spatosc::BasicTranslator::DEFAULT_SEND_PORT;
        else if (translatorName == "FudiTranslator")
            sendToPort = spatosc::FudiTranslator::DEFAULT_SEND_PORT;
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
        post("[spatosc]: translatorName=%s sendToPort=%s sendToAddress=%s\n", translatorName.c_str(), sendToPort.c_str(), sendToAddress.c_str());
    }
    
    bool success;
    if (translatorName == "DmitriTranslator")
        success = x->wrapper.addDmitriTranslator("default", sendToAddress, sendToPort, SPATOSC_DEBUG);
    else 
        success = x->wrapper.addTranslator("default", translatorName, sendToAddress, sendToPort, SPATOSC_DEBUG);
    
    if (! success)
    {
        post("[spatosc]: ERROR calling addTranslator from [spatosc]'s constructor.\n");
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
static void spatosc_setPositionAED(t_spatosc *x, t_symbol *node, t_floatarg angle, t_floatarg elevation, t_floatarg distance);
static void spatosc_setAutoConnect(t_spatosc *x, t_floatarg enabled);
static void spatosc_addTranslator(t_spatosc *x, t_symbol *identifier, t_symbol *translator, t_symbol *host, t_floatarg port);
static void spatosc_removeTranslator(t_spatosc *x, t_symbol *translator);
static void spatosc_hasTranslator(t_spatosc *x, t_symbol *translator);
static void spatosc_setNodeStringProperty(t_spatosc *x, t_symbol *node, t_symbol *key, t_symbol *value);
static void spatosc_setNodeFloatProperty(t_spatosc *x, t_symbol *node, t_symbol *key, t_floatarg value);
static void spatosc_setNodeIntProperty(t_spatosc *x, t_symbol *node, t_symbol *key, t_floatarg value);
static void spatosc_removeNodeStringProperty(t_spatosc *x, t_symbol *node, t_symbol *key);
static void spatosc_removeNodeIntProperty(t_spatosc *x, t_symbol *node, t_symbol *key);
static void spatosc_removeNodeFloatProperty(t_spatosc *x, t_symbol *node, t_symbol *key);
static void spatosc_setDistanceFactor(t_spatosc *x, t_symbol *src, t_symbol *sink, t_floatarg factor);
static void spatosc_setDopplerFactor(t_spatosc *x, t_symbol *src, t_symbol *sink, t_floatarg factor);

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
	class_addmethod(spatosc_class, (t_method) spatosc_setPositionAED, gensym("setPositionAED"), A_SYMBOL, A_FLOAT, A_FLOAT, A_FLOAT, 0);
	class_addmethod(spatosc_class, (t_method) spatosc_addTranslator, gensym("addTranslator"), A_SYMBOL, A_SYMBOL, A_SYMBOL, A_FLOAT, 0);
	class_addmethod(spatosc_class, (t_method) spatosc_removeTranslator, gensym("removeTranslator"), A_SYMBOL, 0);
	class_addmethod(spatosc_class, (t_method) spatosc_hasTranslator, gensym("hasTranslator"), A_SYMBOL, 0);
	class_addmethod(spatosc_class, (t_method) spatosc_setNodeStringProperty, gensym("setNodeStringProperty"), A_SYMBOL, A_SYMBOL, A_SYMBOL, 0);
	class_addmethod(spatosc_class, (t_method) spatosc_setNodeFloatProperty, gensym("setNodeFloatProperty"), A_SYMBOL, A_SYMBOL, A_FLOAT, 0);
	class_addmethod(spatosc_class, (t_method) spatosc_setNodeIntProperty, gensym("setNodeIntProperty"), A_SYMBOL, A_SYMBOL, A_FLOAT, 0);
	class_addmethod(spatosc_class, (t_method) spatosc_removeNodeStringProperty, gensym("removeNodeStringProperty"), A_SYMBOL, A_SYMBOL, 0);
	class_addmethod(spatosc_class, (t_method) spatosc_removeNodeIntProperty, gensym("removeNodeIntProperty"), A_SYMBOL, A_SYMBOL, 0);
	class_addmethod(spatosc_class, (t_method) spatosc_removeNodeFloatProperty, gensym("removeNodeFloatProperty"), A_SYMBOL, A_SYMBOL, 0);
	class_addmethod(spatosc_class, (t_method) spatosc_setDistanceFactor, gensym("setDistanceFactor"), A_SYMBOL, A_SYMBOL, A_FLOAT, 0);
	class_addmethod(spatosc_class, (t_method) spatosc_setDopplerFactor, gensym("setDopplerFactor"), A_SYMBOL, A_SYMBOL, A_FLOAT, 0);
    if (SPATOSC_DEBUG)
    {
        post("[spatosc]: (c) Society for Arts and Technology 2011");
        post("[spatosc]: free software released under the terms of the GNU General Public License.");
        post("[spatosc]: written by Mike Wozniewski, Alexandre Quessy and Tristan Matthews.");
    }
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

static void spatosc_setPositionAED(t_spatosc *x, t_symbol *node, t_floatarg angle, t_floatarg elevation, t_floatarg distance)
{
    output_success(x, x->wrapper.setPositionAED(node->s_name, angle, elevation, distance));
}

static void spatosc_setAutoConnect(t_spatosc *x, t_floatarg enabled)
{
    output_success(x, x->wrapper.setAutoConnect(enabled != 0.0f));
}

// TODO: invert port and host args?
static void spatosc_addTranslator(t_spatosc *x, t_symbol *identifier, t_symbol *translator, t_symbol *host, t_floatarg port)
{
    std::string translatorName = "ConsoleTranslator";
    std::string sendToAddress = "localhost";
    std::string sendToPort = spatosc::BasicTranslator::DEFAULT_SEND_PORT;
    if (std::string("NULL") != translator->s_name)
    {
        translatorName = translator->s_name;
        if (translatorName == "DmitriTranslator")
            sendToPort = spatosc::DmitriTranslator::DEFAULT_SEND_PORT;
        if (translatorName == "FudiTranslator")
            sendToPort = spatosc::FudiTranslator::DEFAULT_SEND_PORT;
    }
    if (host)
    {
        if (std::string("NULL") != host->s_name)
        {
            sendToAddress = host->s_name;
        }
    }
    else
        post("[spatosc]: Error: invalid host symbol");
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
    
    if (translatorName == "DmitriTranslator")
        output_success(x, x->wrapper.addDmitriTranslator(identifier->s_name, sendToAddress, sendToPort, true));
    else
        output_success(x, x->wrapper.addTranslator(identifier->s_name, translatorName, sendToAddress, sendToPort, true));
}

static void spatosc_removeTranslator(t_spatosc *x, t_symbol *identifier)
{
    output_success(x, x->wrapper.removeTranslator(identifier->s_name));
}

static void spatosc_hasTranslator(t_spatosc *x, t_symbol *identifier)
{
    output_success(x, x->wrapper.hasTranslator(identifier->s_name));
}

// setNode*Property:
static void spatosc_setNodeStringProperty(t_spatosc *x, t_symbol *node, t_symbol *key, t_symbol *value)
{
    output_success(x, x->wrapper.setNodeStringProperty(node->s_name, key->s_name, value->s_name));
}

static void spatosc_setNodeFloatProperty(t_spatosc *x, t_symbol *node, t_symbol *key, t_floatarg value)
{
    output_success(x, x->wrapper.setNodeFloatProperty(node->s_name, key->s_name, (double) value));
}

static void spatosc_setNodeIntProperty(t_spatosc *x, t_symbol *node, t_symbol *key, t_floatarg value)
{
    output_success(x, x->wrapper.setNodeIntProperty(node->s_name, key->s_name, (int) value));
}

// removeNode*Property:
static void spatosc_removeNodeStringProperty(t_spatosc *x, t_symbol *node, t_symbol *key)
{
    output_success(x, x->wrapper.removeNodeStringProperty(node->s_name, key->s_name));
}

static void spatosc_removeNodeIntProperty(t_spatosc *x, t_symbol *node, t_symbol *key)
{
    output_success(x, x->wrapper.removeNodeIntProperty(node->s_name, key->s_name));
}

static void spatosc_removeNodeFloatProperty(t_spatosc *x, t_symbol *node, t_symbol *key)
{
    output_success(x, x->wrapper.removeNodeFloatProperty(node->s_name, key->s_name));
}

static void spatosc_setDistanceFactor(t_spatosc *x, t_symbol *src, t_symbol *sink, t_floatarg factor)
{
    output_success(x, x->wrapper.setDistanceFactor(src->s_name, sink->s_name, factor));
}

static void spatosc_setDopplerFactor(t_spatosc *x, t_symbol *src, t_symbol *sink, t_floatarg factor)
{
    output_success(x, x->wrapper.setDopplerFactor(src->s_name, sink->s_name, factor));
}

