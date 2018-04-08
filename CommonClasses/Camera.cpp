#include "Camera.h"

namespace CommonClass
{

Camera::Camera(const CommonClass::vector3 & origin, const CommonClass::vector3 & lookAt, const CommonClass::vector3 & lookUp)
	:m_origin(origin)
{
	m_w = Normalize(m_origin - lookAt);
	m_u = Normalize(crossProd(lookUp, m_w));
	m_v = Normalize(crossProd(m_w, m_u));
}

Camera::~Camera()
{
}

void Camera::IncomeLight(const Types::U32 & x, const Types::U32 & y, const RGBA & color)
{
    if (m_film.get() == nullptr)
    {
        throw std::exception("camera capture light failed: film is not setted");
    }

    m_film->SetPixel(x, y, color);
}

void Camera::IncomeLight(const Types::U32 & x, const Types::U32 & y, const RGB & color)
{
    if (m_film.get() == nullptr)
    {
        throw std::exception("camera capture light failed: film is not setted");
    }

    m_film->SetPixel(x, y, color);
}

void Camera::IncomeAlpha(const Types::U32 & x, const Types::U32 & y, const Types::F32 & color)
{
    if (m_film.get() == nullptr)
    {
        throw std::exception("camera capture light failed: film is not setted");
    }

    m_film->SetAlpha(x, y, color);
}

void Camera::SetFilm(std::unique_ptr<Film> newFilm)
{
	m_film = std::move(newFilm);
}

} // namespace CommonClass


