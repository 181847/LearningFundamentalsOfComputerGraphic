#pragma once
#include "Camera.h"

namespace CommonClass
{

class PerspectiveCamera
    :public Camera
{
public:
    const Types::F32 m_focalLenth;

public:
    /*!
        \brief init camera's position and pose
        \param origin camera location
        \param lookAt target position ( NOT VECTOR )
        \param lookUp Up vector to construct the basis, don't have to be unit vector.
    */
    PerspectiveCamera(const Types::F32 focalLength, CommonClass::vector3& origin, const CommonClass::vector3& lookAt, const CommonClass::vector3& lookUp);
    PerspectiveCamera(const PerspectiveCamera&) = delete;
    PerspectiveCamera& operator=(const PerspectiveCamera&) = delete;
    ~PerspectiveCamera();
    

    /*!
        \brief get perspective ray of the tracing route.
        \param x horizontal index from left to right
        \param y vertical index from bottom to top
    */
    virtual Ray GetRay(const Types::F32 x, const Types::F32 y) override;
};

} // namespace CommonClass
