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

	Types::F32 finalT;

	// next code reference from "Real-Time Rendering 3rd"
	vector3 l = m_center - ray.m_origin;
	Types::F32 s = l * ray.m_direction;
	Types::F32 squareL = l * l;
	Types::F32 squareR = m_radius * m_radius;

	// whether the ray starts outside the sphere, and point outside the sphere.
	if (s < 0 && squareL > squareR)
	{
		return false;
	}

	Types::F32 squareM = squareL - s * s;

	// whether distance from the sphere center to the ray line is longer than sphere radius.
	if (squareM > squareR)
	{
		return false;
	}

	Types::F32 q = std::sqrtf(squareR - squareM);

	if (squareL > squareR)
	{
		// ray starts outside the sphere
		finalT = s - q;
	}
	else
	{
		// ray start inside the sphere
		finalT = s + q;
		// here is some issue about whether allow to capture intersection behind the rayDirection.
		// but here I just ignore the argument t0, and assume that the ray only interset the positive direction.
	}

	// check whether out of range.
	if (finalT < t0 || t1 < finalT)
	{
		return false;
	}
	
	pHitRec->m_hitT = finalT;

	return true;
}

Box Sphere::BoundingBox()
{
	return Box(m_center + vector3(-m_radius, -m_radius, -m_radius), m_center + vector3(m_radius, m_radius, m_radius));
}

} // namespace CommonClass

