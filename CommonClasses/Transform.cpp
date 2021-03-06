#include "Transform.h"
#include <array>

namespace CommonClass
{

Transform::Transform()
{
    // NOTICE: vectors are column vector
    m_column[0] = vector4(1.0f, 0.0f, 0.0f, 0.0f);
    m_column[1] = vector4(0.0f, 1.0f, 0.0f, 0.0f);
    m_column[2] = vector4(0.0f, 0.0f, 1.0f, 0.0f);
    m_column[3] = vector4(0.0f, 0.0f, 0.0f, 1.0f);
}


Transform::Transform(
    const Types::F32 m11, const Types::F32 m12, const Types::F32 m13, const Types::F32 m14, 
    const Types::F32 m21, const Types::F32 m22, const Types::F32 m23, const Types::F32 m24, 
    const Types::F32 m31, const Types::F32 m32, const Types::F32 m33, const Types::F32 m34, 
    const Types::F32 m41, const Types::F32 m42, const Types::F32 m43, const Types::F32 m44)
{
    // NOTICE: vectors are column vector

    m_column[0] = vector4(
        m11, 
        m21,
        m31,
        m41);

    m_column[1] = vector4(
        m12,
        m22,
        m32,
        m42);

    m_column[2] = vector4(
        m13,
        m23,
        m33,
        m43);

    m_column[3] = vector4(
        m14,
        m24,
        m34,
        m44);
}

Transform::~Transform()
{
}

Transform & Transform::operator=(const Transform & m)
{
    for (unsigned int c = 0; c < 4; ++c)
    {
        this->m_column[c] = m.m_column[c];
    }
    return *this;
}

CommonClass::Transform Transform::T()
{
    return Transform(m_11, m_21, m_31, m_41,
                     m_12, m_22, m_32, m_42,
                     m_13, m_23, m_33, m_43,
                     m_14, m_24, m_34, m_44);
}

Transform Transform::Translation(const Types::F32 x, const Types::F32 y, const Types::F32 z)
{
    return Transform(
        1.0f, 0.0f, 0.0f, x,
        0.0f, 1.0f, 0.0f, y,
        0.0f, 0.0f, 1.0f, z,
        0.0f, 0.0f, 0.0f, 1.0f);
}

Transform Transform::InverseTranslation(const Types::F32 x /*= 0.0f*/, const Types::F32 y /*= 0.0f*/, const Types::F32 z /*= 0.0f*/)
{
    return Translation(-x, -y, -z);
}

Transform Transform::Rotation(const Types::F32 yaw, const Types::F32 pitch, const Types::F32 roll)
{
    return RotationY(yaw) * RotationX(pitch) * RotationZ(roll);
}

Transform Transform::InverseRotation(const Types::F32 yaw, const Types::F32 pitch, const Types::F32 roll)
{
    return RotationZ(-roll) * RotationX(-pitch) * RotationY(-yaw);
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

Transform Transform::Scale(const Types::F32 x, const Types::F32 y, const Types::F32 z)
{
    return Transform(
        x,         0.0f,      0.0f,     0.0f,
        0.0f,      y,         0.0f,     0.0f,
        0.0f,      0.0f,      z,        0.0f,
        0.0f,      0.0f,      0.0f,     1.0f);
}

Transform Transform::InverseScale(const Types::F32 x, const Types::F32 y, const Types::F32 z)
{
    return Scale(1.0f / x, 1.0f / y, 1.0f / z);
}

Transform Transform::TRS(const vector3 & t, const vector3 & r, const vector3 & s)
{
    return Translation(t.m_x, t.m_y, t.m_z) * Rotation(r.m_y, r.m_x, r.m_z) * Scale(s.m_x, s.m_y, s.m_z);
}

Transform Transform::InverseTRS(const vector3& t, const vector3& r, const vector3& s)
{
    return InverseScale(s.m_x, s.m_y, s.m_z) * InverseRotation(r.m_y, r.m_x, r.m_z) * InverseTranslation(t.m_x, t.m_y, t.m_z);
}

Transform Transform::Viewport(const Types::F32 left, const Types::F32 right, const Types::F32 bottom, const Types::F32 top)
{
    return Transform(
        0.5f * (right - left + 1), 0.0f, 0.0f, 0.5f * (right + left),
        0.0f, 0.5f * (top - bottom + 1), 0.0f, 0.5f * (top + bottom),
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f );
}

Transform Transform::OrthographicTransOG(const Types::F32 left, const Types::F32 right, const Types::F32 bottom, const Types::F32 top, const Types::F32 near, const Types::F32 far)
{
    const Types::F32 recipocalWidth  = 1.0f / (right - left);
    const Types::F32 recipocalHeight = 1.0f / (top   - bottom);
    const Types::F32 recipocalDist   = 1.0f / (near  - far);

    // map z to [-1(near), 1(far)]
    /*return Transform(
        2.0f * recipocalWidth,  0.0f,                       0.0f,                   (right + left) * - recipocalWidth,
        0.0f,                   2.0f * recipocalHeight,     0.0f,                   (top + bottom) * - recipocalHeight,
        0.0f,                   0.0f,                       2.0f * recipocalDist,   (near + far) * - recipocalDist,
        0.0f,                   0.0f,                       0.0f,                   1.0f);*/

    // map z to [0(near), 1(far)]
    return Transform(
        2.0f * recipocalWidth,  0.0f,                       0.0f,                   (right + left) * - recipocalWidth,
        0.0f,                   2.0f * recipocalHeight,     0.0f,                   (top + bottom) * - recipocalHeight,
        0.0f,                   0.0f,                       - recipocalDist,        near * recipocalDist,
        0.0f,                   0.0f,                       0.0f,                   1.0f);
}

Transform Transform::PerspectiveOG(const Types::F32 left, const Types::F32 right, const Types::F32 bottom, const Types::F32 top, const Types::F32 near, const Types::F32 far)
{
    // by default, this project using right hand system, and axis Z point out of the screen,
    // but the NDC space map Z to 0(near) and 1(far),
    // so here the Z value should be flipped, and mapped to [0, 1] after perspective dividing. 
    const Types::F32 absNear = std::abs(near), absFar = std::abs(far);
    const Types::F32 
        RECIPO_WIDTH (1.0f / (right - left)),
        RECIPO_HEIGHT(1.0f / (top - bottom)),
        RECIPO_DIST  (1.0f / (absNear - absFar));

    return Transform(
        2.0f * absNear * RECIPO_WIDTH,  0.0f,                           (left + right) * RECIPO_WIDTH,       0.0f,
        0.0f,                           2.0f * absNear * RECIPO_HEIGHT, (bottom + top) * RECIPO_HEIGHT,      0.0f,
        0.0f,                           0.0f,                           absFar * RECIPO_DIST,                absFar * absNear * RECIPO_DIST,
        0.0f,                           0.0f,                           -1.0f,                               0.0f
    );
    
    //original version
    /*return Transform(
        2.0f * near * RECIPO_WIDTH, 0.0f,                           (left + right) * -RECIPO_WIDTH,     0.0f,
        0.0f,                       2.0f * near * RECIPO_HEIGHT,    (bottom + top) * -RECIPO_HEIGHT,    0.0f,
        0.0f,                       0.0f,                           (far + near) * RECIPO_DIST,         2.0f * far * near * -RECIPO_DIST,
        0.0f,                       0.0f,                           1.0f,                               0.0f
    );*/
}

Transform Transform::PerspectiveFOV(const Types::F32 fovAngleY, const Types::F32 aspectRatio, const Types::F32 near, const Types::F32 far)
{
    // z will be mapped to [0, 1] after perspective dividing.
    if (fovAngleY < 0.0f
        || aspectRatio < 0.0f
        || near < 0.0f
        || far < 0.0f
        || near > far)
    {
        throw std::exception("perspective matrix of field of view error, arguments invalid.");
    }

    const Types::F32 RECIPO_ASPECT_RATIO   = 1.0f / aspectRatio;
    const Types::F32 RECIPO_TAN_ANGLE      = 1.0f / std::tan(fovAngleY * 0.5f);
    const Types::F32 RECIPO_FAR_NEAR_DIST  = 1.0f / (near - far);

    return Transform(
        RECIPO_ASPECT_RATIO * RECIPO_TAN_ANGLE, 0.0f,                 0.0f,                       0.0f,
        0.0f,                                   RECIPO_TAN_ANGLE,     0.0f,                       0.0f,
        0.0f,                                   0.0f,                 far * RECIPO_FAR_NEAR_DIST, near * far * RECIPO_FAR_NEAR_DIST,
        0.0f,                                   0.0f,                 -1.0f,                      0.0f);
    
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

vector4 operator*(const Transform & m, const vector4 & v)
{
    const Types::F32 column1 = m.m_column[0].m_arr[0] * v.m_arr[0] + m.m_column[1].m_arr[0] * v.m_arr[1] + m.m_column[2].m_arr[0] * v.m_arr[2] + m.m_column[3].m_arr[0] * v.m_arr[3];
    const Types::F32 column2 = m.m_column[0].m_arr[1] * v.m_arr[0] + m.m_column[1].m_arr[1] * v.m_arr[1] + m.m_column[2].m_arr[1] * v.m_arr[2] + m.m_column[3].m_arr[1] * v.m_arr[3];
    const Types::F32 column3 = m.m_column[0].m_arr[2] * v.m_arr[0] + m.m_column[1].m_arr[2] * v.m_arr[1] + m.m_column[2].m_arr[2] * v.m_arr[2] + m.m_column[3].m_arr[2] * v.m_arr[3];
    const Types::F32 column4 = m.m_column[0].m_arr[3] * v.m_arr[0] + m.m_column[1].m_arr[3] * v.m_arr[1] + m.m_column[2].m_arr[3] * v.m_arr[2] + m.m_column[3].m_arr[3] * v.m_arr[3];

    return vector4(
        column1, 
        column2, 
        column3, 
        column4);
}

Transform operator*(const Transform & m1, const Transform & m2)
{
    std::array<std::array<Types::F32, 4>, 4> arr_4_4;

    for (unsigned int row = 0; row < 4; ++row)
    {
        for (unsigned int column = 0; column < 4; ++column)
        {
            Types::F32 component = 0.0f;

            for (unsigned int iter = 0; iter < 4; ++iter)
            {
                component += m1.m_column[iter].m_arr[row] * m2.m_column[column].m_arr[iter];
            }

            arr_4_4[row][column] = component;
        }
    }

    return Transform(
        arr_4_4[0][0], arr_4_4[0][1], arr_4_4[0][2], arr_4_4[0][3],
        arr_4_4[1][0], arr_4_4[1][1], arr_4_4[1][2], arr_4_4[1][3],
        arr_4_4[2][0], arr_4_4[2][1], arr_4_4[2][2], arr_4_4[2][3],
        arr_4_4[3][0], arr_4_4[3][1], arr_4_4[3][2], arr_4_4[3][3]
    );
}

} // namespace CommonClass
