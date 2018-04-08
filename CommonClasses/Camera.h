#pragma once
#include "Film.h"
#include "vector3.h"
#include "Ray.h"
#include <memory>

namespace CommonClass
{

/*!
	\brief a camera have a coordinate frame and can generate tracing rays to caputure lights.
*/
class Camera
{
public:
	/*!
		\brief camera position
	*/
	CommonClass::vector3 m_origin;

	/*!
		\brief point to up
	*/
	CommonClass::vector3 m_u;

	/*!
		\brief point to right
	*/
	CommonClass::vector3 m_v;

	/*!
		\brief point to the photographer( or negative to the direction of camera-->scene)
	*/
	CommonClass::vector3 m_w;
	
	/*!
		\brief used to save pixels and support output to an '*.png' file
	*/
	std::unique_ptr<Film> m_film;

public:
	/*!
		\brief init camera's position and pose
		\param origin camera location
		\param lookAt target position ( NOT VECTOR )
		\param lookUp Up vector to construct the basis, don't have to be unit vector.
	*/
	Camera(const CommonClass::vector3& origin, const CommonClass::vector3& lookAt, const CommonClass::vector3& lookUp);
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;
	~Camera();

	/*!
		\brief get ray of the tracing route.
		\param x horizontal index from left to right
		\param y vertical index from bottom to top
	*/
	virtual Ray GetRay(const Types::U32 x, const Types::U32 y) = 0;
    
	/*!
		\brief set color of the film
		\param x horizontal index from left to right
		\param y vertical index from bottom to top
		\param color set color into the film.
	*/
    void IncomeLight(const Types::U32& x, const Types::U32& y, const RGBA& color);
    
	/*!
		\brief set rgb color of the film
		\param x horizontal index from left to right
		\param y vertical index from bottom to top
		\param color set rgb color into the film.
	*/
    void IncomeLight(const Types::U32& x, const Types::U32& y, const RGB& color);
    
	/*!
		\brief set color of the film
		\param x horizontal index from left to right
		\param y vertical index from bottom to top
		\param color set color into the film.
	*/
    void IncomeAlpha(const Types::U32& x, const Types::U32& y, const Types::F32& color);

	/*!
		\brief set a new film on the camera
	*/
	void SetFilm(std::unique_ptr<Film> newFilm);
};

}// namespace CommonClass

