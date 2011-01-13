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
 * Vectors, matrices and quaternions.
 */
#ifndef _VMATH_H
#define _VMATH_H

#include <cmath>
#include <cstring>

//#define M_PI 3.14159265358979323846
#define TO_DEGREES (180.0 / M_PI)
#define TO_RADIANS (M_PI / 180.0)

// 340m/s = .340 m/ms
#define SPEED_OF_SOUND 0.340


// Below are a bunch of helper classes/functions for 2D/3D math:
namespace spatosc
{
/**
 * 2D vector
 */
class Vector2 {

public:

    double x, y;

    Vector2() {
        x=0.0f;
        y=0.0f;
    }
    Vector2(const Vector2 &a) {
        x=a.x;
        y=a.y;
    }
    Vector2 (double _x, double _y) {
        x=_x;
        y=_y;
    }
    double Mag() {
        return (double)sqrt(x*x+y*y);
    } 
    double Mag2(){
        return x*x+y*y;
    }
    void Normalize() {
        double mag=Mag();
        double rmag=(mag>0.0f)?(1.0f/mag):0.0f;
        x*=rmag;
        y*=rmag;
    }
    Vector2 Norm() {
        double mag=Mag();
        double rmag=(mag>0.0f)?(1.0f/mag):0.0f;
        return Vector2(x*rmag, y*rmag);
    }
    void operator=(const Vector2 &a) {
        x=a.x;
        y=a.y;
    }
    double operator*(Vector2 &v) {
        return x*v.x+y*v.y; // dot product
    }
    Vector2 operator+(Vector2 v) {
        return Vector2(v.x + x, v.y + y);
    }
    Vector2 operator-(Vector2 v) {
        return Vector2(x - v.x, y - v.y);
    }
    Vector2 operator*(double num) {
        return Vector2(x * num, y * num);
    }
    Vector2 operator/(double num) {
        return Vector2(x / num, y / num);
    }    
};


/**
 * 3D vector
 */
class Vector3 {
    
public:
    
    double x, y, z;

    Vector3() {
        x=0.0f;
        y=0.0f;
        z=0.0f;
    }
    Vector3(const Vector3 &a) {
        x=a.x;
        y=a.y;
        z=a.z;
    }
    Vector3(double _x, double _y, double _z) {
        x=_x;
        y=_y;
        z=_z;
    }
    double Mag() {
        return (double)sqrt(x*x+y*y+z*z);
    }
    double Mag2() {
        return x*x+y*y+z*z;
    }
    void Normalize(){
        double mag=Mag();
        double rmag=(mag>0.0f)?(1.0f/mag):0.0f;
        x*=rmag;
        y*=rmag;
        z*=rmag;
    }
    Vector3 Norm(){
        double mag=Mag();
        double rmag=(mag>0.0f)?(1.0f/mag):0.0f;
        return Vector3(x*rmag, y*rmag, z*rmag);
    }
    void operator=(const Vector3 &a){
        x=a.x;
        y=a.y;
        z=a.z;
    }

    Vector3 operator+(Vector3 v) {
        return Vector3(v.x + x, v.y + y, v.z + z);
    }
    Vector3 operator-(Vector3 v) {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }
    Vector3 operator*(double num) {
        return Vector3(x * num, y * num, z * num);
    }
    Vector3 operator/(double num) {
        return Vector3(x / num, y / num, z / num);
    }
    
