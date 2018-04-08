#include "Scene.h"

namespace CommonClass
{

Scene::Scene()
{
	// empty
}

Scene::~Scene()
{
	// empty
}

void Scene::Add(std::unique_ptr<Surface> surf)
{
	m_surfaces.push_back(std::move(surf));
}

bool Scene::Hit(const Ray & ray, const Types::F32 t0, const Types::F32 t1, HitRecord * pHitRec)
{
	Types::F32 t = t1;
	bool isHit = false;

	for (auto & surf : m_surfaces)
	{
		// when hit with bounding box, the t is limited to [0.0f, t1], 
		// because it's possible that "RayOrigin(0.0f) ----> BoundingBox ------> t0 -------> Object -------> t1"
		// which the ray (dist belong to [t0, t1]) doesn't hit the BoundingBox but hit the Object, so set t0 to 0.0f to avoid this mistake.
		// However we do not modify the t1, because if a ray cannot hit a BoundingBox, it cannot hit the Object.
		if (surf->BoundingBox().Hit(ray, 0.0f, t, pHitRec)) 
		{
			if (surf->Hit(ray, t0, t, pHitRec))
			{
				isHit = true;
				if (pHitRec->m_hitT < t)
				{
					t = pHitRec->m_hitT;
				}
			}
		}
	}

    if (isHit)
    {
        pHitRec->m_hitPoint = ray.m_origin + pHitRec->m_hitT * ray.m_direction;
    }

	return isHit;
}

} // namespace CommonClass
