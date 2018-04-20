#include "hvector.h"
#include <MyTools\MathTool.h>

namespace CommonClass
{

hvector::hvector(const Types::F32 x, const Types::F32 y, const Types::F32 z, const Types::F32 w)
    :m_x(x), m_y(y), m_z(z), m_w(w)
{
    // empty
}

hvector & hvector::operator=(const hvector & v)
{
    m_x = v.m_x;
    m_y = v.m_y;
    m_z = v.m_z;
    m_w = v.m_w;
    return *this;
}

hvector::~hvector()
{
    // empty
}

hvector operator+(const hvector & v1, const hvector & v2)
{
    return hvector(v1.m_x + v2.m_x, v1.m_y + v2.m_y, v1.m_z + v2.m_z, v1.m_w + v2.m_w);
}

hvector operator-(const hvector & v1, const hvector & v2)
{
    return hvector(v1.m_x - v2.m_x, v1.m_y - v2.m_y, v1.m_z - v2.m_z, v1.m_w - v2.m_w);
}

hvector operator*(const hvector & v1, const hvector & v2)
{
    return hvector(v1.m_x * v2.m_x, v1.m_y * v2.m_y, v1.m_z * v2.m_z, v1.m_w * v2.m_w);
}

hvector operator/(const hvector & v1, const hvector & v2)
{
    return hvector(v1.m_x / v2.m_x, v1.m_y / v2.m_y, v1.m_z / v2.m_z, v1.m_w / v2.m_w);
}

hvector operator*(const Types::F32 s, const hvector & v)
{
    return hvector(s * v.m_x, s * v.m_y, s * v.m_z, s * v.m_w);
}

hvector operator*(const hvector & v, const Types::F32 s)
{
    return hvector(v.m_x * s, v.m_y * s, v.m_z * s, v.m_w * s);
}

hvector operator/(const hvector & v, const Types::F32 s)
{
    return hvector(v.m_x / s, v.m_y / s, v.m_z / s, v.m_w / s);
}

hvector & operator+=(hvector & v1, const hvector & v2)
{
    v1.m_x += v2.m_x;
    v1.m_y += v2.m_y;
    v1.m_z += v2.m_z;
    v1.m_w += v2.m_w;
    return v1;
}

hvector & operator-=(hvector & v1, const hvector & v2)
{
    v1.m_x -= v2.m_x;
    v1.m_y -= v2.m_y;
    v1.m_z -= v2.m_z;
    v1.m_w -= v2.m_w;
    return v1;
}

hvector & operator*=(hvector & v1, const hvector & v2)
{
    v1.m_x *= v2.m_x;
    v1.m_y *= v2.m_y;
    v1.m_z *= v2.m_z;
    v1.m_w *= v2.m_w;
    return v1;
}

hvector & operator/=(hvector & v1, const hvector & v2)
{
    v1.m_x /= v2.m_x;
    v1.m_y /= v2.m_y;
    v1.m_z /= v2.m_z;
    v1.m_w /= v2.m_w;
    return v1;
}

hvector & operator*=(hvector & v, Types::F32 s)
{
    v.m_x *= s;
    v.m_y *= s;
    v.m_z *= s;
    v.m_w *= s;
    return v;
}

hvector & operator/=(hvector & v, Types::F32 s)
{
    v.m_x /= s;
    v.m_y /= s;
    v.m_z /= s;
    v.m_w /= s;
    return v;
}

bool operator!=(const hvector & v1, const hvector & v2)
{
    for (unsigned int i = 0; i < 4; ++i)
    {
        if (v1.m_arr[i] != v2.m_arr[i])
        {
            return true;
        }
    }
    return false;
}

bool operator==(const hvector & v1, const hvector & v2)
{
    for (unsigned int i = 0; i < 4; ++i)
    {
        if (v1.m_arr[i] != v2.m_arr[i])
        {
            return false;
        }
    }
    return true;
}

bool AlmostEqual(const hvector & v1, const hvector & v2, Types::F32 tolerance)
{
    for (unsigned int i = 0; i < 4; ++i)
    {
        if ( std::abs(v1.m_arr[i] - v2.m_arr[i]) > tolerance)
        {
            return false;
        }
    }
    return true;
}

} // namespace CommonClass
