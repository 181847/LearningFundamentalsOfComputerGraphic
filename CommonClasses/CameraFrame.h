#pragma once
#include "Frame.h"

namespace CommonClass
{

class CameraFrame : public Frame
{
public:
    explicit CameraFrame(const vector3& origin, const vector3& lookAt, const vector3& lookUp = vector3(0.0f, 1.0f, 0.0f));
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
