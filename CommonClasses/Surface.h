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
    /*!
        \brief push the hitPoint a little back to the origin.
        When I render a shadow using another ray start from the hit point,
        I find a lot black dots appear in the image, then I find out that it's a numberic issue,
        to solve this, just try to push hit point back a little, in my test -0.00004f can make all black dots disappear,
        modify it when consider you situation.
    */
    static Types::F32 s_offsetHitT;
public:
    virtual bool Hit(const Ray& ray, const Types::F32 t0, const Types::F32 t1, HitRecord* pHitRec) const = 0;
    virtual AABB BoundingBox() const = 0 ;
};

} // namespace CommonClass
