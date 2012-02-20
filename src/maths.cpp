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

#include "maths.h"

namespace
{

// std::isnan is not found in some old std C++ libraries, so we define it here:
inline bool isNan(double x)
{
    return x != x;
}

} // end of anonymous namespace

namespace spatosc
{

double distance2(Vector2 v1, Vector2 v2)
{
    double dx = v2.x - v1.x;
    double dy = v2.y - v1.y;
    return (double) sqrt(dx*dx+dy*dy);
}

Vector2 midpoint2(Vector2 v1, Vector2 v2)
{
    return Vector2( (v1.x+v2.x)/2.0,(v1.y+v2.y)/2.0 );
}

Vector2 centroid2(Vector2 v1, Vector2 v2, Vector2 v3)
{
    double avgX = (v1.x+v2.x+v3.x) / 3.0;
    double avgY = (v1.y+v2.y+v3.y) / 3.0;
    return Vector2(avgX,avgY);
}

Vector2 centroid2(Vector2 v1, Vector2 v2, Vector2 v3, Vector2 v4)
{
    double avgX = (v1.x+v2.x+v3.x+v4.x) / 4.0;
    double avgY = (v1.y+v2.y+v3.y+v4.y) / 4.0;
    return Vector2(avgX,avgY);
}

double distance3(Vector3 v1, Vector3 v2)
{
    double dx = v2.x - v1.x;
    double dy = v2.y - v1.y;
    double dz = v2.z - v1.z;
    return (double) sqrt(dx*dx+dy*dy+dz*dz);
}

double computeAngle2(double dx, double dy)
{
    double angle = 0;
    if (dx == 0)
    {
        angle = (double) M_PI / 2;
    }
    else
    {
        angle = (double)(atan2(dy,dx) + M_PI);
    }
    return angle;
}

double computeAngle(double dx, double dy)
{
    double angle = 0;
    if (dx == 0)
    {
        angle = (double) M_PI / 2;
    }
    else
    {
        if (dx > 0)
            return (double) atan(dy / dx);
        else
            angle = (double)(atan(dy / dx) + M_PI);
    }
    return angle;
}

double angleBetweenVectors(Vector3 v1, Vector3 v2)
{
    // normalize vectors
    v1.Normalize();
    v2.Normalize();

    // Get the dot product of the vectors
    double dotProduct = v1 * v2;

    // for acos, the value has to be between -1.0 and 1.0, but due to numerical
    // imprecisions it sometimes comes outside this range
    if (dotProduct > 1.0)
        dotProduct = 1.0;
    if (dotProduct < -1.0)
        dotProduct = -1.0;

    // Get the angle in radians between the 2 vectors
    // (should this be -acos ? ie, negative?)
    double angle = acos( dotProduct );

    // Here we make sure that the angle is not a -1.#IND0000000 number,
    // which means indefinite
    if (isNan(angle))
        return 0;

    // Return the angle in radians
    return angle;
}

double angleBetweenVectors(Vector3 v1, Vector3 v2, int nullAxis)
{
	Vector2 tmp1, tmp2;
    switch (nullAxis)
    {
        case 1: // X_AXIS is ignored (ELEV)
			tmp1 = Vector2(v1.y,v1.z);
			tmp2 = Vector2(v2.y,v2.z);
			break;
        case 2: // Y_AXIS is ignored (ROLL)
            tmp1 = Vector2(v1.x,v1.z);
			tmp2 = Vector2(v2.x,v2.z);
            break;
        case 3: // Z_AXIS is ignored (AZIM)
            tmp1 = Vector2(v1.x,v1.y);
			tmp2 = Vector2(v2.x,v2.y);
            break;
    }

    tmp1.Normalize();
    tmp2.Normalize();

    double angle = atan2(tmp2.y,tmp2.x) - atan2(tmp1.y,tmp1.x);
    if (angle > M_PI)
	angle -= 2 * M_PI;
    else if (angle < -M_PI)
	angle += 2 * M_PI;

    return(angle);
}

/**
 * Returns a quaternion that represents the rotation from v1 to v2
 */
Quaternion RotationBetweenVectors(Vector3 v1, Vector3 v2)
{
    v1.Normalize();
    v2.Normalize();

    // Get the dot product of the vectors
    double dotProduct = v1 * v2;
    Vector3 crossProduct = v1 ^ v2;

    double qw = (double) sqrt(v1.Mag2()*v2.Mag2()) + dotProduct;

    if (qw < 0.0001) { // vectors are 180 degrees apart
        return Quaternion(0,-v1.z,v1.y,v1.x);
    }

    return Quaternion(crossProduct.x, crossProduct.y, crossProduct.z, qw);
}
/**
 * This is from OSG's Quat::makeRotate
 * by Nicolas Brodu
 * http://logiciels.cnes.fr/MARMOTTES/marmottes-mathematique.pdf
 */
Quaternion RotationBetweenVectors2(Vector3 v1, Vector3 v2)
{
    const float epsilon = 0.0000001;
    Quaternion q;

    Vector3 sourceVector = v1;
    Vector3 targetVector = v2;
    sourceVector.Normalize();
    targetVector.Normalize();

    double dotProdPlus1 = (sourceVector * targetVector) + 1.0;

    if (dotProdPlus1 < epsilon)
    {
        // Vectors are opposite, so we need to find an orthogonal vector to v1
        // around which to rotate, and give it an angle of pi
        // The trick is to realize one value at least is >0.6 for a normalized
        // vector
        if (fabs(sourceVector.x) < 0.6) {
            const double norm = sqrt(1.0 - (sourceVector.x*sourceVector.x));
            q.x = 0.0;
            q.y = sourceVector.z / norm;
            q.z = -sourceVector.y / norm;
            q.w = 0.0;
        } else if (fabs(sourceVector.y) < 0.6) {
            const double norm = sqrt(1.0 - (sourceVector.y*sourceVector.y));
            q.x = -sourceVector.z / norm;
            q.y = 0.0;
            q.z = sourceVector.x / norm;
            q.w = 0.0;
        } else {
            const double norm = sqrt(1.0 - (sourceVector.z*sourceVector.z));
            q.x = sourceVector.y / norm;
            q.y = -sourceVector.x / norm;
            q.z = 0.0;
            q.w = 0.0;
        }
    }

    else {
        // Find the shortest angle quaternion that transforms normalized vectors
        // into one other. Formula is still valid when vectors are colinear
        const double s = sqrt(0.5 * dotProdPlus1);
        const Vector3 tmp = (sourceVector ^ targetVector) / (2.0*s);
        q.x = tmp.x;
        q.y = tmp.y;
        q.z = tmp.z;
        q.w = s;
    }

    return q;
}

Quaternion RotationBetweenVectors3(Vector3 v1, Vector3 v2)
{
    const float epsilon = 0.0000001;

    double length1 = v1.Mag();
    double length2 = v2.Mag();
    double cosangle = (v1 * v2) / (length1 * length2);

    if (fabs(cosangle-1.0) < epsilon)
    {
        // near co-linear vectors
        return Quaternion(0.0, 0.0, 0.0, 1.0);
    }

    else if (fabs(cosangle+1.0) < epsilon)
    {
        // vectors are opposite, so we need to find an orthogonal vector to v1
        // around which to rotate

        Vector3 tmp;

        if (fabs(v1.x)<fabs(v1.y))
            if (fabs(v1.x)<fabs(v1.z))
                tmp = Vector3(1.0,0.0,0.0); // use x axis.
            else
                tmp = Vector3(0.0,0.0,1.0);
        else if (fabs(v1.y)<fabs(v1.z))
            tmp = Vector3(0.0,1.0,0.0);
        else
            tmp = Vector3(0.0,0.0,1.0);

        // find orthogonal axis.
        Vector3 axis = v2^tmp;
        axis.Normalize();

        // cos of half angle of PI is zero.
        return QuatFromAxis(axis, 0.0);
    }

    else {

        // this is the usual case: take a cross-product of v1 and v2
        // and that is the axis around which to rotate

        Vector3 axis = v1^v2;
        double angle = acos( cosangle );
        return QuatFromAxis(axis, angle);
    }
}

//Quaternion EulerToQuat (double roll, double pitch, double yaw)
Quaternion EulerToQuat (Vector3 v)
{
    double cr, cp, cy, sr, sp, sy, cpcy, spsy;

    // IMPORTANT: note pitch/roll flip for these equations:
    // TODO: isn't it wrong to flip these? see commented section below
    double pitch = v.y * TO_RADIANS;
    double roll  = v.x * TO_RADIANS;
    double yaw   = v.z * TO_RADIANS;

    // calculate trig identities
    cr = cos(roll/2);
    cp = cos(pitch/2);
    cy = cos(yaw/2);

    sr = sin(roll/2);
    sp = sin(pitch/2);
    sy = sin(yaw/2);

    cpcy = cp * cy;
    spsy = sp * sy;

    return Quaternion( sr * cpcy - cr * spsy,
                    cr * sp * cy + sr * cp * sy,
                    cr * cp * sy - sr * sp * cy,
                    cr * cpcy + sr * spsy );

    /*
    double angle = pitch * 0.5;
    const double sr = sin(angle);
    const double cr = cos(angle);

    angle = roll * 0.5;
    const double sp = sin(angle);
    const double cp = cos(angle);

    angle = yaw * 0.5;
    const double sy = sin(angle);
    const double cy = cos(angle);

    const double cpcy = cp * cy;
    const double spcy = sp * cy;
    const double cpsy = cp * sy;
    const double spsy = sp * sy;

    q.x = (double)(sr * cpcy - cr * spsy);
    q.y = (double)(cr * spcy + sr * cpsy);
    q.z = (double)(cr * cpsy - sr * spcy);
    q.w = (double)(cr * cpcy + sr * spsy);

    return q.Normalize();
    */

}

inline int signof(double a) { return (a == 0) ? 0 : (a<0 ? -1 : 1); }
/*
	std::cout << copysign(1.0,-4.4) << std::endl;
    std::cout << copysign(1.0,4.4) << std::endl;
    std::cout << copysign(1.0,0.0) << std::endl;
*/


Vector3 QuatToEuler(Quaternion q1)
{
	// http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler

	double pitch, roll, yaw;
	
    double sqw = q1.w*q1.w;
    double sqx = q1.x*q1.x;
    double sqy = q1.y*q1.y;
    double sqz = q1.z*q1.z;
	double unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
	double test = q1.x*q1.y + q1.z*q1.w;

	// singularity at north pole:
	if (test > 0.499*unit) {
		yaw = 2 * atan2(q1.x,q1.w);
		pitch = M_PI/2;
		roll = 0;
	}

	// singularity at south pole:
	else if (test < -0.499*unit) {
		yaw = -2 * atan2(q1.x,q1.w);
		pitch = -M_PI/2;
		roll = 0;
	}

	// normal case:
	else {
		yaw = atan2(2*q1.y*q1.w-2*q1.x*q1.z , sqx - sqy - sqz + sqw);
		pitch = asin(2*test/unit);
		roll = atan2(2*q1.x*q1.w-2*q1.y*q1.z , -sqx + sqy - sqz + sqw);
	}
	return Vector3(pitch,roll,yaw);
}

Vector3 sphericalToCartesian(Vector3 aed)
{
    // http://en.wikipedia.org/wiki/Spherical_coordinate_system
    double a = aed.x;
    double e = aed.y-M_PI/2;
    double d = aed.z;
    double x = d * sin(e) * cos(a);
    double y = d * sin(e) * sin(a);
    double z = d * cos(e);
    return Vector3(x,y,z);
}

Vector3 cartesianToSpherical(Vector3 v)
{
	// http://en.wikipedia.org/wiki/Spherical_coordinate_system
	// but modified, since ground ref is the XY plane

	double azim, elev, dist;
	
	dist = v.Mag();
	azim = atan2(v.y,v.x) - M_PI/2;

	// put in range of [-pi,pi]
	if (azim > M_PI)
		azim -= 2 * M_PI;
    else if (azim < -M_PI)
		azim += 2 * M_PI;
	
	if (dist > 0.000001)
		elev = M_PI/2 - acos(v.z/dist);
	else
		elev = 0.0;
	
	return Vector3(azim, elev, dist);
}

	
Vector3 rotateAroundAxis(Vector3 v, Vector3 axis, double angle)
{
    double c, s, t;
    c = cos(angle);
    s = sin(angle);
    t = 1 - c;

    axis.Normalize();
    double w1 = (t * axis.x * axis.x +          c  ) * v.x
              + (t * axis.x * axis.y + s * axis.z) * v.y
              + (t * axis.x * axis.z - s * axis.y) * v.z;

    double w2 = (t * axis.x * axis.y - s * axis.z) * v.x
              + (t * axis.y * axis.y +          c) * v.y
              + (t * axis.y * axis.z + s * axis.x) * v.z;

    double w3 = (t * axis.x * axis.z + s * axis.y) * v.x
              + (t * axis.y * axis.z - s * axis.x) * v.y
              + (t * axis.z * axis.z +          c) * v.z;

    Vector3 w = Vector3(w1,w2,w3);
    w.Normalize(); // why normalize?

    return w;
}

// Computes the homography between 4 points:

Matrix3 ComputeHomography(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4)
{
    Matrix3 result;
    Matrix3 V;

    double a, b, c, d, e, f;

    V._11 = p1.x;    V._12 = p1.y;
    V._13 = p2.x;    V._21 = p2.y;
    V._22 = p3.x;    V._23 = p3.y;
    V._31 = p4.x;    V._32 = p4.y;
    V._33 = 0.0;

    a = V._21 + V._23 - V._12 - V._32;
    b = V._13 + V._22 - V._11 - V._31;
    c = V._31 - V._22;
    d = V._31 - V._13;
    e = V._32 - V._23;
    f = V._32 - V._21;

    result._31 = ( (a*c) - (b*e) )/( (f*c) - (d*e) );
    result._32 = ( (a*d) - (b*f) )/( (d*e) - (f*c) );
    result._33 = 1.0;

    result._11 = V._13*(result._31+1.0)-V._11;
    result._12 = V._22*(result._32+1.0)-V._11;
    result._13 =                        V._11;

    result._21 = V._21*(result._31+1.0)-V._12;
    result._22 = V._23*(result._32+1.0)-V._12;
    result._23=                         V._12;

    return result;
}

} // end namespace spatosc

