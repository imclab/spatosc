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
#include "geotransform.h"
namespace spatosc
{

GeoTransform::GeoTransform() : mat_(spatosc::Matrix4Identity()), hasTransformation_(false)
{}

void GeoTransform::apply(double &x, double &y, double &z) const
{
    if (hasTransformation_)
    {
        // FIXME:Tue Feb  8 14:02:51 EST 2011:tmatth:only does translation right now
        x += mat_._41;
        y += mat_._42;
        z += mat_._43;
    }
}

void GeoTransform::translate(double tx, double ty, double tz)
{
    hasTransformation_ = true;
    mat_._41 = tx;
    mat_._42 = ty;
    mat_._43 = tz;
}

} // end namespace spatosc

