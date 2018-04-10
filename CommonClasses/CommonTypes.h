#pragma once
#include <limits>

namespace Types
{

using F32 = float;
using I32 = int;
using U32 = unsigned int;
using U8  = unsigned char;

namespace Constant
{
	
const Types::F32 EPSILON_F32 = 1e-20f;
const Types::F32 MAX_F32 = std::numeric_limits<Types::F32>::max();
const Types::F32 MIN_F32 = -MAX_F32; // std::numeric_limits<Types::F32>::min();


}

}// namespace Types
