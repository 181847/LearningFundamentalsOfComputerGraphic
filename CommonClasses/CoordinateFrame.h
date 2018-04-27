#pragma once
#include "CommonTypes.h"
#include "hvector.h"
#include "vector3.h"
#include "Transform.h"

namespace CommonClass
{

/*!
    \brief a Cooridinate is used to represent a frame in a space
    the coordinate frame have three axis(u/v/w),
    each axis is orthorgonal to others.
*/
class CoordinateFrame
{

public:
    vector3 m_u, m_v, m_w, m_e;

public:
    /*!
        \brief construct a coordinate frame with two axis and a position
        \param u the first axis which correspond to x. Don;t have to be normalized.
        \param v the second axis which correspond to y. Dont't have to be normalized.
        \param e the position of th coordinate frame
        the u and v of the parameter don't need to orthorgonal to each other. 
        Warning, we don't need a 'w' axis, because we can derviate it from u and v.
        
    */
    CoordinateFrame(const vector3 u, const vector3 v, const vector3 e);
    ~CoordinateFrame();

    /*!
        \brief get the transform which will convert local position(in this coordinate) to world location.
    */
    Transform GetTransformLocalToWorld();

    /*!
        \brief  get the transform which will convert world position to local position(in this coordinate.
    */
    Transform GetTransformWorldToLocal();
};

} // namespace CommonClass
