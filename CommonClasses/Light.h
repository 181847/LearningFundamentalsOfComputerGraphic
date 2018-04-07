#pragma once
#include "vector3.h"
#include "Ray.h"

namespace CommonClass
{

/*!
	\brief this class is a prototype of the light in ray tracing, now it's a point light.
*/
class Light
{
public:
	vector3 m_position;
	
public:
	Light();
	Light(const vector3& position);
	~Light();

	/*
		\brief get a ray from the hit point to the light.
	*/
	Ray ToLight(const vector3& reflectPoint);
};

} // namespace CommonClass
