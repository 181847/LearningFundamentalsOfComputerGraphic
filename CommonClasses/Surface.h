#pragma once
#include "Ray.h"
#include "Box.h"
#include "HitRecord.h"
#include "AABB.h"

namespace CommonClass
{

/*!
	\brief Surface is a base class for all visible object in the ray tracing program
*/
class Surface
{
public:
	virtual bool Hit(const Ray& ray, const Types::F32 t0, const Types::F32 t1, HitRecord* pHitRec) = 0;
	virtual AABB BoundingBox() = 0;
};

} // namespace CommonClass
