#pragma once
#include "vector3.h"
#include "Surface.h"
#include "ColorTemplate.h"
#include <array>

namespace CommonClass
{


class Triangle
	:public Surface
{
public:
	std::array<vector3, 3> m_points;

    /*!
        \brief the material of this triangle.
    */
    Material               m_material;

public:
    /*!
        \brief build a triangle by three points
        \param p0-2 three point position
        clockwise is front face.
    */
	Triangle(const vector3& p0, const vector3& p1, const vector3& p2);
	Triangle(std::array<vector3, 3>& threePoints);
	~Triangle();

	virtual bool Hit(const Ray& ray, const Types::F32 t0, const Types::F32 t1, HitRecord* pHitRec) override;
	virtual AABB BoundingBox() override;


};
/*! ensurance, extra 4 bytes stands for the virtual pointer int the Surface*/
static_assert(sizeof(Triangle) == 4 + 3 * sizeof(vector3) + sizeof(Material), "size of Triangle is wrong");

} // namespace CommonClass
