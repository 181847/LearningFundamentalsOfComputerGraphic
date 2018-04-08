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
    RGB     m_ambient = RGB::WHITE * 0.1f;

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
	bool Hit(const Ray& ray, const Types::F32 t0, const Types::F32 t1, HitRecord* pHitRec);
};

} // namespace CommonClass
