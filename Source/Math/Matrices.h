///////////////////////////////////////////////////////////////////////////////
// Matrice.h
// =========
// NxN Matrix Math classes
//
// All matrices are row major. (OpenGL uses column-major matrix)
// | 0 1 |    | 0 1 2 |    |  0  1  2  3 |
// | 2 3 |    | 3 4 5 |    |  4  5  6  7 |
//            | 6 7 8 |    |  8  9 10 11 |
//                         | 12 13 14 15 |
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2005-06-
// UPDATED: 2012-05-29
//
// Copyright (C) 2005 Song Ho Ahn
///////////////////////////////////////////////////////////////////////////////

#ifndef MATH_MATRICES_H
#define MATH_MATRICES_H

#include "Vectors.h"


///////////////////////////////////////////////////////////////////////////
// 4x4 matrix
///////////////////////////////////////////////////////////////////////////
class Matrix4
{
public:
    // constructors
    Matrix4();  // init with identity
    Matrix4(const float src[16]);
    Matrix4(float xx, float xy, float xz, float xw,
            float yx, float yy, float yz, float yw,
            float zx, float zy, float zz, float zw,
            float wx, float wy, float wz, float ww);

    void        set(const float src[16]);
    void        set(float xx, float xy, float xz, float xw,
                    float yx, float yy, float yz, float yw,
                    float zx, float zy, float zz, float zw,
                    float wx, float wy, float wz, float ww);
    void        setRow(int index, const float row[4]);
    void        setRow(int index, const Vec3f& v);
    void        setColumn(int index, const float col[4]);
    void        setColumn(int index, const Vec3f& v);

    const float* get() const;
    const float* getTranspose();                        // return transposed matrix
    float        getDeterminant();

    Matrix4&    identity();
    Matrix4&    transpose();                            // transpose itself and return reference
    Matrix4&    invert();                               // check best inverse method before inverse
    Matrix4&    invertEuclidean();                      // inverse of Euclidean transform matrix
    Matrix4&    invertProjective();                     // inverse of projective matrix using partitioning
    Matrix4&    invertGeneral();                        // inverse of generic matrix

    // transform matrix
    Matrix4&    translate(float x, float y, float z);   // translation by (x,y,z)
    Matrix4&    translate(const Vec3f& v);            //
    Matrix4&    rotate(float angle, const Vec3f& axis); // rotate angle(degree) along the given axix
    Matrix4&    rotate(float angle, float x, float y, float z);
    Matrix4&    rotateX(float angle);                   // rotate on X-axis with degree
    Matrix4&    rotateY(float angle);                   // rotate on Y-axis with degree
    Matrix4&    rotateZ(float angle);                   // rotate on Z-axis with degree
    Matrix4&    scale(float scale);                     // uniform scale
    Matrix4&    scale(float sx, float sy, float sz);    // scale by (sx, sy, sz) on each axis

    // operators
    Matrix4     operator+(const Matrix4& rhs) const;    // add rhs
    Matrix4     operator-(const Matrix4& rhs) const;    // subtract rhs
    Matrix4&    operator+=(const Matrix4& rhs);         // add rhs and update this object
    Matrix4&    operator-=(const Matrix4& rhs);         // subtract rhs and update this object
    Vec3f     operator*(const Vec3f& rhs) const;    // multiplication: v' = M * v
    Matrix4     operator*(const Matrix4& rhs) const;    // multiplication: M3 = M1 * M2
    Matrix4&    operator*=(const Matrix4& rhs);         // multiplication: M1' = M1 * M2
    bool        operator==(const Matrix4& rhs) const;   // exact compare, no epsilon
    bool        operator!=(const Matrix4& rhs) const;   // exact compare, no epsilon
    float       operator[](int index) const;            // subscript operator v[0], v[1]
    float&      operator[](int index);                  // subscript operator v[0], v[1]

    friend Matrix4 operator-(const Matrix4& m);                     // unary operator (-)
    friend Matrix4 operator*(float scalar, const Matrix4& m);       // pre-multiplication
    friend Vec3f operator*(const Vec3f& vec, const Matrix4& m); // pre-multiplication

protected:

private:
    float       getCofactor(float m0, float m1, float m2,
                            float m3, float m4, float m5,
                            float m6, float m7, float m8);

    float m[16];
    float tm[16];                                       // transpose m

};



///////////////////////////////////////////////////////////////////////////
// inline functions for Matrix4
///////////////////////////////////////////////////////////////////////////
inline Matrix4::Matrix4()
{
    // initially identity matrix
    identity();
}



inline Matrix4::Matrix4(const float src[16])
{
    set(src);
}



inline Matrix4::Matrix4(float xx, float xy, float xz, float xw,
                        float yx, float yy, float yz, float yw,
                        float zx, float zy, float zz, float zw,
                        float wx, float wy, float wz, float ww)
{
    set(xx, xy, xz, xw,  yx, yy, yz, yw,  zx, zy, zz, zw,  wx, wy, wz, ww);
}



