#pragma once
#include "Film.h"
#include "vector3.h"
#include <memory>

namespace CommonClass
{

/*!
	\brief camera class for orthographic view (RIGHT HAND)
	the main functionality of this class is to generate perspective ray through Film class
*/
class OrthographicCamera
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

	std::unique_ptr<Film> m_film;

public:
	/*!
		\brief init camera's position and pose
		\param origin camera location
		\param lookAt target position ( NOT VECTOR )
		\param lookUp Up vector to construct the basis, don't have to be unit vector.
	*/
	OrthographicCamera(const CommonClass::vector3& origin, const CommonClass::vector3& lookAt, const CommonClass::vector3& lookUp);

	~OrthographicCamera();

	
};

} // namespace CommonClass
