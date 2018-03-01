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

bool AlmostEqual(const vector3 & a, const vector3 & b, int ulp)
{
	return MathTool::almost_equal(a.m_x, b.m_x, ulp)
		&& MathTool::almost_equal(a.m_y, b.m_y, ulp)
		&& MathTool::almost_equal(a.m_z, b.m_z, ulp);
}

}// namespace CommonClass
