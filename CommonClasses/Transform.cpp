#include "Transform.h"
#include <array>

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

Transform & Transform::operator=(const Transform & m)
{
    for (unsigned int c = 0; c < 4; ++c)
    {
        this->m_column[c] = m.m_column[c];
    }
    return *this;
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
        0.0f,                   0.0f,                       - recipocalDist,		near * recipocalDist,
        0.0f,                   0.0f,                       0.0f,                   1.0f);
}

Transform Transform::PerspectiveOG(const Types::F32 left, const Types::F32 right, const Types::F32 bottom, const Types::F32 top, const Types::F32 near, const Types::F32 far)
{
    // bigger one minus small one, should result in a positive number.
    const Types::F32 
        RECIPO_WIDTH (1.0f / (right - left)),
        RECIPO_HEIGHT(1.0f / (top - bottom)),
        RECIPO_DIST  (1.0f / (near - far));		// notice that current coordinate system is right hand system, the Z axis point out the screen, and [ far < near < 0 ]

    const Types::F32 absNear = std::abs(near), absFar = std::abs(far);

    // a special version of perspective view transformation matrix.
    //Reference from : <Fundemantals Of Compute Graphics, 3rd> page.155
    // I have modify the matrix a little bit in the third row, in which I flip the sign of last two elements.
	// when you perform perspective divide, the relative position of x/y will remains,
	// but z will be flipped, and the relationship between near and far is [ 0 < near < far ]
    return Transform(
        - 2.0f * near * RECIPO_WIDTH,   0.0f,                           (left + right) * RECIPO_WIDTH,       0.0f,
        0.0f,                          -2.0f * near * RECIPO_HEIGHT,    (bottom + top) * RECIPO_HEIGHT,      0.0f,
        0.0f,                           0.0f,                           -(far + near)  * RECIPO_DIST,        2.0f * far * near * RECIPO_DIST,
        0.0f,                           0.0f,                           -1.0f,                               0.0f
    );

    /*return Transform(
        2.0f * near * RECIPO_WIDTH, 0.0f,                           (left + right) * -RECIPO_WIDTH,     0.0f,
        0.0f,                       2.0f * near * RECIPO_HEIGHT,    (bottom + top) * -RECIPO_HEIGHT,    0.0f,
        0.0f,                       0.0f,                           (far + near) * RECIPO_DIST,         2.0f * far * near * -RECIPO_DIST,
        0.0f,                       0.0f,                           1.0f,                               0.0f
    );*/
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
    const Types::F32 column1 = m.m_column[0].m_arr[0] * v.m_arr[0] + m.m_column[1].m_arr[0] * v.m_arr[1] + m.m_column[2].m_arr[0] * v.m_arr[2] + m.m_column[3].m_arr[0] * v.m_arr[3];
    const Types::F32 column2 = m.m_column[0].m_arr[1] * v.m_arr[0] + m.m_column[1].m_arr[1] * v.m_arr[1] + m.m_column[2].m_arr[1] * v.m_arr[2] + m.m_column[3].m_arr[1] * v.m_arr[3];
    const Types::F32 column3 = m.m_column[0].m_arr[2] * v.m_arr[0] + m.m_column[1].m_arr[2] * v.m_arr[1] + m.m_column[2].m_arr[2] * v.m_arr[2] + m.m_column[3].m_arr[2] * v.m_arr[3];
    const Types::F32 column4 = m.m_column[0].m_arr[3] * v.m_arr[0] + m.m_column[1].m_arr[3] * v.m_arr[1] + m.m_column[2].m_arr[3] * v.m_arr[2] + m.m_column[3].m_arr[3] * v.m_arr[3];

    return hvector(
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
