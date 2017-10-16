///////////////////////////////////////////////////////////////////////////////
// Vectors.h
// =========
// 2D/3D/4D vectors
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2007-02-14
// UPDATED: 2013-01-20
//
// Copyright (C) 2007-2013 Song Ho Ahn
///////////////////////////////////////////////////////////////////////////////


#ifndef VECTORS_H_DEF
#define VECTORS_H_DEF

#include <cmath>
#include <iostream>


///////////////////////////////////////////////////////////////////////////////
// 3D vector
///////////////////////////////////////////////////////////////////////////////
struct Vec3f
{
    float x;
    float y;
    float z;

    // ctors
    Vec3f() : x(0), y(0), z(0) {};
    Vec3f(float x, float y, float z) : x(x), y(y), z(z) {};

    // utils functions
    void        set(float x, float y, float z);
    float       length() const;                         //
    float       distance(const Vec3f& vec) const;     // distance between two vectors
    Vec3f&    normalize();                            //
    float       dot(const Vec3f& vec) const;          // dot product
    Vec3f     cross(const Vec3f& vec) const;        // cross product
    bool        equal(const Vec3f& vec, float e) const; // compare with epsilon

    // operators
    Vec3f     operator-() const;                      // unary operator (negate)
    Vec3f     operator+(const Vec3f& rhs) const;    // add rhs
    Vec3f     operator-(const Vec3f& rhs) const;    // subtract rhs
    Vec3f&    operator+=(const Vec3f& rhs);         // add rhs and update this object
    Vec3f&    operator-=(const Vec3f& rhs);         // subtract rhs and update this object
    Vec3f     operator*(const float scale) const;     // scale
    Vec3f     operator*(const Vec3f& rhs) const;    // multiplay each element
    Vec3f&    operator*=(const float scale);          // scale and update this object
    Vec3f&    operator*=(const Vec3f& rhs);         // product each element and update this object
    Vec3f     operator/(const float scale) const;     // inverse scale
    Vec3f&    operator/=(const float scale);          // scale and update this object
    bool        operator==(const Vec3f& rhs) const;   // exact compare, no epsilon
    bool        operator!=(const Vec3f& rhs) const;   // exact compare, no epsilon
    bool        operator<(const Vec3f& rhs) const;    // comparison for sort
    float       operator[](int index) const;            // subscript operator v[0], v[1]
    float&      operator[](int index);                  // subscript operator v[0], v[1]


    friend Vec3f operator*(const float a, const Vec3f vec);
    
    double getAngleBetweenVectors(Vec3f vector);
    Vec3f getClosestPointOnLine(Vec3f vA, Vec3f vB);
    float getPlaneDistance(Vec3f Normal);
    Vec3f projectionOfOther(const Vec3f& other) const;
};

inline float Vec3f:: getPlaneDistance(Vec3f Normal)//clase vector
{
    float distance = 0;									// This variable holds the distance from the plane tot he origin
    
    // Use the plane equation to find the distance (Ax + By + Cz + D = 0)  We want to find D.
    // So, we come up with D = -(Ax + By + Cz)
    // Basically, the negated dot product of the normal of the plane and the point. (More about the dot product in another tutorial)
    distance = -((Normal.x * x) + (Normal.y * y) + (Normal.z * z));
    
    return distance;									// Return the distance
}

////////////////////////////// CLOSEST POINT ON LINE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns the point on the line vA_vB that is closest to the point vPoint
/////
////////////////////////////// CLOSEST POINT ON LINE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

inline Vec3f Vec3f::getClosestPointOnLine(Vec3f vA, Vec3f vB)
{
    // Create the vector from end point vA to our point vPoint.
    Vec3f vVector1 = *this - vA;
    
    // Create a normalized direction vector from end point vA to end point vB
    Vec3f vVector2 = (vB - vA).normalize();
    
    
    // Use the distance formula to find the distance of the line segment (or magnitude)
    float d = vA.distance(vB);
    
    
    // Using the dot product, we project the vVector1 onto the vector vVector2.
    // This essentially gives us the distance from our projected vector from vA.
    float t =vVector2.dot(vVector1);
    
    // If our projected distance from vA, "t", is less than or equal to 0, it must
    // be closest to the end point vA.  We want to return this end point.
    if (t <= 0)
        return vA;
    
    // If our projected distance from vA, "t", is greater than or equal to the magnitude
    // or distance of the line segment, it must be closest to the end point vB.  So, return vB.
    if (t >= d)
        return vB;
    
    // Here we create a vector that is of length t and in the direction of vVector2
    Vec3f vVector3 = vVector2 * t;
    
    // To find the closest point on the line segment, we just add vVector3 to the original
    // end point vA.
    Vec3f vClosestPoint = vA + vVector3;
    
    // Return the closest point on the line segment
    return vClosestPoint;
}



