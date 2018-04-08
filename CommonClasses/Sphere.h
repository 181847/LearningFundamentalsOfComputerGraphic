#pragma once
#include "Surface.h"
#include "HitRecord.h"
#include "ColorTemplate.h"
#include "Material.h"

namespace CommonClass
{

class Sphere :
	public Surface
{
public:
    /*!
        \brief the center of the sphere
    */
	vector3		m_center;

    /*!
        \brief the radius of the sphere
    */
	Types::F32	m_radius;

    /*!
        \brief material of this sphere.
    */
    Material    m_material;

public:
	Sphere(const vector3& center, const Types::F32 radius);
	~Sphere();

	/*!
		\brief try ray hit on this sphere
		\param ray hit ray
		\param t0 min T value
		\param t1 max T value
		\param pHitRec return the hit detail
	*/
	virtual bool Hit(const Ray& ray, const Types::F32 t0, const Types::F32 t1, HitRecord* pHitRec) override;

	/*!
		\brief get bounding box of the sphere.
	*/
	virtual AABB BoundingBox() override;
};

} // namespace CommonClass

