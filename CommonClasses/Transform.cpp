#include "Transform.h"


namespace CommonClass
{

Transform::Transform()
{
    // NOTICE: vectors are column vector
    m_column[0] = hvector(1.0f, 0.0f, 0.0f, 0.0f);
    m_column[1] = hvector(0.0f, 1.0f, 0.0f, 0.0f);
    m_column[2] = hvector(0.0f, 0.0f, 1.0f, 0.0f);
    m_column[3] = hvector(0.0f, 0.0f, 0.0f, 1.0f);
}


Transform::Transform(
    const Types::F32 m11, const Types::F32 m12, const Types::F32 m13, const Types::F32 m14, 
    const Types::F32 m21, const Types::F32 m22, const Types::F32 m23, const Types::F32 m24, 
    const Types::F32 m31, const Types::F32 m32, const Types::F32 m33, const Types::F32 m34, 
    const Types::F32 m41, const Types::F32 m42, const Types::F32 m43, const Types::F32 m44)
{
    // NOTICE: vectors are column vector

    m_column[0] = hvector(
        m11, 
        m21,
        m31,
        m41);

    m_column[1] = hvector(
        m12,
        m22,
        m32,
        m42);

    m_column[2] = hvector(
        m13,
        m23,
        m33,
        m43);

    m_column[3] = hvector(
        m14,
        m24,
        m34,
        m44);
}

Transform::~Transform()
{
}

Transform Transform::Translation(const Types::F32 x, const Types::F32 y, const Types::F32 z)
{
    return Transform(
        1.0f, 0.0f, 0.0f, x,
        0.0f, 1.0f, 0.0f, y,
        0.0f, 0.0f, 1.0f, z,
        0.0f, 0.0f, 0.0f, 1.0f);
}

Transform Transform::RotationX(const Types::F32 x)
{
    float 
        cosTheta = std::cosf(x),
        sinTheta = std::sinf(x);

    return Transform(
        1.0f, 0.0f,      0.0f,     0.0f,
        0.0f, cosTheta, -sinTheta, 0.0f,
        0.0f, sinTheta,  cosTheta, 0.0f,
        0.0f, 0.0f,      0.0f,     1.0f);
}

Transform Transform::RotationY(const Types::F32 y)
{
    float
        cosTheta = std::cosf(y),
        sinTheta = std::sinf(y);

    return Transform(
        cosTheta,  0.0f, sinTheta, 0.0f,
        0.0f,      1.0f, 0.0f,     0.0f,
        -sinTheta, 0.0f, cosTheta, 0.0f,
        0.0f,      0.0f, 0.0f,     1.0f);
}

Transform Transform::RotationZ(const Types::F32 z)
{
    float
        cosTheta = std::cosf(z),
        sinTheta = std::sinf(z);

    return Transform(
        cosTheta,  -sinTheta, 0.0f,     0.0f,
        sinTheta,  cosTheta,  0.0f,     0.0f,
        0.0f,      0.0f,      1.0f,     0.0f,
        0.0f,      0.0f,      0.0f,     1.0f);
}

Transform Transform::Viewport(const Types::F32 left, const Types::F32 right, const Types::F32 bottom, const Types::F32 top)
{
    return Transform(
        0.5f * (right - left + 1), 0.0f, 0.0f, 0.5f * (right + left),
        0.0f, 0.5f * (top - bottom + 1), 0.0f, 0.5f * (top + bottom),
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f );
}

bool operator==(const Transform & m1, const Transform & m2)
{
    for (unsigned int i = 0; i < 4; ++i)
    {
        for (unsigned int j = 0; j < 4; ++j)
        {
            if (m1.m_column[i].m_arr[j] != m2.m_column[i].m_arr[j])
            {
                return false;
            }
        }
    }
    return true;
}

bool operator!=(const Transform & m1, const Transform & m2)
{
    for (unsigned int i = 0; i < 4; ++i)
    {
        for (unsigned int j = 0; j < 4; ++j)
        {
            if (m1.m_column[i].m_arr[j] != m2.m_column[i].m_arr[j])
            {
                return true;
            }
        }
    }
    return false;
}

hvector operator*(const Transform & m, const hvector & v)
{
    return hvector(
        m.m_column[0].m_arr[0] * v.m_arr[0] + m.m_column[1].m_arr[0] * v.m_arr[1] + m.m_column[2].m_arr[0] * v.m_arr[2] + m.m_column[3].m_arr[0] * v.m_arr[3],
        m.m_column[0].m_arr[1] * v.m_arr[0] + m.m_column[1].m_arr[1] * v.m_arr[1] + m.m_column[2].m_arr[1] * v.m_arr[2] + m.m_column[3].m_arr[1] * v.m_arr[3],
        m.m_column[0].m_arr[2] * v.m_arr[0] + m.m_column[1].m_arr[2] * v.m_arr[1] + m.m_column[2].m_arr[2] * v.m_arr[2] + m.m_column[3].m_arr[2] * v.m_arr[3],
        m.m_column[0].m_arr[3] * v.m_arr[0] + m.m_column[1].m_arr[3] * v.m_arr[1] + m.m_column[2].m_arr[3] * v.m_arr[2] + m.m_column[3].m_arr[3] * v.m_arr[3]
    );
}

} // namespace CommonClass
