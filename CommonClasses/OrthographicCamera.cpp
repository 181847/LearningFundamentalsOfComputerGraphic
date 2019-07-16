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

CommonClass::Ray OrthographicCamera::GetRay(const Types::F32 x, const Types::F32 y)
{
    Types::F32 outU, outV;
    m_film->GetPixelUV(x, y, outU, outV);

    vector3 rayOrigin = m_origin + outU * m_u + outV * m_v;
    
    return Ray(rayOrigin, -m_w);
}

} // namespace CommonClass
