/*
 * basic_receiver.cpp
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

#include "spatosc/spatosc.h"
#include <boost/python.hpp>

BOOST_PYTHON_MODULE(spatosc)
{
    using namespace boost::python;
    using namespace spatosc;

    class_<Wrapper>("Wrapper")
        //.def("addTranslator", &Wrapper::addTranslator)
        .def("clearScene", &Wrapper::clearScene)
        .def("connect", &Wrapper::connect)
        .def("createListener", &Wrapper::createListener)
        .def("createSource", &Wrapper::createSource)
        .def("debugPrint", &Wrapper::debugPrint)
        .def("deleteNode", &Wrapper::deleteNode)
        .def("disconnect", &Wrapper::disconnect)
        .def("flushMessages", &Wrapper::flushMessages)
        .def("getNodeFloatProperty", &Wrapper::getNodeFloatProperty)
        .def("getNodeIntProperty", &Wrapper::getNodeIntProperty)
        .def("getNodeStringProperty", &Wrapper::getNodeStringProperty)
        .def("hasTranslator", &Wrapper::hasTranslator)
        .def("removeNodeFloatProperty", &Wrapper::removeNodeFloatProperty)
        .def("removeNodeIntProperty", &Wrapper::removeNodeIntProperty)
        .def("removeNodeStringProperty", &Wrapper::removeNodeStringProperty)
        .def("removeTranslator", &Wrapper::removeTranslator)
        .def("setAutoConnect", &Wrapper::setAutoConnect)
        .def("setConnectFilter", &Wrapper::setConnectFilter)
        .def("setDefaultDistanceFactor", &Wrapper::setDefaultDistanceFactor)
        .def("setDefaultDopplerFactor", &Wrapper::setDefaultDopplerFactor)
        .def("setDefaultRolloffFactor", &Wrapper::setDefaultRolloffFactor)
        .def("setDistanceFactor", &Wrapper::setDistanceFactor)
        .def("setDopplerFactor", &Wrapper::setDopplerFactor)
        .def("setNodeActive", &Wrapper::setNodeActive)
        .def("setNodeFloatProperty", &Wrapper::setNodeFloatProperty)
        .def("setNodeIntProperty", &Wrapper::setNodeIntProperty)
        .def("setNodeStringProperty", &Wrapper::setNodeStringProperty)
        //.def("setOrientation", &Wrapper::setOrientation)
        .def("setPosition", &Wrapper::setPosition)
        .def("setPositionAED", &Wrapper::setPositionAED)
        .def("setRadius", &Wrapper::setRadius)
        .def("setScale", &Wrapper::setScale)
        .def("setSynchronous", &Wrapper::setSynchronous)
        .def("setTranslation", &Wrapper::setTranslation)
        .def("setTranslatorVerbose", &Wrapper::setTranslatorVerbose)
        ;
}

