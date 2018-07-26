#pragma once
#include "vector3.h"
#include "Ray.h"
#include "HitRecord.h"

namespace CommonClass
{

/*!
    \brief axis-aligned bouding box, this is used to accelerate the ray intersectionn between objects.
    this class dosen't derived from Surface, because it have no function to return another bounding box.
*/
class AABB
{
public:
    /*!
        \brief min values in corresponding axis.
    */
    vector3 m_minPoint;
    
    /*!
        \brief max values in corresponding axis.
    */
    vector3 m_maxPoint;

public:
    AABB(const vector3& minPoint, const vector3& maxPoint);
    ~AABB();

    /*!
        \brief whether the ray hit the box with interval [t0, t1].
        \param ray ray to test
        \param t0 min intersect dist(typical to be >= 0.0f. NOT ensure the negative number will work)
        \param t1 max intersect dist
        \param pHitRec return limited hit record, the AABB may not return entire information,
                       WARNING!! when try hitting test with bounding box, PLEASE use another HitRecord object
                       in order to avoid the confusion ( when the bounding box hit distance may overwirte the normal surface hit distance.) 
        In the distance interval, if ray dosen't hit the box,
        it should never hit the inside object.
    */
    bool Hit(const Ray& ray, const Types::F32 t0, const Types::F32 t1, HitRecord * pHitRec);
};

} // namespace CommonClass
