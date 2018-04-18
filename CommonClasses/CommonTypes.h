#pragma once
#include <limits>

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
    \brief the constant Pi in float
*/
const float  PI_F = 3.14159265358979f;

/*!
    \brief a positive float number close to zero.
*/
const Types::F32 EPSILON_F32 = 1e-20f;

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
