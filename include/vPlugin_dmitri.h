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
 * The vPlugin_dmitri class.
 */
#ifndef __vPlugin_dmitri_H
#define __vPlugin_dmitri_H

#include <lo/lo.h>
#include <string>
#include "vPlugin.h"

class vSoundConn;

/**
 * Renderer for the proprietary D-Mitri system.
 */
class vPlugin_dmitri : public vPlugin
{

public:

	vPlugin_dmitri(const std::string &ip);
	~vPlugin_dmitri();

	void update(vSoundConn *conn);
    virtual std::string getTypeString() const;

private:

    static const double SPACEMAP_RADIUS;

	lo_address destAddr_;
	lo_server lo_serv_;
};


#endif