    double operator*(Vector3 &v) { // dot product
        return x*v.x+y*v.y+z*v.z; 
    }
    Vector3 operator^ (Vector3 &v) // cross product
    {
        return Vector3(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x);
    }
    
};

/*
inline double operator*(Vector3 &a, Vector3 &b){
    return a.x*b.x+a.y*b.y+a.z*b.z;
}
inline double operator*(Vector2 &a, Vector2 &b){
    return a.x*b.x+a.y*b.y;
}

inline Vector3 operator+(Vector3 &a, Vector3 &b){
    return Vector3(a.x+b.x, a.y+b.y, a.z+b.z);
}
inline Vector2 operator+(Vector2 &a, Vector2 &b){
    return Vector2(a.x+b.x, a.y+b.y);
}

inline Vector3 operator-(Vector3 &a, Vector3 &b){
    return Vector3(a.x-b.x, a.y-b.y, a.z-b.z);
}
inline Vector2 operator-(Vector2 &a, Vector2 &b){
    return Vector2(a.x-b.x, a.y-b.y);
}

inline Vector3 operator*(Vector3 &a, double s){
    return Vector3(a.x*s, a.y*s, a.z*s);
}
inline Vector2 operator*(Vector2 &a, double s){
    return Vector2(a.x*s, a.y*s);
}


inline Vector3 operator%(Vector3 &a, Vector3 &b){
    return Vector3(
        a.y*b.z-a.z*b.y,
        a.z*b.x-a.x*b.z,
        a.x*b.y-a.y*b.x);
}
*/


/**
 * 3x3 matrix
 */
class Matrix3
{
public:
    double _11, _12, _13;
    double _21, _22, _23;
    double _31, _32, _33;
public:
    Matrix3(){}
    Matrix3(Matrix3 &a){ memcpy(this, &a, sizeof(Matrix3)); }
    void operator=(const Matrix3 &a){ memcpy(this, &a, sizeof(Matrix3)); }
};

inline Matrix3 operator*(Matrix3 &a, Matrix3 &b){
    Matrix3 r;
    double *d=(double*)&r._11;
    double *s1=(double*)&a._11;
    double *s2=(double*)&b._11;
    for(int y=0; y<3; y++)
        for(int x=0; x<3; x++)
            *d++=
                s1[y*3+0]*s2[0*3+x]+
                s1[y*3+1]*s2[1*3+x]+
                s1[y*3+2]*s2[2*3+x];
    return r;
}

inline Vector3 operator*(Matrix3 &a, Vector3 &b){
    return Vector3(
        a._11*b.x+a._21*b.y+a._31*b.z,
        a._12*b.x+a._22*b.y+a._32*b.z,
        a._13*b.x+a._23*b.y+a._33*b.z);
}

inline Matrix3 Matrix3Identity(){
    Matrix3 r;
    memset(&r, 0, sizeof(r));
    r._11=r._22=r._33=1.0f;
    return r;
}


/**
 * 4x4 matrix
 */
class Matrix4
{
public:
    double _11, _12, _13, _14;
    double _21, _22, _23, _24;
    double _31, _32, _33, _34;
    double _41, _42, _43, _44;
public:
    Matrix4(){}
    Matrix4(const Matrix4 &a){ memcpy(this, &a, sizeof(Matrix4)); }
    Matrix4 Inverse();
    void operator=(const Matrix4 &a){ memcpy(this, &a, sizeof(Matrix4)); }
};

inline Matrix4 Matrix4::Inverse(){
    // transpose upper 3x3 Matrix4
    Matrix4 r;
    memset(&r, 0, sizeof(r));
    r._11=_11; r._12=_21; r._13=_31;
    r._21=_12; r._22=_22; r._23=_32;
    r._31=_13; r._32=_23; r._33=_33;
    r._41=-(_11*_41+_12*_42+_13*_43);
    r._42=-(_21*_41+_22*_42+_23*_43);
    r._43=-(_31*_41+_32*_42+_33*_43);
    r._44=1.0f;
    return r;
}

inline Matrix4 operator*(Matrix4 &a, Matrix4 &b){
    Matrix4 r;
    double *d=(double*)&r._11;
    double *s1=(double*)&a._11;
    double *s2=(double*)&b._11;
    for(int y=0; y<4; y++)
        for(int x=0; x<4; x++)
            *d++=
                s1[y*4+0]*s2[0*4+x]+
                s1[y*4+1]*s2[1*4+x]+
                s1[y*4+2]*s2[2*4+x]+
                s1[y*4+3]*s2[3*4+x];
    return r;
}

inline Vector3 operator*(Matrix4 &a, Vector3 &b){
    return Vector3(
        a._11*b.x+a._21*b.y+a._31*b.z+a._41,
        a._12*b.x+a._22*b.y+a._32*b.z+a._42,
        a._13*b.x+a._23*b.y+a._33*b.z+a._43);
}

inline Matrix4 Matrix4Identity(){
    Matrix4 r;
    memset(&r, 0, sizeof(r));
    r._11=r._22=r._33=r._44=1.0f;
    return r;
}

inline Matrix4 Matrix4Translation(double x, double y, double z){
    Matrix4 r;
    memset(&r, 0, sizeof(r));
    r._11=r._22=r._33=r._44=1.0f;
    r._41=x;
    r._42=y;
    r._43=z;
    return r;
}

/**
 * A quaternion represents an orientation in the 3D space.
 */
class Quaternion{
public:
    double x, y, z, w;
public:
    Quaternion(){
    }
    Quaternion(const Quaternion &q){
        x=q.x;
        y=q.y;
        z=q.z;
        w=q.w;
    }
    Quaternion(double _x, double _y, double _z, double _w){
        x=_x;
        y=_y;
        z=_z;
        w=_w;
    }
    void ToAxis(Vector3 &axis, double angle){
        Quaternion q=(*this).Norm();
        angle=acosf(q.w)*2.0f /*TO_RADIANS*/;
        double sa=1.0f/sqrtf(1.0f-q.w*q.w);
        axis.x=q.x*sa;
        axis.y=q.y*sa;
        axis.z=q.z*sa;
    }
    Matrix4 ToMatrix4(){
        double x2=x*x;
        double y2=y*y;
        double z2=z*z;
        Matrix4 r=Matrix4Identity();
        r._11=1.0f-2.0f*y2-2.0f*z2;
        r._12=2.0f*x*y+2.0f*z*w;
        r._13=2.0f*x*z-2.0f*y*w;
        r._21=2.0f*x*y-2.0f*z*w;
        r._22=1.0f-2.0f*x2-2.0f*z2;
        r._23=2.0f*y*z+2.0f*x*w;
        r._31=2.0f*x*z+2.0f*y*w;
        r._32=2.0f*y*z-2.0f*x*w;
        r._33=1.0f-2.0f*x2-2.0f*y2;
        return r;
    }
    double Mag(){
        return sqrtf(x*x+y*y+z*z+w*w);
    }
    Quaternion operator*(double s){
        return Quaternion(x*s, y*s, z*s, w*s);
    }
    void operator=(const Quaternion &a){
        memcpy(this, &a, sizeof(Quaternion));
    }
    void operator*=(const double s){
        x*=s;
        y*=s;
        z*=s;
        w*=s;
    }
    
