#include "Ray.h"


namespace CommonClass
{
Ray::Ray()
	:m_origin(vector3(0.0f, 0.0f, 0.0f)), m_direction(vector3(1.0f, 0.0f, 0.0f))
{
}

Ray::Ray(const vector3 & origin, const vector3 & direction)
	:m_origin(origin), m_direction(Normalize(direction))
{
}

Ray::~Ray()
{
}

void Ray::SetOrigin(const vector3 & origin)
{
	m_origin = origin;
}

void Ray::SetDirection(const vector3 & direction)
{
	m_direction = Normalize(direction);
}

} // namespace CommonClass
