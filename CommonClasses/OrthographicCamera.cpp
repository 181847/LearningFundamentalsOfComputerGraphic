#include "OrthographicCamera.h"

namespace CommonClass
{
OrthographicCamera::OrthographicCamera(const CommonClass::vector3 & origin, const CommonClass::vector3 & lookAt, const CommonClass::vector3 & lookUp)
	:m_origin(origin)
{
	m_w = Normalize(m_origin - lookAt);
	m_v = Normalize(crossProd(lookUp, m_w));
	m_u = Normalize(crossProd(m_w, m_v));
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

} // namespace CommonClass
