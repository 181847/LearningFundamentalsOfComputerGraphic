#include "OrthographicCamera.h"

namespace CommonClass
{
OrthographicCamera::OrthographicCamera(const CommonClass::vector3 & origin, const CommonClass::vector3 & lookAt, const CommonClass::vector3 & lookUp)
	:m_origin(origin)
{
	m_w = Normalize(m_origin - lookAt);
	m_v = crossProd(lookUp, m_w);
	m_u = crossProd(m_w, m_v);
}


OrthographicCamera::~OrthographicCamera()
{
}

} // namespace CommonClass
