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

#include <lo/lo.h>
#include <string>
#include "translator.h"

namespace spatosc
{
class Connection;

/**
 * Translator for the proprietary D-Mitri system.
 */
class DmitriTranslator : public Translator
{
public:
    DmitriTranslator(const std::string &ip);
    ~DmitriTranslator();
    void update(Connection *conn);
    virtual std::string getTypeString() const;

private:
    static const double SPACEMAP_RADIUS;
    lo_address destAddr_;
    lo_server lo_serv_;
};

} // end namespace spatosc

#endif // __DMITRI_TRANSLATOR_H__
