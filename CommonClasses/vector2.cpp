#include "vector2.h"
#include <math.h>
#include <MyTools\MathTool.h>

namespace CommonClass
{

vector2::vector2()
    :m_x(0.0f), m_y(0.0f)
{
    // empty
}

vector2::vector2(const Types::F32 & x, const Types::F32 & y)
    :m_x(x), m_y(y)
{
    // empty
}


vector2::~vector2()
{
    // empty
}

vector2 operator+(const vector2 & a, const vector2 & b)
{
    return vector2(a.m_x + b.m_x, a.m_y + b.m_y);
}

vector2 operator-(const vector2 & a, const vector2 & b)
{
    return vector2(a.m_x - b.m_x, a.m_y - b.m_y);
}

bool operator==(const vector2 & a, const vector2 & b)
{
    return a.m_x == b.m_x 
        && a.m_y == b.m_y;
}

bool operator!=(const vector2 & a, const vector2 & b)
{
    return a.m_x != b.m_x
        || a.m_y != b.m_y;
}

vector2 operator*(const vector2 & a, const vector2 & b)
{
    return vector2(a.m_x * b.m_x, a.m_y * b.m_y);
}

CommonClass::vector2 operator*(const vector2& a, const Types::F32& s)
{
    return vector2(a.m_x * s, a.m_y * s);
}

CommonClass::vector2 operator*(const Types::F32& s, const vector2& b)
{
    return vector2(b.m_x * s, b.m_y * s);
}

CommonClass::vector2 operator/(const vector2& a, const vector2& b)
{
    return vector2(a.m_x / b.m_x, a.m_y / b.m_y);
}

CommonClass::vector2 operator/(const vector2& a, const Types::F32& s)
{
    return vector2(a.m_x / s, a.m_y / s);
}

Types::F32 dotProd(const vector2 & a, const vector2 & b)
{
    return (a.m_x * b.m_x + a.m_y * b.m_y);
}

bool AlmostEqual(const vector2 & a, const vector2 & b, int ulp)
{
    return MathTool::almost_equal(a.m_x, b.m_x, ulp)
        && MathTool::almost_equal(a.m_y, b.m_y, ulp);
}

}// namespace CommonClass
