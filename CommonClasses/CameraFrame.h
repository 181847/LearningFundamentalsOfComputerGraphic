#pragma once
#include "Frame.h"

namespace CommonClass
{

class CameraFrame : public Frame
{
public:
    explicit CameraFrame(const vector3& origin = vector3::UNIT, const vector3& lookAt = vector3::ZERO, const vector3& lookUp = vector3::AXIS_Y);
    ~CameraFrame();

    /*!
        \brief refresh detail.
    */
    void RebuildFrameDetail();

public:
    vector3 m_lookUp;

    vector3 m_lookAt;
};

}// namespace CommonClass
