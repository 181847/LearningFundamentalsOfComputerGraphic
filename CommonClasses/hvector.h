#pragma once
#include "CommonTypes.h"

namespace CommonClass
{

class vector3;

/*!
    \brief homogeneous vector with four components
    we assume the hvector is column vector
*/
class hvector
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
    explicit hvector(const Types::F32 x = 0.0f, const Types::F32 y = 0.0f, const Types::F32 z = 0.0f, const Types::F32 w = 1.0f);
    hvector& operator = (const hvector& v);
    ~hvector();

    /*!
        \brief convert xyz part to vector3
    */
    vector3 ToVector3() const;
};
static_assert(sizeof(hvector) == 4 * sizeof(Types::F32), "sizeof hvector should be four times of F32");

/*!
    \brief ignore the w component, which will stay same as the first hvector's w component
*/
hvector  operator +  (const hvector&    v1,     const hvector&      v2      );
hvector  operator -  (const hvector&    v1,     const hvector&      v2      );
hvector  operator *  (const hvector&    v1,     const hvector&      v2      );
hvector  operator /  (const hvector&    v1,     const hvector&      v2      );
hvector  operator *  (const Types::F32  s,      const hvector&      v       );
hvector  operator *  (const hvector&    v,      const Types::F32    s       );
hvector  operator /  (const hvector&    v,      const Types::F32    s       );
hvector& operator += (hvector&          v1,     const hvector&      v2      );
hvector& operator -= (hvector&          v1,     const hvector&      v2      );
hvector& operator *= (hvector&          v1,     const hvector&      v2      );
hvector& operator /= (hvector&          v1,     const hvector&      v2      );
hvector& operator *= (hvector&          v,      Types::F32          s       );
hvector& operator /= (hvector&          v,      Types::F32          s       );
bool     operator != (const hvector&    v1,     const hvector&      v2      );
bool     operator == (const hvector&    v1,     const hvector&      v2      );

/*!
    \brief prevent any other type do the comparison with hvector
    In some situation, <hvector == float> will be transfered to <hvector == hvector(float, ...)>,
    next two template can prevent that situation.
*/
template<typename T>
bool operator != (const hvector& v, const T&) = delete;
template<typename T>
bool operator == (const hvector& v, const T&) = delete;

/*!
    \brief whether two hvector are almost equal
    \param v1,v2 the two hvector to compare
    \param tolerance the avaiable interval in which we think two float is almost equal
*/
bool AlmostEqual(const hvector& v1, const hvector& v2, Types::F32 tolerance = 1e-7f);

} // namespace CommonClass
