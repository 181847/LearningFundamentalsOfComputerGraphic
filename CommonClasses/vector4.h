#pragma once
#include "CommonTypes.h"

namespace CommonClass
{

class vector3;

/*!
    \brief homogeneous vector with four components
    we assume the vector4 is column vector
*/
class vector4
{
public:
    union
    {
        struct
        {
            Types::F32 m_x;
            Types::F32 m_y;
            Types::F32 m_z;
            Types::F32 m_w;
        };
        Types::F32 m_arr[4];
    };
public:
    /*!
        \brief construct a homogeneous vector, default to be (0, 0, 0, 1)
    */
    explicit vector4(const Types::F32 x = 0.0f, const Types::F32 y = 0.0f, const Types::F32 z = 0.0f, const Types::F32 w = 1.0f);
    vector4& operator = (const vector4& v);
    ~vector4();

    /*!
        \brief convert xyz part to vector3
    */
    vector3 ToVector3() const;
};
static_assert(sizeof(vector4) == 4 * sizeof(Types::F32), "sizeof vector4 should be four times of F32");

/*!
    \brief ignore the w component, which will stay same as the first vector4's w component
*/
vector4  operator +  (const vector4&    v1,     const vector4&      v2      );
vector4  operator -  (const vector4&    v1,     const vector4&      v2      );
vector4  operator *  (const vector4&    v1,     const vector4&      v2      );
vector4  operator /  (const vector4&    v1,     const vector4&      v2      );
vector4  operator *  (const Types::F32  s,      const vector4&      v       );
vector4  operator *  (const vector4&    v,      const Types::F32    s       );
vector4  operator /  (const vector4&    v,      const Types::F32    s       );
vector4& operator += (vector4&          v1,     const vector4&      v2      );
vector4& operator -= (vector4&          v1,     const vector4&      v2      );
vector4& operator *= (vector4&          v1,     const vector4&      v2      );
vector4& operator /= (vector4&          v1,     const vector4&      v2      );
vector4& operator *= (vector4&          v,      Types::F32          s       );
vector4& operator /= (vector4&          v,      Types::F32          s       );
bool     operator != (const vector4&    v1,     const vector4&      v2      );
bool     operator == (const vector4&    v1,     const vector4&      v2      );

/*!
    \brief whether two vector4 are almost equal
    \param v1,v2 the two vector4 to compare
    \param tolerance the avaiable interval in which we think two float is almost equal
*/
bool AlmostEqual(const vector4& v1, const vector4& v2, Types::F32 tolerance = 1e-7f);

} // namespace CommonClass
