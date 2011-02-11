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
 * Translator for the proprietary D-Mitri system.
 * Note that nodes need a positive channel ID. If not set, their coordinates will not be sent to D-Mitri.
 */
class DmitriTranslator : public Translator
{
public:
    DmitriTranslator(const std::string &ip, const std::string &toPort, const std::string &fromPort, bool verbose);
    DmitriTranslator(const std::string &ip, const std::string &toPort, bool verbose);
    virtual void pushOSCMessages(Connection *conn);
    static const char *DEFAULT_SEND_PORT;
    static const char *DEFAULT_RECEIVER_PORT;

private:
    static const double SPACEMAP_RADIUS;
    std::tr1::shared_ptr<OscSender> sender_;
    // not implemented
    DmitriTranslator(const DmitriTranslator&);
    const DmitriTranslator& operator=(const DmitriTranslator&);
    bool verbose_;
};

} // end namespace spatosc

#endif // __DMITRI_TRANSLATOR_H__
