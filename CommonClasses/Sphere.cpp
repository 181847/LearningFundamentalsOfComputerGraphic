#include "Sphere.h"
#include <assert.h>
#include <cmath>

namespace CommonClass
{

Sphere::Sphere(const vector3 & center, const Types::F32 radius)
	:m_center(center), m_radius(radius)
{
}

Sphere::~Sphere()
{
}

bool Sphere::Hit(const Ray & ray, const Types::F32 t0, const Types::F32 t1, HitRecord * pHitRec)
{
	assert(pHitRec != nullptr && "argument nullptr error");

	vector3 sphereToRay = ray.m_origin - m_center;
	Types::F32 tempVal = ray.m_direction * sphereToRay;
	Types::F32 delta = 
		tempVal * tempVal -
		(ray.m_direction * ray.m_direction) * (sphereToRay * sphereToRay - m_radius * m_radius);
	
	Types::F32 sqrtDelta = 0.0f;

	if (delta < 0)
	{
		return false;
	}
	else
	{
		sqrtDelta = std::sqrtf(delta);
	}

	Types::F32 headExpr = -(ray.m_direction * sphereToRay);
	Types::F32 recipocalDirectionSquare = 1.0f / (ray.m_direction * ray.m_direction);

	Types::F32 plusDT, minusDT;
	if (delta == 0.0f)
	{
		plusDT = minusDT = headExpr * recipocalDirectionSquare;
	}
	else
	{
		plusDT = (headExpr + sqrtDelta) * recipocalDirectionSquare;
		minusDT = (headExpr - sqrtDelta) * recipocalDirectionSquare;
	}

	Types::F32 finalT;

	int flag = 0;

	if (t0 < plusDT && plusDT < t1)
	{
		flag += 1;
	}
	if (t0 < minusDT && minusDT < t1)
	{
		flag += 2;
	}

	switch (flag)
	{
	case 0:
		return false;

	case 1:
		finalT = plusDT;
		break;

	case 2:
		finalT = minusDT;
		break;

	case 3:
		finalT = std::fminf(plusDT, minusDT);
		break;
	}

	pHitRec->m_hitT = finalT;
	return true;
}

Box Sphere::BoundingBox()
{
	return Box(m_center + vector3(-m_radius, -m_radius, -m_radius), m_center + vector3(m_radius, m_radius, m_radius));
}

} // namespace CommonClass

