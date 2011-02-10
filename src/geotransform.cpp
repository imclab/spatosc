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

#include <iostream>

#include "geotransform.h"
#include "maths.h"

namespace spatosc
{

GeoTransform::GeoTransform() : 
    translation_(3, 0.0), 
    rotation_(0, 0, 0, 1), 
    scaling_(3, 1.0), 
    hasTransformation_(false)
{
}

void GeoTransform::apply(double &x, double &y, double &z) const
{
    if (hasTransformation_)
    {
        // apply translation, rotation, then scale 
        // translation
        x += translation_[0];
        y += translation_[1];
        z += translation_[2];

        // rotation
        Vector3 tmp(x, y, z);
        tmp = rotation_ * tmp;

        x = tmp.x;
        y = tmp.y;
        z = tmp.z;
        
        // scaling
        x *= scaling_[0];
        y *= scaling_[1];
        z *= scaling_[2];
    }
}

void GeoTransform::apply(Vector3 &pos) const
{
    apply(pos.x, pos.y, pos.z);
}
        
void GeoTransform::scale(double sx, double sy, double sz)
{
    hasTransformation_ = true;
    scaling_[0] = sx;
    scaling_[1] = sy;
    scaling_[2] = sz;
}

void GeoTransform::translate(double tx, double ty, double tz)
{
    hasTransformation_ = true;
    translation_[0] = tx;
    translation_[1] = ty;
    translation_[2] = tz;
}

void GeoTransform::rotate(double pitch, double roll, double yaw)
{
    hasTransformation_ = true;
    Vector3 rot(pitch, roll, yaw);
    rotation_ = EulerToQuat(rot);
}

void GeoTransform::rotate(double x, double y, double z, double w)
{
    hasTransformation_ = true;
    rotation_.x = x;
    rotation_.y = y;
    rotation_.z = z;
    rotation_.w = w;
}

} // end namespace spatosc

