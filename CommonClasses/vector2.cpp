#include "vector2.h"

namespace CommonClass
{

vector2::vector2()
	:m_x(0.0f), m_y(0.0f)
{
}

vector2::vector2(const Types::F32 & x, const Types::F32 & y)
	:m_x(x), m_y(y)
{
}


vector2::~vector2()
{
}

vector2 operator+(const vector2 & a, const vector2 & b)
{
	return vector2(a.m_x + b.m_x, a.m_y + b.m_y);
}

vector2 operator-(const vector2 & a, const vector2 & b)
{
	return vector2(a.m_x - b.m_x, a.m_y - b.m_y);
}

vector2 operator*(const vector2 & a, const vector2 & b)
{
	return vector2(a.m_x * b.m_x, a.m_y * b.m_y);
}

vector2 dotProd(const vector2 & a, const vector2 & b)
{
	return vector2(a.m_x * b.m_x, a.m_y * b.m_y);
}

}// namespace CommonClass
