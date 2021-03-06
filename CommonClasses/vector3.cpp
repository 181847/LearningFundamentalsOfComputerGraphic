#include "vector3.h"
#include "vector4.h"
#include "Utils/MathTool.h"

namespace CommonClass
{
    
const vector3 vector3::ZERO    = vector3(0.0f, 0.0f, 0.0f);
const vector3 vector3::UNIT    = vector3(1.0f, 1.0f, 1.0f);
const vector3 vector3::AXIS_X  = vector3(1.0f, 0.0f, 0.0f);
const vector3 vector3::AXIS_Y  = vector3(0.0f, 1.0f, 0.0f);
const vector3 vector3::AXIS_Z  = vector3(0.0f, 0.0f, 1.0f);
const vector3 vector3::RED     = vector3(1.0f, 0.0f, 0.0f);
const vector3 vector3::GREEN   = vector3(0.0f, 1.0f, 0.0f);
const vector3 vector3::BLUE    = vector3(0.0f, 0.0f, 1.0f);
const vector3 vector3::YELLOW  = vector3(1.0f, 1.0f, 0.0f);
const vector3 vector3::CYAN    = vector3(0.0f, 1.0f, 1.0f);
const vector3 vector3::MAGENTA = vector3(1.0f, 0.0f, 1.0f);
const vector3 vector3::WHITE   = vector3(1.0f, 1.0f, 1.0f);
const vector3 vector3::BLACK   = vector3(0.0f, 0.0f, 0.0f);


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


vector3::vector3(const Types::F32* pArr)
    :m_x(pArr[0]), m_y(pArr[1]), m_z(pArr[2])
{
    // empty
}

vector3::~vector3()
{
    // empty
}

CommonClass::vector4 vector3::Tovector4(const Types::F32& w /*= 1.0f*/) const
{
    return vector4(m_x, m_y, m_z, w);
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

vector3 operator*(const vector3 & a, const vector3 & b)
{
    return vector3(a.m_x * b.m_x, a.m_y * b.m_y, a.m_z * b.m_z);
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

CommonClass::vector3 operator/(const vector3 & a, const vector3 & b)
{
    return vector3(a.m_x / b.m_x, a.m_y / b.m_y, a.m_z / b.m_z);
}

CommonClass::vector3 operator/(const vector3 & a, const Types::F32 bFloat)
{
    return vector3(a.m_x / bFloat, a.m_y / bFloat, a.m_z / bFloat);
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

bool Refract(const vector3& incomeVec, const vector3& unitNormal, const Types::F32& reflectIndex, vector3 * outRefract)
{
    using namespace Types;
    F32 dirDotNormal = dotProd(incomeVec, unitNormal);
    F32 recipIndex = 1.0f / reflectIndex;

    F32 sigma = 1 - (1 - dirDotNormal * dirDotNormal) * recipIndex * recipIndex;
    if (sigma < 0)
    {
        return false;
    }
    else
    {
        *outRefract = recipIndex * (incomeVec - unitNormal * dirDotNormal) - unitNormal * std::sqrt(sigma);
        return true;
    }
}

bool AlmostEqual(const vector3 & a, const vector3 & b, int ulp)
{
    return MathTool::almost_equal(a.m_x, b.m_x, ulp)
        && MathTool::almost_equal(a.m_y, b.m_y, ulp)
        && MathTool::almost_equal(a.m_z, b.m_z, ulp);
}

bool AlmostPerpendicular(const vector3 & a, const vector3 & b, const Types::F32 epsilon)
{
    Types::F32 dpValue = dotProd(a, b);
    return std::abs(dpValue) < std::abs(epsilon);
}

bool AlmostIsUnitVector(const vector3 & a, int ulp)
{
    return MathTool::almost_equal(Length(a), 1.0f, ulp);;
}

}// namespace CommonClass
