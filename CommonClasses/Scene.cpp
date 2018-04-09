#include "Scene.h"

namespace CommonClass
{

Scene::Scene()
    :m_pointLight(vector3(0.0f, 0.0f, 0.0f), RGB::WHITE)
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

        // push the hit point out from the surface a little.
        pHitRec->m_hitPoint = pHitRec->m_hitPoint + Surface::s_offsetHitT * pHitRec->m_normal;
    }

	return isHit;
}

RGB Scene::RayColor(const Ray & ray, const Types::F32 t0, const Types::F32 t1, unsigned int reflectLayerIndex)
{
    HitRecord hitRec, shadowHitRec;
    if (this->Hit(ray, t0, t1, &hitRec))
    {
        RGB color;

        color = hitRec.m_material.m_kDiffuse * this->m_ambient;

        vector3 toLight = m_pointLight.ToMeFrom(hitRec.m_hitPoint);

        Ray shadowRayTest = Ray(hitRec.m_hitPoint, toLight);

        if (reflectLayerIndex > 0)
        {
            vector3 reflectVec = ray.m_direction - 2 * (dotProd(ray.m_direction, hitRec.m_normal)) * hitRec.m_normal;
            color = color + hitRec.m_material.m_shinness / 32.0f * RayColor(Ray(hitRec.m_hitPoint, reflectVec), t0, t1, reflectLayerIndex - 1);
        }

        // is the hit point in the shadow?
        if (!this->Hit(shadowRayTest, 0.0f, 1000.0f, &shadowHitRec))
        {// yes it's in the shadow.
            vector3 toEye = -ray.m_direction;
            vector3 halfVec = Normalize(toEye + toLight);

            RGB lightStrength = m_pointLight.m_color * std::max(0.0f, hitRec.m_normal * toLight);

            const Types::F32 m = hitRec.m_material.m_shinness;

            Types::F32 shinnessSthrength = (m + 8) / 8 * std::powf(halfVec * hitRec.m_normal, m);

            RGB fresnelCoefficient = hitRec.m_material.RFresnel(toEye * hitRec.m_normal);

            color = color 
                + lightStrength 
                * (hitRec.m_material.m_kDiffuse 
                    + fresnelCoefficient * shinnessSthrength);
        }

        return color;
    }
    else
    {
        return m_background;
    }
}

} // namespace CommonClass
