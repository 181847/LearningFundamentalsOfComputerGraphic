#include "Light.h"

namespace CommonClass
{

Light::Light(const vector3 & position, const RGB & color)
    :m_position(position), m_color(color)
{
}

Light::~Light()
{
}

Ray Light::ToMeFrom(const vector3 & reflectPoint)
{
	return Ray(reflectPoint, Normalize(reflectPoint - m_position));
}

} // namespace CommonClass
