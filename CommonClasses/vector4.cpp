#include "vector4.h"
#include "vector3.h"
#include "Utils/MathTool.h"

namespace CommonClass
{

const vector4 vector4::RED     = vector4(1.0f, 0.0f, 0.0f, 1.0f);
const vector4 vector4::GREEN   = vector4(0.0f, 1.0f, 0.0f, 1.0f);
const vector4 vector4::BLUE    = vector4(0.0f, 0.0f, 1.0f, 1.0f);
const vector4 vector4::YELLOW  = vector4(1.0f, 1.0f, 0.0f, 1.0f);
const vector4 vector4::CYAN    = vector4(0.0f, 1.0f, 1.0f, 1.0f);
const vector4 vector4::MAGENTA = vector4(1.0f, 0.0f, 1.0f, 1.0f);
const vector4 vector4::WHITE   = vector4(1.0f, 1.0f, 1.0f, 1.0f);
const vector4 vector4::BLACK   = vector4(0.0f, 0.0f, 0.0f, 1.0f);

vector4::vector4(const Types::F32 x, const Types::F32 y, const Types::F32 z, const Types::F32 w)
    :m_x(x), m_y(y), m_z(z), m_w(w)
{
    // empty
}

//vector4 & vector4::operator=(const vector4 & v)
//{
//    m_x = v.m_x;
//    m_y = v.m_y;
//    m_z = v.m_z;
//    m_w = v.m_w;
//    return *this;
//}

vector4::~vector4()
{
    // empty
}

CommonClass::vector3 vector4::ToVector3() const
{
    return vector3(m_x, m_y, m_z);
}

vector4 operator+(const vector4 & v1, const vector4 & v2)
{
    //return vector4(v1.m_x + v2.m_x, v1.m_y + v2.m_y, v1.m_z + v2.m_z, v1.m_w + v2.m_w);
    return vector4(v1.m_x + v2.m_x, v1.m_y + v2.m_y, v1.m_z + v2.m_z, v1.m_w);// , v1.m_w + v2.m_w);
}

vector4 operator-(const vector4 & v1, const vector4 & v2)
{
    //return vector4(v1.m_x - v2.m_x, v1.m_y - v2.m_y, v1.m_z - v2.m_z, v1.m_w - v2.m_w);
    return vector4(v1.m_x - v2.m_x, v1.m_y - v2.m_y, v1.m_z - v2.m_z, v1.m_w);//, v1.m_w - v2.m_w);
}

vector4 operator*(const vector4 & v1, const vector4 & v2)
{
    //return vector4(v1.m_x * v2.m_x, v1.m_y * v2.m_y, v1.m_z * v2.m_z, v1.m_w * v2.m_w);
    return vector4(v1.m_x * v2.m_x, v1.m_y * v2.m_y, v1.m_z * v2.m_z, v1.m_w);// , v1.m_w * v2.m_w);
}

vector4 operator/(const vector4 & v1, const vector4 & v2)
{
    //return vector4(v1.m_x / v2.m_x, v1.m_y / v2.m_y, v1.m_z / v2.m_z, v1.m_w / v2.m_w);
    return vector4(v1.m_x / v2.m_x, v1.m_y / v2.m_y, v1.m_z / v2.m_z, v1.m_w);// , v1.m_w / v2.m_w);
}

vector4 operator*(const Types::F32 s, const vector4 & v)
{
    //return vector4(s * v.m_x, s * v.m_y, s * v.m_z, s * v.m_w);
    return vector4(s * v.m_x, s * v.m_y, s * v.m_z, v.m_w);// , s * v.m_w);
}

vector4 operator*(const vector4 & v, const Types::F32 s)
{
    //return vector4(v.m_x * s, v.m_y * s, v.m_z * s, v.m_w * s);
    return vector4(v.m_x * s, v.m_y * s, v.m_z * s, v.m_w);// , v.m_w * s);
}

vector4 operator/(const vector4 & v, const Types::F32 s)
{
    //return vector4(v.m_x / s, v.m_y / s, v.m_z / s, v.m_w / s);
    return vector4(v.m_x / s, v.m_y / s, v.m_z / s, v.m_w);// , v.m_w / s);
}

vector4 & operator+=(vector4 & v1, const vector4 & v2)
{
    v1.m_x += v2.m_x;
    v1.m_y += v2.m_y;
    v1.m_z += v2.m_z;
    //v1.m_w += v2.m_w;
    return v1;
}

vector4 & operator-=(vector4 & v1, const vector4 & v2)
{
    v1.m_x -= v2.m_x;
    v1.m_y -= v2.m_y;
    v1.m_z -= v2.m_z;
    //v1.m_w -= v2.m_w;
    return v1;
}

vector4 & operator*=(vector4 & v1, const vector4 & v2)
{
    v1.m_x *= v2.m_x;
    v1.m_y *= v2.m_y;
    v1.m_z *= v2.m_z;
    //v1.m_w *= v2.m_w;
    return v1;
}

vector4 & operator/=(vector4 & v1, const vector4 & v2)
{
    v1.m_x /= v2.m_x;
    v1.m_y /= v2.m_y;
    v1.m_z /= v2.m_z;
    //v1.m_w /= v2.m_w;
    return v1;
}

vector4 & operator*=(vector4 & v, Types::F32 s)
{
    v.m_x *= s;
    v.m_y *= s;
    v.m_z *= s;
    //v.m_w *= s;
    return v;
}

vector4 & operator/=(vector4 & v, Types::F32 s)
{
    v.m_x /= s;
    v.m_y /= s;
    v.m_z /= s;
    //v.m_w /= s;
    return v;
}

bool operator!=(const vector4 & v1, const vector4 & v2)
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

bool operator==(const vector4 & v1, const vector4 & v2)
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

bool AlmostEqual(const vector4 & v1, const vector4 & v2, Types::F32 tolerance)
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
