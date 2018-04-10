#include "vector3.h"
#include <MyTools\MathTool.h>

namespace CommonClass
{

vector3::vector3()
	:m_x(0.0f), m_y(0.0f), m_z(0.0f)
{
	// empty
}

vector3::vector3(const Types::F32 & x, const Types::F32 & y, const Types::F32 & z)
	: m_x(x), m_y(y), m_z(z)
{
	// empty
}


vector3::~vector3()
{
	// empty
}

vector3 operator+(const vector3 & a, const vector3 & b)
{
	return vector3(a.m_x + b.m_x, a.m_y + b.m_y, a.m_z + b.m_z);
}

vector3 operator-(const vector3 & a, const vector3 & b)
{
	return vector3(a.m_x - b.m_x, a.m_y - b.m_y, a.m_z - b.m_z);
}

bool operator==(const vector3 & a, const vector3 & b)
{
	return a.m_x == b.m_x
		&& a.m_y == b.m_y
		&& a.m_z == b.m_z;
}

bool operator!=(const vector3 & a, const vector3 & b)
{
	return a.m_x != b.m_x
		|| a.m_y != b.m_y
		|| a.m_z != b.m_z;
}

Types::F32 operator*(const vector3 & a, const vector3 & b)
{
	return (a.m_x * b.m_x + a.m_y * b.m_y + a.m_z * b.m_z);
}

vector3 operator*(const vector3 & a, const Types::F32 bFloat)
{
	return vector3(a.m_x * bFloat, a.m_y * bFloat, a.m_z * bFloat);
}

vector3 operator*(const Types::F32 bFloat, const vector3 & a)
{
	return vector3(a.m_x * bFloat, a.m_y * bFloat, a.m_z * bFloat);
}

vector3 operator-(const vector3 & a)
{
	return vector3(-a.m_x, -a.m_y, -a.m_z);
}

Types::F32 dotProd(const vector3 & a, const vector3 & b)
{
	return (a.m_x * b.m_x + a.m_y * b.m_y + a.m_z * b.m_z);
}

vector3 crossProd(const vector3 & a, const vector3 & b)
{
	return vector3(
		a.m_y * b.m_z - b.m_y * a.m_z,
		b.m_x * a.m_z - a.m_x * b.m_z,
		a.m_x * b.m_y - b.m_x * a.m_y);
}

vector3 Normalize(const vector3 & a)
{
	const Types::F32 squreLen = a.m_x * a.m_x + a.m_y * a.m_y + a.m_z * a.m_z;
	if (squreLen == 0.0f)
	{
		throw std::exception("cannot normalize a zero vector.");
	}
	const Types::F32 reciprocalLen = 1 / (std::sqrtf(squreLen));
	return a * reciprocalLen;
}

vector3 Normalize(const vector3 & a, Types::F32 * const pOutLength)
{
	const Types::F32 squreLen = a.m_x * a.m_x + a.m_y * a.m_y + a.m_z * a.m_z;
	if (squreLen == 0.0f)
	{
		throw std::exception("cannot normalize a zero vector.");
	}
    *pOutLength = std::sqrtf(squreLen);
	const Types::F32 reciprocalLen = 1 / (*pOutLength);
	return a * reciprocalLen;
}

Types::F32 Length(const vector3 & a)
{
	return std::sqrtf(a.m_x * a.m_x + a.m_y * a.m_y + a.m_z * a.m_z);
}

vector3 Reflect(const vector3 & incomeVec, const vector3 & unitNormal)
{
    return incomeVec - (2.0f * (dotProd(incomeVec, unitNormal)) * unitNormal);
}

bool AlmostEqual(const vector3 & a, const vector3 & b, int ulp)
{
	return MathTool::almost_equal(a.m_x, b.m_x, ulp)
		&& MathTool::almost_equal(a.m_y, b.m_y, ulp)
		&& MathTool::almost_equal(a.m_z, b.m_z, ulp);
}

bool AlmostPerpendicular(const vector3 & a, const vector3 & b, const Types::F32 minBound, const Types::F32 maxBound)
{
	Types::F32 dpValue = dotProd(a, b);
	return minBound < dpValue && dpValue < maxBound;
}

bool AlmostIsUnitVector(const vector3 & a, int ulp)
{
	return MathTool::almost_equal(Length(a), 1.0f, ulp);;
}

}// namespace CommonClass
