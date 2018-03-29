#pragma once
#include "Camera.h"

namespace CommonClass
{

/*!
	\brief camera class for orthographic view (RIGHT HAND)
	the main functionality of this class is to generate perspective ray through Film class
*/
class OrthographicCamera
	:public Camera
{
public:
	/*!
		\brief init camera's position and pose
		\param origin camera location
		\param lookAt target position ( NOT VECTOR )
		\param lookUp Up vector to construct the basis, don't have to be unit vector.
	*/
	OrthographicCamera(const CommonClass::vector3& origin, const CommonClass::vector3& lookAt, const CommonClass::vector3& lookUp);
	OrthographicCamera(const OrthographicCamera&) = delete;
	OrthographicCamera& operator=(const OrthographicCamera&) = delete;
	~OrthographicCamera();

	/*!
		\brief get parallel ray of the tracing route.
		\param x horizontal index from left to right
		\param y vertical index from bottom to top
	*/
	virtual Ray GetRay(const Types::U32 x, const Types::U32 y) override;
};

} // namespace CommonClass
