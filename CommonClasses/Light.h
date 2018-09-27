#pragma once
#include "vector3.h"
#include "Ray.h"
#include "ColorTemplate.h"

namespace CommonClass
{

/*!
    \brief this class is a prototype of the light in ray tracing, now it's a point light.
*/
class Light
{
public:
    /*!
        \brief the position of the light
    */
    vector3 m_position;

    /*!
        \brief the diffuse color of the color.
    */
    vector3     m_color;
    
public:
    /*!
        \brief construct a light
        \param position where the light is
        \param diffuse the diffuse color of the light.
    */
    Light(const vector3& position, const vector3& color);
    ~Light();

    /*
        \brief get a normalized vector from the reflect point to the light.
        \param reflectPoint the position of reflection
        \param pOutDistance return the distance between reflectPoint and light.
    */
    vector3 ToMeFrom(const vector3& reflectPoint, Types::F32 * const pOutDistance) const;
};

} // namespace CommonClass
