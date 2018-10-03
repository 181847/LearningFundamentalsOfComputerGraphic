#pragma once
#include "vector3.h"
#include "ColorTemplate.h"
#include "Material.h"
#include <memory>

namespace CommonClass
{

class HitRecord
{
public:
    /*!
        \brief hit distance from ray origin.
    */
    Types::F32    m_hitT;

    /*!
        \brief the hit point location.
    */
    vector3        m_hitPoint;

    /*!
        \brief the normal of the hit point.
    */
    vector3        m_normal;

    /*!
        \brief WARNING!! This property is not correct for all hit object, just an experimental property of only triangle hit test, is the hit position is the back of the surface?
    */
    bool        m_isBackFace = false;

    /*!
        \brief the material of hit point.
    */
    std::shared_ptr<Material> m_material;

public:
};

} // namespace CommonClass