inline void Matrix4::set(const float src[16])
{
    m[0] = src[0];  m[1] = src[1];  m[2] = src[2];  m[3] = src[3];
    m[4] = src[4];  m[5] = src[5];  m[6] = src[6];  m[7] = src[7];
    m[8] = src[8];  m[9] = src[9];  m[10]= src[10]; m[11]= src[11];
    m[12]= src[12]; m[13]= src[13]; m[14]= src[14]; m[15]= src[15];
}



inline void Matrix4::set(float xx, float xy, float xz, float xw,
                         float yx, float yy, float yz, float yw,
                         float zx, float zy, float zz, float zw,
                         float wx, float wy, float wz, float ww)
{
    m[0] = xx;  m[1] = xy;  m[2] = xz;  m[3] = xw;
    m[4] = yx;  m[5] = yy;  m[6] = yz;  m[7] = yw;
    m[8] = zx;  m[9] = zy;  m[10]= zz;  m[11]= zw;
    m[12]= wx;  m[13]= wy;  m[14]= wz;  m[15]= ww;
}



inline void Matrix4::setRow(int index, const float row[4])
{
    m[index*4] = row[0];  m[index*4 + 1] = row[1];  m[index*4 + 2] = row[2];  m[index*4 + 3] = row[3];
}

inline void Matrix4::setRow(int index, const Vec3f& v)
{
    m[index*4] = v.x;  m[index*4 + 1] = v.y;  m[index*4 + 2] = v.z;
}



inline void Matrix4::setColumn(int index, const float col[4])
{
    m[index] = col[0];  m[index + 4] = col[1];  m[index + 8] = col[2];  m[index + 12] = col[3];
}


inline void Matrix4::setColumn(int index, const Vec3f& v)
{
    m[index] = v.x;  m[index + 4] = v.y;  m[index + 8] = v.z;
}



inline const float* Matrix4::get() const
{
    return m;
}



inline const float* Matrix4::getTranspose()
{
    tm[0] = m[0];   tm[1] = m[4];   tm[2] = m[8];   tm[3] = m[12];
    tm[4] = m[1];   tm[5] = m[5];   tm[6] = m[9];   tm[7] = m[13];
    tm[8] = m[2];   tm[9] = m[6];   tm[10]= m[10];  tm[11]= m[14];
    tm[12]= m[3];   tm[13]= m[7];   tm[14]= m[11];  tm[15]= m[15];
    return tm;
}



inline Matrix4& Matrix4::identity()
{
    m[0] = m[5] = m[10] = m[15] = 1.0f;
    m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11] = m[12] = m[13] = m[14] = 0.0f;
    return *this;
}



inline Matrix4 Matrix4::operator+(const Matrix4& rhs) const
{
    return Matrix4(m[0]+rhs[0],   m[1]+rhs[1],   m[2]+rhs[2],   m[3]+rhs[3],
                   m[4]+rhs[4],   m[5]+rhs[5],   m[6]+rhs[6],   m[7]+rhs[7],
                   m[8]+rhs[8],   m[9]+rhs[9],   m[10]+rhs[10], m[11]+rhs[11],
                   m[12]+rhs[12], m[13]+rhs[13], m[14]+rhs[14], m[15]+rhs[15]);
}



inline Matrix4 Matrix4::operator-(const Matrix4& rhs) const
{
    return Matrix4(m[0]-rhs[0],   m[1]-rhs[1],   m[2]-rhs[2],   m[3]-rhs[3],
                   m[4]-rhs[4],   m[5]-rhs[5],   m[6]-rhs[6],   m[7]-rhs[7],
                   m[8]-rhs[8],   m[9]-rhs[9],   m[10]-rhs[10], m[11]-rhs[11],
                   m[12]-rhs[12], m[13]-rhs[13], m[14]-rhs[14], m[15]-rhs[15]);
}



inline Matrix4& Matrix4::operator+=(const Matrix4& rhs)
{
    m[0] += rhs[0];    m[1] += rhs[1];    m[2] += rhs[2];    m[3] += rhs[3];
    m[4] += rhs[4];    m[5] += rhs[5];    m[6] += rhs[6];    m[7] += rhs[7];
    m[8] += rhs[8];    m[9] += rhs[9];    m[10] += rhs[10];  m[11] += rhs[11];
    m[12] += rhs[12];  m[13] += rhs[13];  m[14] += rhs[14];  m[15] += rhs[15];
    return *this;
}



inline Matrix4& Matrix4::operator-=(const Matrix4& rhs)
{
    m[0] -= rhs[0];    m[1] -= rhs[1];    m[2] -= rhs[2];    m[3] -= rhs[3];
    m[4] -= rhs[4];    m[5] -= rhs[5];    m[6] -= rhs[6];    m[7] -= rhs[7];
    m[8] -= rhs[8];    m[9] -= rhs[9];    m[10] -= rhs[10];  m[11] -= rhs[11];
    m[12] -= rhs[12];  m[13] -= rhs[13];  m[14] -= rhs[14];  m[15] -= rhs[15];
    return *this;
}


