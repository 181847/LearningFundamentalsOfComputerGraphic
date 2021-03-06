#pragma once
#include <limits>
#include <type_traits>

namespace Types
{

using F32 = float;
using I32 = int;
using U32 = unsigned int;
using U8  = unsigned char;
using I32 = int;

/*!
    \brief some constant for use.
*/
namespace Constant
{

/*!
    \brief min value in compile time.
*/
template<int A, int B>
struct MIN
{
    enum { value = A < B ? A : B };
};

/*!
    \brief the constant Pi in float
*/
const float  PI_F  = 3.14159265358979f;
const float  PI2_F = 6.28318530717958f;

/*!
    \brief a positive float number close to zero.
*/
const Types::F32 EPSILON_F32 = 1e-20f;

#undef max
/*!
    \brief positive max float number.
*/
const Types::F32 MAX_F32 = std::numeric_limits<Types::F32>::max();

/*!
    \brief negative min float number.
*/
const Types::F32 MIN_F32 = -MAX_F32; // std::numeric_limits<Types::F32>::min();


}

}// namespace Types
