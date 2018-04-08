#include "Polygon.h"

namespace CommonClass
{


Polygon::Polygon(const vector3 & p0, const vector3 & p1, const vector3 & p2)
	:m_boundingBox(vector3(), vector3()) // a bounding box in the origin
{
	m_points.push_back(p0);
	m_points.push_back(p1);
	m_points.push_back(p2);

	m_normal = Normalize(crossProd(p1 - p0, p2 - p0));

	m_d = -(m_normal * p0);


	if (std::fabsf(m_normal.m_z) > std::fabsf(m_normal.m_x) && std::fabsf(m_normal.m_z) > std::fabsf(m_normal.m_y))
	{
		m_index0 = 0;
		m_index1 = 1;
	}
	else if (std::fabsf(m_normal.m_y) > std::fabsf(m_normal.m_x))
	{
		m_index0 = 0;
		m_index1 = 2;
	}
	else
	{
		m_index0 = 1;
		m_index1 = 2;
	}

	BuildBoundingBox();
}

Polygon::~Polygon()
{
}

void Polygon::AddPoint(const vector3 & p)
{
	vector3 projectP;

	Types::F32 dist = m_normal * p + m_d;

	// TODO: Here may add point without projection.

	projectP = p - dist * m_normal;

	m_points.push_back(projectP);

	BuildBoundingBox();
}

bool Polygon::Hit(const Ray & ray, const Types::F32 t0, const Types::F32 t1, HitRecord * pHitRec)
{
	const Types::F32 t = (-m_d - dotProd(m_normal, ray.m_origin)) / dotProd(m_normal, ray.m_direction);
	
	// reject if out of range.
	if (t < t0 || t1 < t)
	{
		return false;
	}

	vector3 hitPoint = ray.m_origin + t * ray.m_direction;
	/*!
		\brief t_arr1 = hitPoint.m_arr[m_index1]
		in the RTR3rd book about ray-polygon hit pseudocode, it stands for the ty.
	*/
	Types::F32 t_arr1 = hitPoint.m_arr[m_index1];

	unsigned int numPoints = m_points.size();
	bool inside = false;

	vector3* e0 = &(m_points[numPoints - 1]);
	vector3* e1 = nullptr;

	bool y0 = (e0->m_arr[m_index1] >= t_arr1);
	bool y1 = false;

	for (unsigned int i = 0; i < numPoints; ++i)
	{
		e1 = &(m_points[i]);

		y1 = (e1->m_arr[m_index1] >= t_arr1);

		if (y0 != y1)
		{
			bool leftSide = 
				(e1->m_arr[m_index1] - t_arr1) * (e0->m_arr[m_index0] - e1->m_arr[m_index0]) 
				>= (e1->m_arr[m_index0] - hitPoint.m_arr[m_index0]) * (e0->m_arr[m_index1] - e1->m_arr[m_index1]);
			if (leftSide == y1)
			{
				inside = !inside;
			}
		}

		y0 = y1;
		e0 = e1;
	}

	if (inside)
	{
        vector3 e1 = m_points[1] - m_points[0];
        vector3 e2 = m_points[2] - m_points[0];
		pHitRec->m_hitT = t;
        pHitRec->m_normal = Normalize(crossProd(e2, e1));
        pHitRec->m_kDiffuse = m_kDiffuse;
		return true;
	}
	else
	{
		return false;
	}
}

AABB Polygon::BoundingBox()
{
	return m_boundingBox;
}

void Polygon::BuildBoundingBox()
{
	vector3 minPoint(Types::Constant::MAX_F32, Types::Constant::MAX_F32, Types::Constant::MAX_F32),
		maxPoint(Types::Constant::MIN_F32, Types::Constant::MIN_F32, Types::Constant::MIN_F32);

	const unsigned int numPoints = m_points.size();

	// loop for x, y, z
	for (int i = 0; i < 3; ++i)
	{
		// first figure out the min/max with first two points.
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
		}// end if m_points[0]...

		// loop rest (numPoints - 2) points.
		for (unsigned int j = 2; j < numPoints; ++j)
		{
			if (m_points[j].m_arr[i] < minPoint.m_arr[i])
			{
				minPoint.m_arr[i] = m_points[j].m_arr[i];
			}
			else if (m_points[j].m_arr[i] > maxPoint.m_arr[i])
			{
				maxPoint.m_arr[i] = m_points[j].m_arr[i];
			}
		}// end for unsigned int i = 2
	}// end for x, y, z

	m_boundingBox.m_minPoint = minPoint;
	m_boundingBox.m_maxPoint = maxPoint;
}

} // namespace CommonClass
