#pragma once
#include <vector>
#include <memory>
#include "Ray.h"
#include "HitRecord.h"
#include "Surface.h"
#include "Light.h"

namespace CommonClass
{

/*!
	\brief Scene arrange all the lights and shading object, compute the closest hit point of a ray.
*/
class Scene
{
public:
    /*!
        \brief ambient color.
    */
    RGB     m_ambient = RGB::WHITE * 0.3f;

    /*!
        \brief background color for rendering.
        default to be black.
    */
    const RGB     m_background = RGB::BLACK;

    /*!
        \brief default point light
        this is an experimental object for test.
        default to be at origin, color is white.
    */
    Light   m_pointLight;

private:
	std::vector<std::unique_ptr<Surface>> m_surfaces;


public:
	Scene();
	Scene(const Scene&) = delete;
	Scene& operator = (const Scene&) = delete;
	~Scene();

public:
	/*!
		\brief add a new surface object to the scene, such as a sphere or a triangle.
		\param surf the new surface
	*/
	void Add(std::unique_ptr<Surface> surf);

	/*!
		\brief find the closest hit point in the scene.
		\param ray the ray to cast
		\param t0 minimum distance(include t0, this should be positive, no gaurantee about that the negative value will work properly.).
		\param t1 maxmum distance(include t1)
	*/
	bool Hit(const Ray& ray, const Types::F32 t0, const Types::F32 t1, HitRecord* pHitRec) const;

    /*!
        \brief evaluate the color of the hit point.
        \param ray ray to test
        \param t0 min distance, should be positive
        \param t1 max distance, shoubld be positive
        \param reflectLayerIndex reflectLayerIndex is used to prevent deep recursion to compute specular color,
                                only if reflectLayerIndex greater than 0, then generate another ray to compute specular color with RayColor(..., reflectLayerIndex - 1).
    */
    RGB RayColor(const Ray& ray, const Types::F32 t0, const Types::F32 t1, unsigned int reflectLayerIndex = 3) const;

    /*!
        \brief evaluate the light of this point.
        \param viewRay the ray to capture the light
        \param hitRec the properties of hit position
        this will take multiple light into account.
    */
    RGB LightColor(const Ray& viewRay, const HitRecord& hitRec) const;
};

} // namespace CommonClass
