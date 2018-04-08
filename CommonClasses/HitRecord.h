#pragma once
#include "vector3.h"
#include "ColorTemplate.h"

namespace CommonClass
{

class HitRecord
{
public:
    /*!
        \brief hit distance from ray origin.
    */
	Types::F32	m_hitT;

    /*!
        \brief the hit point location.
    */
	vector3		m_hitPoint;

    /*!
        \brief the normal of the hit point.
    */
	vector3		m_normal;

	/*!
		\brief WARNING!! This property is not correct for all hit object, just an experimental property of only triangle hit test, is the hit position is the back of the surface?
	*/
	bool        m_isBackFace = false;

    /*!
        \brief the diffuse coefficient of the hit point.
    */
    RGB         m_kDiffuse;

public:
};

} // namespace CommonClass
