#pragma once
#include "vector3.h"

namespace CommonClass
{

/*!
    \brief Ray is used in the RayTracing, basicly have an origin and an direction.
*/
class Ray
{
public:
    vector3 m_origin;

    /*!
        \brief unit vector for direction
    */
    vector3 m_direction;

public:
    /*!
        \brief construct a default ray, which start at the origin, point at the positive direction of X axis.
    */
    Ray();

    /*!
        \brief construct a ray
        \param origin where the ray start
        \param direction where the ray casts to
        the constructor will automatically normalize the direction vector
    */
    Ray(const vector3& origin, const vector3& direction);
    ~Ray();

    /*!
        \brief set origin location
    */
    void SetOrigin(const vector3& origin);

    /*!
        \brief set direction and normalize it.
    */
    void SetDirection(const vector3& direction);

};

} // namespace CommonClass
