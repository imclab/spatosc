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

#include "spatosc/spatosc.h"
#include <boost/python.hpp>

using namespace boost::python;
using namespace spatosc;

// TODO: wrap overloaded methods:
// BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(addTranslator_overloads, Wrapper::addTranslator_overloads, 2, 3) // min args, max args
// .def("addTranslator", &Wrapper::addTranslator,
//         addTranslator_overloads(args("name", "type", "addr"))
//     )

BOOST_PYTHON_MODULE(spatosc)
{

    class_<Wrapper>("Wrapper", "Wraps a SpatOSC scene")
        .def("clearScene", &Wrapper::clearScene)
        .def("addTranslatorWithAddress", &Wrapper::addTranslatorWithAddress)
        .def("addTranslatorWithoutAddress", &Wrapper::addTranslatorWithoutAddress)
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
        .def("setNodeOrientation", &Wrapper::setNodeOrientation)
        .def("setNodeStringProperty", &Wrapper::setNodeStringProperty)
        .def("setPosition", &Wrapper::setPosition)
        .def("setPositionAED", &Wrapper::setPositionAED)
        .def("setRadius", &Wrapper::setRadius)
        .def("setScale", &Wrapper::setScale)
        .def("setSynchronous", &Wrapper::setSynchronous)
        .def("setTranslation", &Wrapper::setTranslation)
        .def("setTranslatorVerbose", &Wrapper::setTranslatorVerbose)
        .def("setVerbose", &Wrapper::setVerbose)
        ;
}