inline double Vec3f::getAngleBetweenVectors(Vec3f vector)
{
    // Get the dot product of the vectors
    double dotProduct = dot(vector);
    
    // Get the product of both of the vectors magnitudes
    double vectorsMagnitude = length() * vector.length() ;
    
    // Get the angle in radians between the 2 vectors
    double angle = acos( dotProduct / vectorsMagnitude );
    
    // Here we make sure that the angle is not a -1.#IND0000000 number, which means indefinate
    if(isnan(angle))
        return 0;
    
    // Return the angle in radians
    return( angle );
}




// fast math routines from Doom3 SDK
inline float invSqrt(float x)
{
    float xhalf = 0.5f * x;
    int i = *(int*)&x;          // get bits for floating value
    i = 0x5f3759df - (i>>1);    // gives initial guess
    x = *(float*)&i;            // convert bits back to float
    x = x * (1.5f - xhalf*x*x); // Newton step
    return x;
}







///////////////////////////////////////////////////////////////////////////////
// inline functions for Vec3f
///////////////////////////////////////////////////////////////////////////////
inline Vec3f Vec3f::operator-() const {
    return Vec3f(-x, -y, -z);
}

inline Vec3f Vec3f::operator+(const Vec3f& rhs) const {
    return Vec3f(x+rhs.x, y+rhs.y, z+rhs.z);
}

inline Vec3f Vec3f::operator-(const Vec3f& rhs) const {
    return Vec3f(x-rhs.x, y-rhs.y, z-rhs.z);
}

inline Vec3f& Vec3f::operator+=(const Vec3f& rhs) {
    x += rhs.x; y += rhs.y; z += rhs.z; return *this;
}

inline Vec3f& Vec3f::operator-=(const Vec3f& rhs) {
    x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this;
}

inline Vec3f Vec3f::operator*(const float a) const {
    return Vec3f(x*a, y*a, z*a);
}

inline Vec3f Vec3f::operator*(const Vec3f& rhs) const {
    return Vec3f(x*rhs.x, y*rhs.y, z*rhs.z);
}

inline Vec3f& Vec3f::operator*=(const float a) {
    x *= a; y *= a; z *= a; return *this;
}

inline Vec3f& Vec3f::operator*=(const Vec3f& rhs) {
    x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this;
}

inline Vec3f Vec3f::operator/(const float a) const {
    return Vec3f(x/a, y/a, z/a);
}

inline Vec3f& Vec3f::operator/=(const float a) {
    x /= a; y /= a; z /= a; return *this;
}

inline bool Vec3f::operator==(const Vec3f& rhs) const {
    return (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
}

inline bool Vec3f::operator!=(const Vec3f& rhs) const {
    return (x != rhs.x) || (y != rhs.y) || (z != rhs.z);
}

inline bool Vec3f::operator<(const Vec3f& rhs) const {
    if(x < rhs.x) return true;
    if(x > rhs.x) return false;
    if(y < rhs.y) return true;
    if(y > rhs.y) return false;
    if(z < rhs.z) return true;
    if(z > rhs.z) return false;
    return false;
}

inline float Vec3f::operator[](int index) const {
    return (&x)[index];
}

inline float& Vec3f::operator[](int index) {
    return (&x)[index];
}

inline void Vec3f::set(float x, float y, float z) {
    this->x = x; this->y = y; this->z = z;
}

inline float Vec3f::length() const {
    return sqrtf(x*x + y*y + z*z);
}

inline float Vec3f::distance(const Vec3f& vec) const {
    return sqrtf((vec.x-x)*(vec.x-x) + (vec.y-y)*(vec.y-y) + (vec.z-z)*(vec.z-z));
}

inline Vec3f& Vec3f::normalize() {
    //@@const float EPSILON = 0.000001f;
    float xxyyzz = x*x + y*y + z*z;
    //@@if(xxyyzz < EPSILON)
    //@@    return *this; // do nothing if it is ~zero vector

    //float invLength = invSqrt(xxyyzz);
    float invLength = 1.0f / sqrtf(xxyyzz);
    x *= invLength;
    y *= invLength;
    z *= invLength;
    return *this;
}

inline float Vec3f::dot(const Vec3f& rhs) const {
    return (x*rhs.x + y*rhs.y + z*rhs.z);
}

inline Vec3f Vec3f::cross(const Vec3f& rhs) const {
    return Vec3f(y*rhs.z - z*rhs.y, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x);
}

inline bool Vec3f::equal(const Vec3f& rhs, float epsilon) const {
    return fabs(x - rhs.x) < epsilon && fabs(y - rhs.y) < epsilon && fabs(z - rhs.z) < epsilon;
}

inline Vec3f operator*(const float a, const Vec3f vec) {
    return Vec3f(a*vec.x, a*vec.y, a*vec.z);
}

// Returns the component of other parallel to this.
inline Vec3f  Vec3f::projectionOfOther(const Vec3f& other) const
{
    double factor = dot(other) / (length()*length());
    Vec3f projection(*this);
    projection*=factor;
    return projection;
}


#endif
