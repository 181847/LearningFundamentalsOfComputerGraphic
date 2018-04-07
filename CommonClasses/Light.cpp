#include "Light.h"

namespace CommonClass
{



Light::Light()
	:m_position(0.0f, 0.0f, 0.0f)
{
}


Light::Light(const vector3 & position)
	:m_position(position)
{
}

Light::~Light()
{
}

Ray Light::ToLight(const vector3 & reflectPoint)
{
	return Ray(reflectPoint, Normalize(reflectPoint - m_position));
}

} // namespace CommonClass
