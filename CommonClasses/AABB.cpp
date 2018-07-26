#include "AABB.h"
#include <limits>
#include <algorithm>


namespace CommonClass
{

CommonClass::AABB::AABB(const vector3 & minPoint, const vector3 & maxPoint)
    :m_minPoint(minPoint), m_maxPoint(maxPoint)
{
}

AABB::~AABB()
{
}

bool AABB::Hit(const Ray & ray, const Types::F32 t0, const Types::F32 t1, HitRecord * pHitRec)
{
    Types::F32 tmin = Types::Constant::MIN_F32;
    Types::F32 tmax = Types::Constant::MAX_F32;
    Types::F32 f;
    Types::F32 recipocalF;
    Types::F32 p;
    Types::F32 t_1;
    Types::F32 t_2;

    for (unsigned int i = 0; i < 3; ++i)
    {
        p = ray.m_origin.m_arr[i];
        f = ray.m_direction.m_arr[i];

        // is the ray parallel to the plane?
        if (std::fabsf(f) > Types::Constant::EPSILON_F32)
        {
            recipocalF = 1.0f / f;

            t_1 = (m_maxPoint.m_arr[i] - p) * recipocalF;
            t_2 = (m_minPoint.m_arr[i] - p) * recipocalF;

            if (t_1 > t_2) std::swap(t_1, t_2);

            if (t_1 > tmin) tmin = t_1;

            if (t_2 < tmax) tmax = t_2;

            if (tmin > tmax || tmax < t0 || tmin > t1)
                return false;

        }
        // is the ray stand out side the two parallel planes?
        else if (p < m_minPoint.m_arr[i] || p > m_maxPoint.m_arr[i])
        {
            return false;
        }
    }
    
    if (tmin > t0) pHitRec->m_hitT = tmin;
    else pHitRec->m_hitT = tmax;

    return true;
}

} // namespace CommonClass
