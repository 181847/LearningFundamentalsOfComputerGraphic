#include "PerspectiveCamera.h"

namespace CommonClass
{

PerspectiveCamera::PerspectiveCamera(const Types::F32 focalLength, CommonClass::vector3 & origin, const CommonClass::vector3 & lookAt, const CommonClass::vector3 & lookUp)
    :Camera(origin, lookAt, lookUp), m_focalLenth(focalLength)
{
}

PerspectiveCamera::~PerspectiveCamera()
{
}

CommonClass::Ray PerspectiveCamera::GetRay(const Types::F32 x, const Types::F32 y)
{
    Types::F32 outU, outV;
    m_film->GetPixelUV(x, y, outU, outV);

    vector3 direction(-m_focalLenth * m_w + outU * m_u + outV * m_v);

    return Ray(m_origin, direction);
}

} // namespace CommonClass