inline Vec3f Matrix4::operator*(const Vec3f& rhs) const
{
    return Vec3f(m[0]*rhs.x + m[1]*rhs.y + m[2]*rhs.z,
                   m[4]*rhs.x + m[5]*rhs.y + m[6]*rhs.z,
                   m[8]*rhs.x + m[9]*rhs.y + m[10]*rhs.z);
}



inline Matrix4 Matrix4::operator*(const Matrix4& n) const
{
    return Matrix4(m[0]*n[0]  + m[1]*n[4]  + m[2]*n[8]  + m[3]*n[12],   m[0]*n[1]  + m[1]*n[5]  + m[2]*n[9]  + m[3]*n[13],   m[0]*n[2]  + m[1]*n[6]  + m[2]*n[10]  + m[3]*n[14],   m[0]*n[3]  + m[1]*n[7]  + m[2]*n[11]  + m[3]*n[15],
                   m[4]*n[0]  + m[5]*n[4]  + m[6]*n[8]  + m[7]*n[12],   m[4]*n[1]  + m[5]*n[5]  + m[6]*n[9]  + m[7]*n[13],   m[4]*n[2]  + m[5]*n[6]  + m[6]*n[10]  + m[7]*n[14],   m[4]*n[3]  + m[5]*n[7]  + m[6]*n[11]  + m[7]*n[15],
                   m[8]*n[0]  + m[9]*n[4]  + m[10]*n[8] + m[11]*n[12],  m[8]*n[1]  + m[9]*n[5]  + m[10]*n[9] + m[11]*n[13],  m[8]*n[2]  + m[9]*n[6]  + m[10]*n[10] + m[11]*n[14],  m[8]*n[3]  + m[9]*n[7]  + m[10]*n[11] + m[11]*n[15],
                   m[12]*n[0] + m[13]*n[4] + m[14]*n[8] + m[15]*n[12],  m[12]*n[1] + m[13]*n[5] + m[14]*n[9] + m[15]*n[13],  m[12]*n[2] + m[13]*n[6] + m[14]*n[10] + m[15]*n[14],  m[12]*n[3] + m[13]*n[7] + m[14]*n[11] + m[15]*n[15]);
}



inline Matrix4& Matrix4::operator*=(const Matrix4& rhs)
{
    *this = *this * rhs;
    return *this;
}



inline bool Matrix4::operator==(const Matrix4& n) const
{
    return (m[0] == n[0])   && (m[1] == n[1])   && (m[2] == n[2])   && (m[3] == n[3]) &&
           (m[4] == n[4])   && (m[5] == n[5])   && (m[6] == n[6])   && (m[7] == n[7]) &&
           (m[8] == n[8])   && (m[9] == n[9])   && (m[10] == n[10]) && (m[11] == n[11]) &&
           (m[12] == n[12]) && (m[13] == n[13]) && (m[14] == n[14]) && (m[15] == n[15]);
}



inline bool Matrix4::operator!=(const Matrix4& n) const
{
    return (m[0] != n[0])   || (m[1] != n[1])   || (m[2] != n[2])   || (m[3] != n[3]) ||
           (m[4] != n[4])   || (m[5] != n[5])   || (m[6] != n[6])   || (m[7] != n[7]) ||
           (m[8] != n[8])   || (m[9] != n[9])   || (m[10] != n[10]) || (m[11] != n[11]) ||
           (m[12] != n[12]) || (m[13] != n[13]) || (m[14] != n[14]) || (m[15] != n[15]);
}



inline float Matrix4::operator[](int index) const
{
    return m[index];
}



inline float& Matrix4::operator[](int index)
{
    return m[index];
}



inline Matrix4 operator-(const Matrix4& rhs)
{
    return Matrix4(-rhs[0], -rhs[1], -rhs[2], -rhs[3], -rhs[4], -rhs[5], -rhs[6], -rhs[7], -rhs[8], -rhs[9], -rhs[10], -rhs[11], -rhs[12], -rhs[13], -rhs[14], -rhs[15]);
}



inline Matrix4 operator*(float s, const Matrix4& rhs)
{
    return Matrix4(s*rhs[0], s*rhs[1], s*rhs[2], s*rhs[3], s*rhs[4], s*rhs[5], s*rhs[6], s*rhs[7], s*rhs[8], s*rhs[9], s*rhs[10], s*rhs[11], s*rhs[12], s*rhs[13], s*rhs[14], s*rhs[15]);
}


inline Vec3f operator*(const Vec3f& v, const Matrix4& m)
{
    return Vec3f(v.x*m[0] + v.y*m[4] + v.z*m[8],  v.x*m[1] + v.y*m[5] + v.z*m[9],  v.x*m[2] + v.y*m[6] + v.z*m[10]);
}

// END OF MATRIX4 INLINE //////////////////////////////////////////////////////
#endif
