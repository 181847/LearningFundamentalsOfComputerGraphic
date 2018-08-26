#pragma once
#include "vector3.h"
#include "Transform.h"

namespace CommonClass
{

/*!
    \brief a help class for frame transformations.
*/
class Frame
{
public:
    /*!
        \brief build the frame by u/v axis, the w is automatically calculated.
        \param u serve as the x axis in local frame, it will be normalized
        \param v serve as the y axis in local frame, this value will be fixed, to ensure that it's perpendicular to u axis and normalized.
    */
    explicit Frame(const vector3& origin, const vector3& u, const vector3& v);

    /*!
        \brief set origin only, standard frame as in the world space.
    */
    explicit Frame(const vector3& origin);

    /*!
        \brief same frame as the world space.
    */
    explicit Frame();
    ~Frame();

    /*!
        \brief get the transform to world space.
    */
    Transform LocalToWorld();

    /*!
        \brief get transform from World to Local.
    */
    Transform WorldToLocal();

    /*!
        \brief check the three axises are perpendicular to each other.
    */
    void CheckPerpendicular() const;

    /*!
        \brief rebuild  the frame by axises of u/v, compute w.
    */
    void RebuildByUV();

protected:
    /*!
        \brief the center of the frame in world space
    */
    vector3 m_origin;

    /*!
        \brief x axis in frame, right hand system.
    */
    vector3 m_u;

    /*!
        \brief y axis in frame, right hand system.
    */
    vector3 m_v;

    /*!
        \brief z axis in frame, right hand system.
    */
    vector3 m_w;
};

}// namespace CommonClass
