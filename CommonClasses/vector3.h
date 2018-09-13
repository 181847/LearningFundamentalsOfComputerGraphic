#pragma once
#include "CommonTypes.h"

namespace CommonClass
{

class hvector;

/*!
    \brief a vector in 2D space
*/
class vector3
{

public:
    union
    {
        struct
        {
            Types::F32 m_x, m_y, m_z;
        };
        Types::F32 m_arr[3];
    };

    /*!
        \brief decide the precision of the almost equal floats
    */
    static const Types::I32 COMPARE_ULP = 8;
    

    /*!
        \brief some useful constant values.
    */
    static const vector3 ZERO;
    static const vector3 UNIT;
    static const vector3 AXIS_X;
    static const vector3 AXIS_Y;
    static const vector3 AXIS_Z;

public:
    explicit vector3();
    explicit vector3(const Types::F32 & x, const Types::F32 & y, const Types::F32 & z);
    explicit vector3(const Types::F32* pArr);
    ~vector3();

    hvector ToHvector(const Types::F32& w = 1.0f) const;
};
/*! ensurance */
static_assert(sizeof(vector3) == 3 * sizeof(Types::F32), "size of vector3 is wrong");

vector3    operator + (const vector3 & a, const vector3 & b);
vector3    operator - (const vector3 & a, const vector3 & b);
bool       operator ==(const vector3 & a, const vector3 & b);
bool       operator !=(const vector3 & a, const vector3 & b);
vector3       operator * (const vector3 & a, const vector3 & b);
vector3       operator / (const vector3 & a, const vector3 & b);
vector3       operator * (const vector3 & a, const Types::F32 bFloat);
vector3       operator * (const Types::F32 bFloat, const vector3 & a);
vector3       operator / (const vector3 & a, const Types::F32 bFloat);
vector3       operator - (const vector3 & a);

/*!
    \brief compute the dot product of two vector
*/
Types::F32 dotProd    (const vector3 & a, const vector3 & b);    // same as operator *

/*!
    \brief compute the cross product of two 3D vector
*/
vector3    crossProd  (const vector3 & a, const vector3 & b);

/*!
    \brief normalize the vector3
*/
vector3       Normalize(const vector3 & a);

/*!
    \brief normalize the vector3 and return the length of it (before normalize)
    \param a the vector to be normalized
    \param pOutLength point to the return length.
*/
vector3    Normalize(const vector3 & a, Types::F32 * const pOutLength);

/*!
    \brief get length of the vector.
*/
Types::F32 Length(const vector3 & a);

/*!
    \brief reflect a vector by a unit vector
    \param incomeVec the vector to be reflected
    \param unitNormal the normal of the reflect plane.
*/
vector3 Reflect(const vector3& incomeVec, const vector3& unitNormal);

/*!
    \brief two vector are almost equal
    \param a compared vector
    \param b compared vector
    \param ulp the precision of compare, lower and more accurate
*/
bool AlmostEqual(const vector3 & a, const vector3 & b, int ulp = vector3::COMPARE_ULP);

/*!
    \brief check wether two vector are almost perpendicular to each other
    \param a/b two vector to be checked
    \param minBound/maxBound this check use dot product, but the error in the dot product is large, so here we set an acceptable range,
            in which we think 'a' and 'b' are perpendicular.
*/
bool AlmostPerpendicular(const vector3 & a, const vector3 & b, const Types::F32 epsilon = +1e-7f);

/*!
    \brief check whether a vector is almost a unit vector
    \param a the vector to be checked
    \param ulp the precision, larger ulp is, more precise the result is.
*/
bool AlmostIsUnitVector(const vector3 & a, int ulp = 8);

}// namespace CommonClass
