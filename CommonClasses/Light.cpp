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

vector3 Light::ToMeFrom(const vector3 & reflectPoint)
{
	return Normalize(m_position - reflectPoint);
}

} // namespace CommonClass
