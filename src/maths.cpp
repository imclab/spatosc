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

/**
 * Returns an absolute angle difference between v1 and v2 (with no notion of
 * which is ahead or behind the other). Returned angle is from 0 to PI
 */
double AngleBetweenVectors(Vector3 v1, Vector3 v2)
{
    // normalize vectors
    v1.Normalize();
    v2.Normalize();

    // Get the dot product of the vectors
    double dotProduct = v1 * v2;

    // for acos, the value has to be between -1.0 and 1.0, but due to numerical
    // imprecisions it sometimes comes outside this range
    if (dotProduct > 1.0) dotProduct = 1.0;
    if (dotProduct < -1.0) dotProduct = -1.0;

    // Get the angle in radians between the 2 vectors
    // (should this be -acos ? ie, negative?)
    double angle = acos( dotProduct );

    // Here we make sure that the angle is not a -1.#IND0000000 number,
    // which means indefinite
    if (isNan(angle))
        return 0;

    // Return the angle in radians
    return( angle );
}

/**
 * Returns a signed angle of rotation that describes the rotation from v1 to v2,
 * assuming that one axis is null. 1=X_AXIS, 2=Y_AXIS, 3=Z_AXIS
 */
double AngleBetweenVectors(Vector3 v1, Vector3 v2, int nullAxis)
{
    // normalize vectors
    v1.Normalize();
    v2.Normalize();

    double angle = 0;
    switch (nullAxis)
    {
        case 1: // X_AXIS is ignored (ELEV)
            angle = atan2(v2.z,v2.y) - atan2(v1.z,v1.y);
            break;
        case 2: // Y_AXIS is ignored (ROLL)
            angle = atan2(v2.z,v2.x) - atan2(v1.z,v1.x);
            break;
        case 3: // Z_AXIS is ignored (AZIM)
            angle = atan2(v2.y,v2.x) - atan2(v1.y,v1.x);
            break;
    }

    // angle will be from -90 to 270 for some reason, so convert to -PI,PI
    if (angle>M_PI) angle -= 2 * M_PI;

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

//Quaternion EulerToQuat (double roll, double pitch, double yaw)
Quaternion EulerToQuat (Vector3 v)
{
    double cr, cp, cy, sr, sp, sy, cpcy, spsy;

    // IMPORTANT: note pitch/roll flip for these equations:
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
}

Vector3 QuatToEuler(Quaternion q)
{
    // based on:
    // en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

    double pitch, roll, yaw;

    roll = asin( 2* (q.x*q.z - q.w*q.y) );
    yaw = atan2( 2* (q.x*q.y + q.z*q.w) , 1 - (2* (q.y*q.y + q.z*q.z)) );
    pitch =  atan2( 2* (q.x*q.w + q.y*q.z) , 1 - (2* (q.z*q.z + q.w*q.w)) );

    return Vector3(M_PI-pitch,-roll,yaw);
}

Vector3 sphericalToCartesian(Vector3 aed)
{
    // http://en.wikipedia.org/wiki/Spherical_coordinate_system

    double x = aed.z * sin(aed.y) * cos(aed.x);
    double y = aed.z * sin(aed.y) * sin(aed.x);
    double z = aed.z * cos(aed.y);

    return Vector3(x,y,z);
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
    w.Normalize();

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

