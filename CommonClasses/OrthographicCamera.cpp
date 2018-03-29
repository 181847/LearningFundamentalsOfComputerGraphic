#include "OrthographicCamera.h"

namespace CommonClass
{
OrthographicCamera::OrthographicCamera(const CommonClass::vector3 & origin, const CommonClass::vector3 & lookAt, const CommonClass::vector3 & lookUp)
	:m_origin(origin)
{
	m_w = Normalize(m_origin - lookAt);
	m_u = Normalize(crossProd(lookUp, m_w));
	m_v = Normalize(crossProd(m_w, m_u));
}


void OrthographicCamera::IncomeLight(const Types::U32 x, const Types::U32 y, const CommonClass::RGBA & color)
{
	if (m_film.get() == nullptr)
	{
		throw std::exception("camera capture light failed: film is not setted");
	}

	m_film->SetPixel(x, y, color);
}

void OrthographicCamera::SetFilm(std::unique_ptr<Film> newFilm)
{
	m_film = std::move(newFilm);
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