    Vector3 operator*(Vector3 v)
    {
        // nVidia SDK implementation (taken from OSG)
        Vector3 uv, uuv;
        Vector3 qvec(this->x, this->y, this->z);
        uv = qvec ^ v;
        uuv = qvec ^ uv;
        uv = uv * ( 2.0f * this->w );
        uuv = uuv * 2.0f;
        return v + uv + uuv;
    }
    
    void Normalize(){
        (*this)*=1.0f/Mag();
    }
    Quaternion Norm(){
        return (*this)*(1.0f/Mag());
    }
};

inline Quaternion operator*(Quaternion &a, Quaternion &b){
    return Quaternion(
        a.y*b.z-a.z*b.y+a.w*b.x+a.x*b.w,
        a.z*b.x-a.x*b.z+a.w*b.y+a.y*b.w,
        a.x*b.y-a.y*b.x+a.w*b.z+a.z*b.w,
        a.w*b.w-a.x*b.x-a.y*b.y-a.z*b.z).Norm();
}


inline Quaternion QuatFromAxis(Vector3 &axis, double angle){
    double sa=sinf(angle*0.5f*TO_RADIANS);
    double ca=cosf(angle*0.5f*TO_RADIANS);
    return Quaternion(axis.x*sa, axis.y*sa, axis.z*sa, ca).Norm();
}


double distance2(Vector2 v1, Vector2 v2);
Vector2 midpoint2(Vector2 v1, Vector2 v2);
Vector2 centroid2(Vector2 v1, Vector2 v2, Vector2 v3);
Vector2 centroid2(Vector2 v1, Vector2 v2, Vector2 v3, Vector2 v4);
double distance3(Vector3 v1, Vector3 v2);


double computeAngle(double dx, double dy);
double AngleBetweenVectors(Vector3 v1, Vector3 v2);
Quaternion RotationBetweenVectors(Vector3 v1, Vector3 v2);
Quaternion EulerToQuat (Vector3 eulerAngles);
//Quaternion EulerToQuat (double r, double p, double y);
Vector3 QuatToEuler(Quaternion q);
Vector3 sphericalToCartesian(Vector3 aed);
Vector3 rotateAroundAxis(Vector3 v, Vector3 axis, double angle);
Matrix3 ComputeHomography(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4);

};

#endif
