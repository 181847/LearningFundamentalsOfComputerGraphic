#include "OrthographicCamera.h"

namespace CommonClass
{
OrthographicCamera::OrthographicCamera(const CommonClass::vector3 & origin, const CommonClass::vector3 & lookAt, const CommonClass::vector3 & lookUp)
	:Camera(origin, lookAt, lookUp)
{
}

OrthographicCamera::~OrthographicCamera()
{
}

Ray OrthographicCamera::GetRay(const Types::U32 x, const Types::U32 y)
{
	Types::F32 outU, outV;
	m_film->GetPixelUV(x, y, outU, outV);

	vector3 rayOrigin = m_origin + outU * m_u + outV * m_v;
	
	return Ray(rayOrigin, -m_w);
}

} // namespace CommonClass
