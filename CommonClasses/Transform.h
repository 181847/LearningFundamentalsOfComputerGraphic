#pragma once
#include "CommonTypes.h"
#include "hvector.h"

namespace CommonClass
{

/*!
    \brief Transform is a 4x4 matrix support translation/rotation/viewTransformation...
    we assume the vector are column vector, so the translation part is on the right of the matrix like
    [
    1, 0, 0, x
    0, 1, 0, y
    0, 0, 1, z
    0, 0, 0, 1
    ]
*/
class Transform
{
public:
    /*!
        \brief transform are defined by 4 column vectors.
    */
    hvector m_column[4];

public:
    /*!
        \brief construct a unit matrix
    */
    Transform();

    /*!
        \brief construct a matrix by specifying all 16 components,
        NOTICE: matix are consist of column vectors,
        for example, if you want to make up a translation matrix(5, 4, 6),
        you should set m14 = 5, m24 = 4, m34 = 6.
    */
    Transform(
        const Types::F32 m11, const Types::F32 m12, const Types::F32 m13, const Types::F32 m14,
        const Types::F32 m21, const Types::F32 m22, const Types::F32 m23, const Types::F32 m24,
        const Types::F32 m31, const Types::F32 m32, const Types::F32 m33, const Types::F32 m34,
        const Types::F32 m41, const Types::F32 m42, const Types::F32 m43, const Types::F32 m44);

    ~Transform();

    /*!
        \brief construct a translation matrix for x/y/z.
        \param x movement in x direction
        \param y movement in y direction
        \param z movement in z direction
    */
    static Transform Translation(const Types::F32 x = 0.0f, const Types::F32 y = 0.0f, const Types::F32 z = 0.0f);

    /*!
        \brief construct a rotation matrix around axis x
        \param x rotate radians
    */
    static Transform RotationX(const Types::F32 x);
    
    /*!
        \brief construct a rotation matrix around axis y
        \param y rotate radians
    */
    static Transform RotationY(const Types::F32 y);
    
    /*!
        \brief construct a rotation matrix around axis z
        \param z rotate radians
    */
    static Transform RotationZ(const Types::F32 z);

    /*!
        \brief construct a viewport transformation with four window boundry, 
        \param left left boundry of the screen space,     >= 0
        \param right right boundry of the screen space,   <= PIXEL_WIDTH - 1
        \param bottom bottom boundry of the screen space, >= 0
        \param top top boundry of the screen space,       <= PIXEL_HEIGHT - 1
        the NDC space this transformation based on is [-1, +1]^3, x, y
        and in the right hand order.
        the transformed location is bounded to [left - 0.5, right + 0.5]x[bottom - 0.5, top + 0.5]
    */
    static Transform Viewport(const Types::F32 left, const Types::F32 right, const Types::F32 bottom, const Types::F32 top);
};

/*!
    \brief two transform matrix is same only if all coresponding component is same.
*/
bool operator == (const Transform& m1, const Transform& m2);

/*!
    \brief two transform matrix is same only if all coresponding component is same.
*/
bool operator != (const Transform& m1, const Transform& m2);

/*!
    \brief apply matrix transformation on the hvector, return an new instance.
*/
hvector operator * (const Transform& m, const hvector& v);

} // namespace CommonClass
