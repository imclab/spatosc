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
 * Wrapper for including <tr1/memory>
 */
#ifndef __GEOTRANSFORM_H__
#define __GEOTRANSFORM_H__

#include "maths.h"

namespace spatosc {
class GeoTransform 
{
    public:
        GeoTransform();
        /** Maps a given point to transformed coordinates */
        void apply(double &x, double &y, double &z) const;
        void translate(double tx, double ty, double tz);
    private:
        spatosc::Matrix4 mat_;
};

} // end namespace spatosc
#endif // __GEOTRANSFORM_H__

