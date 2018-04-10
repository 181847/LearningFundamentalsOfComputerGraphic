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

void Scene::Add(std::unique_ptr<Light> light)
{
    m_lights.push_back(std::move(light));
}

bool Scene::Hit(const Ray & ray, const Types::F32 t0, const Types::F32 t1, HitRecord * pHitRec) const
{
	Types::F32 t = t1;
	bool isHit = false;

    // MUST use another HitRecord for bounding box ray hit test.
    // if not the information will influence the normal surface hit test.
    HitRecord recForBBox;
	for (auto & surf : m_surfaces)
	{
		// when hit with bounding box, the t is limited to [0.0f, t1], 
		// because it's possible that "RayOrigin(0.0f) ----> BoundingBox ------> t0 -------> Object -------> t1"
		// which the ray (dist belong to [t0, t1]) doesn't hit the BoundingBox but hit the Object, so set t0 to 0.0f to avoid this mistake.
		// However we do not modify the t1, because if a ray cannot hit a BoundingBox, it cannot hit the Object.
        if (surf->BoundingBox().Hit(ray, 0.0f, t, &recForBBox))
        //if (surf->BoundingBox().Hit(ray, 0.0f, t, pHitRec))
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
        pHitRec->m_hitPoint = ray.m_origin + (pHitRec->m_hitT + Surface::s_offsetHitT) * ray.m_direction;
    }

	return isHit;
}

RGB Scene::RayColor(const Ray & ray, const Types::F32 t0, const Types::F32 t1, unsigned int reflectLayerIndex) const
{
    HitRecord hitRec, shadowHitRec;
    if (this->Hit(ray, t0, t1, &hitRec))
    {
        RGB color;

        // ambient light
        color = hitRec.m_material.m_kDiffuse * this->m_ambient;

        // reflection
        if (reflectLayerIndex > 0)
        {
            vector3 reflectVec = Reflect(ray.m_direction, hitRec.m_normal);
            color = color + hitRec.m_material.m_shinness / 32.0f * RayColor(Ray(hitRec.m_hitPoint, reflectVec), 0.0f, 1000.0f, reflectLayerIndex - 1);
        }

        color = color + LightColor(ray, hitRec);

        return color;
    }
    else
    {
        return m_background;
    }
}

RGB Scene::LightColor(const Ray & viewRay, const HitRecord & hitRec) const
{
    RGB lightColor = RGB::BLACK;

    for (auto & light : m_lights)
    {
        // shadow test
        Types::F32 toLightDist = 0.0f;
        vector3 toLight = light->ToMeFrom(hitRec.m_hitPoint, &toLightDist);

        Ray shadowRayTest = Ray(hitRec.m_hitPoint, toLight);

        HitRecord shadowHitRec;
        // is the hit point in the shadow?
        if (!this->Hit(shadowRayTest, 0.0f, toLightDist, &shadowHitRec))
        {// yes it's in the shadow.
            vector3 toEye = -viewRay.m_direction;
            vector3 halfVec = Normalize(toEye + toLight);

            RGB lightStrength = light->m_color * std::max(0.0f, hitRec.m_normal * toLight);

            const Types::F32 m = hitRec.m_material.m_shinness;

            Types::F32 shinnessSthrength = (m + 8) / 8 * std::powf(halfVec * hitRec.m_normal, m);

            RGB fresnelCoefficient = hitRec.m_material.RFresnel(toEye * hitRec.m_normal);

            lightColor = lightColor +
                lightStrength
                * (hitRec.m_material.m_kDiffuse
                    + fresnelCoefficient * shinnessSthrength);
        }// end if
    }// end for

    return lightColor;
}

} // namespace CommonClass
