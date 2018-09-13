#pragma once
#include "CommonTypes.h"

namespace CommonClass
{

/*! 
    \brief a vector in 2D space
*/
class vector2
{

public:
    union
    {
        struct 
        {
            Types::F32 m_x, m_y;
        };
        Types::F32 m_arr[2];
    };

    /*!
        \brief decide the precision of the almost equal floats
    */
    static const Types::I32 COMPARE_ULP = 8;

public:
    vector2();
    explicit vector2(const Types::F32 & x, const Types::F32 & y);
    ~vector2();
};

vector2    operator + (const vector2&       a, const vector2&       b);
vector2    operator - (const vector2&       a, const vector2&       b);
vector2    operator * (const vector2&       a, const Types::F32&    s);
vector2    operator * (const Types::F32&    s, const vector2&       b);
bool       operator ==(const vector2&       a, const vector2&       b);
bool       operator !=(const vector2&       a, const vector2&       b);
vector2    operator * (const vector2&       a, const vector2&       b);
vector2    operator / (const vector2&       a, const vector2&       b);
vector2    operator / (const vector2&       a, const Types::F32&    s);

/*!
    \brief compute the dot product of two vector
*/
Types::F32 dotProd    (const vector2 & a, const vector2 & b);    // same as operator *

/*!
    \brief two vector are almost equal
    \param a compared vector
    \param b compared vector
    \param ulp the precision of compare, lower and more accurate
*/
bool AlmostEqual(const vector2 & a, const vector2 & b, int ulp = vector2::COMPARE_ULP);

}// namespace CommonClass
