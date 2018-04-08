#include "Triangle.h"
#include <assert.h>

#define REORDER_MIN_TO_MAX(a, b) do {if (a > b) { std::swap(a, b); }} while(0)
#define ASSIGN_IF_LESS(assignTo, mayBeLess) do { if (assignTo > mayBeLess){ assignTo = mayBeLess;} }while(0)
#define ASSIGN_IF_GREATER(assignTo, mayBeGreater) do { if (assignTo < mayBeGreater){ assignTo = mayBeGreater;} }while(0)

namespace CommonClass
{

Triangle::Triangle(const vector3 & p0, const vector3 & p1, const vector3 & p2)
	:m_points({p0, p1, p2})
{
	
}

Triangle::Triangle(std::array<vector3, 3>& threePoints)
	:m_points(threePoints)
{
}

Triangle::~Triangle()
{
}

bool Triangle::Hit(const Ray & ray, const Types::F32 t0, const Types::F32 t1, HitRecord * pHitRec)
{
	vector3 e1 = m_points[1] - m_points[0];
	vector3 e2 = m_points[2] - m_points[0];
	
	vector3 q = crossProd(ray.m_direction, e2);
	Types::F32 a = dotProd(e1, q);

	if (-Types::Constant::EPSILON_F32 < a && a < Types::Constant::EPSILON_F32)
	{
		return false;
	}

	Types::F32 f = 1 / a;
	vector3 s = ray.m_origin - m_points[0];

	Types::F32 u = f * dotProd(s, q);

	if (u < 0.0f)
	{
		return false;
	}

	vector3 r = crossProd(s, e1);
	Types::F32 v = f * dotProd(ray.m_direction, r);

	if (v < 0.0f || u + v > 1.0f)
	{
		return false;
	}

	Types::F32 t = f * dotProd(r, e2);

	if (t < t0 || t1 < t)
	{
		// out of range
		return false;
	}

	pHitRec->m_hitT = t;
    pHitRec->m_normal = Normalize(crossProd(e2, e1));
    pHitRec->m_kDiffuse = m_kDiffuse;
	pHitRec->m_isBackFace = a < 0.0f;
	return true;
}

AABB Triangle::BoundingBox()
{
	//assert(false && "Triangle::BoundingBox not implemented.");
	vector3 minPoint(Types::Constant::MAX_F32, Types::Constant::MAX_F32, Types::Constant::MAX_F32), 
		    maxPoint(Types::Constant::MIN_F32, Types::Constant::MIN_F32, Types::Constant::MIN_F32);

	// loop for x, y, z
	for (int i = 0; i < 3; ++i)
	{
		if (m_points[0].m_arr[i] < m_points[1].m_arr[i])
		{
			minPoint.m_arr[i] = m_points[0].m_arr[i];
			maxPoint.m_arr[i] = m_points[1].m_arr[i];
		}
		else
		{
			// change min max assign order.
			maxPoint.m_arr[i] = m_points[0].m_arr[i];
			minPoint.m_arr[i] = m_points[1].m_arr[i];
		}

		if (m_points[2].m_arr[i] < minPoint.m_arr[i])
		{
			minPoint.m_arr[i] = m_points[2].m_arr[i];
		}
		else if (m_points[2].m_arr[i] > maxPoint.m_arr[i])
		{
			maxPoint.m_arr[i] = m_points[2].m_arr[i];
		}
	}

	return AABB(minPoint, maxPoint);
}

} // namespace CommonClass
