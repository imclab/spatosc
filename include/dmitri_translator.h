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
 * The D-Mitri translator class.
 */
#ifndef __DMITRI_TRANSLATOR_H__
#define __DMITRI_TRANSLATOR_H__

#include <string>
#include "memory.h"
#include "translator.h"

namespace spatosc
{
class OscSender;
class Connection;

/**
 * Translator for the proprietary D-Mitri system from Meyer Sound
 * (http://www.meyersound.com), and works in conjunction with a spacemap defined
 * in the CueStation software.
 *
 * This translator requires the following configuration in CueStation:
 *
 * 1) You must define a spacemap that performs spherical panning of audio. The
 *    The assumption is that the sphere is viewed from the center, looking up,
 *    and is projected on a plane. The lower hemisphere is unfolded and mapped
 *    along the outside of the resuling circular projection. Positive elevations
 *    result in spacemap positions ranging from (0,0) to SPACEMAP_EQUATOR_RADIUS
 *    and ngative elevations range from the SPACEMAP_EQUATOR_RADIUS to the
 *    SPACEMAP_POLE_RADUIS.
 *
 *    Examples:
 *    -> a sound source in front (0,1,0) will have
 *       a spacemap position of: (0,-SPACEMAP_EQUATOR_RADIUS)
 *    -> a sound source above (0,0,1) will have
 *       a spacemap position of: (0,0)
 *
 * 2) You must create an OSC Map Command that takes a spacemap message with two
 *    floats and sends to the control points for the X and Y position on the
 *    spacemap.

 *    See below for a screenshot of the OSC Map Command
 *    \image html doc/images/CueStation_OSC_map_command.png
 *
 *    NOTE: it is possible to send an OSC message directly to the
 *    spacemap, but you need to send a 'P' type, which is non-standard, not
 *    included in any OSC library, and proprietary to Meyer. There is a request
 *    to change this (Fogbugz #6798), which will remove this requirement in the
 *    future.
 *
 *
 */
class DmitriTranslator : public Translator
{
public:

    /**
     * The DEFAULT_SEND_PORT should always be 18033 (as of CueStation 5.0)
     */
    static const char *DEFAULT_SEND_PORT;

    /**
     * The DEFAULT_RECEIVER_PORT is specified in CueStation when defining an
     * OSC Map Command. Only messages from this outgoing port will be accepted.
     */
    static const char *DEFAULT_RECEIVER_PORT;

    /**
     * Constructor (only IP address is required; ports can be overridden)
     */
    DmitriTranslator(
            const std::string &ip,
            const std::string &toPort = DmitriTranslator::DEFAULT_SEND_PORT,
            const std::string &fromPort = DmitriTranslator::DEFAULT_RECEIVER_PORT,
            bool verbose = false);

    /**
     * Legacy constructor to support templated scene::addTranslator method
     * (to be deprecated)
     */
    DmitriTranslator(const std::string &ip, const std::string &toPort, bool verbose = false);

    /**
     * This is where we customize messages for D-Mitri's OSC protocol.
     */
    virtual void pushConnectionChanges(Connection *conn);

    /**
     * Gets the OscSender object so that you can send arbitrary commands to the
     * D-Mitri input socket. For example, you could send
     * \verbatim /set "Input 1 Level" 40 \endverbatim
     * to set an input level (independent of the Bus level that is controlled by
     * SpatOSC).
     */
    OscSender &getSender() const;

    /**
     * Set the equator radius in the spacemap.
     *
     * NOTE: You MUST do this before you create your scene, otherwise the effect
     * won't be applied until a node is moved. Alternatively, you could call
     * scene::forceRefresh to ensure that all nodes are properly updated
     * afterwards
     */
    void setEquatorRadius(double radius);

private:
    static double SPACEMAP_EQUATOR_RADIUS;
    static const double SPACEMAP_POLE_RADIUS;
    std::tr1::shared_ptr<OscSender> sender_;
    // not implemented
    DmitriTranslator(const DmitriTranslator&);
    const DmitriTranslator& operator=(const DmitriTranslator&);
    bool verbose_;
};

} // end namespace spatosc

#endif // __DMITRI_TRANSLATOR_H__
