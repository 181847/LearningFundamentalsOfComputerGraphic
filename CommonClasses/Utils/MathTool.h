#pragma once
#include <algorithm>
#undef min

// this head file define some useful function for math

namespace MathTool
{

const double PI  =3.141592653589793238463;
const float  PI_F=3.14159265358979f;
	
template<typename T>
inline T clamp(const T& target, const T& min, const T& max)
{
	if (target < min)
	{
		return min;
	}
	else if (target > max)
	{
		return max;
	}
	else
	{
		return target;
	}
}

template<typename T>
inline T Lerp(const T& a, const T& b, const float& t)
{
    return (1 - t) * a + t * b;
}

/*!
    \brief if v < 0 return 0, if v > 1 return 1, else return v.
*/
inline float Saturate(const float v)
{
    if (v < 0.0f)       return 0.0f;
    else if (v > 1.0f)  return 1.0f;
    else                return v;
}

inline float Attenuation(const float v, const float falloffStart, const float falloffEnd)
{
    return Saturate((falloffEnd - v) / (falloffEnd - falloffStart));
}

// This function used to compare floating-point values for equality.
// Refer to http://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
almost_equal(T x, T y, int ulp)
{
	// the machine epsilon has to be scaled to the magnitude of the values used
	// and multiplied by the desired precision in ULPs (units in the last place)
	return std::abs(x - y) <= std::numeric_limits<T>::epsilon() * std::abs(x + y) * ulp
		// unless the result is subnormal
		|| std::abs(x - y) < std::numeric_limits<T>::min();
}

inline bool AlmostEqual(float a, float b, float tolerance = 1e-5f)
{
    return std::abs(a - b) < tolerance;
}

}// namespace MathTool
