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
 * The GeoTransform class.
 */
#ifndef __GEOTRANSFORM_H__
#define __GEOTRANSFORM_H__

#include <vector>
#include "maths.h"

namespace spatosc {
/**
 * Applies geometrical transformations on 3D position and orientation vectors.
 * Transformations are: rotation, scale and translation.
 */
class GeoTransform 
{
    public:
        GeoTransform();
        /**
         * Applies the transformations to a given position
         */
        void apply(double &x, double &y, double &z) const;
        /**
         * Applies the transformations to a given position
         */
        void apply(Vector3 &pos) const;
        /**
         * Applies the transformations to a given orientation
         */
        void applyToOrientation(Quaternion &orientation) const;

        /** 
         * Changes the translation transformation to apply.
         */
        void translate(double tx, double ty, double tz);
        /**
         * Changes the orientation transformation to apply.
         */
        void rotate(double pitch, double roll, double yaw);
        /**
         * Changes the orientation transformation to apply.
         */
        void rotate(double x, double y, double z, double w);
        /**
         * Changes the scale transformation to apply.
         */
        void scale(double sx, double sy, double sz);
    private:
        std::vector<double> translation_;
        Quaternion rotation_;
        std::vector<double> scaling_;
        bool hasTransformation_;
};

} // end namespace spatosc
#endif // __GEOTRANSFORM_H__

