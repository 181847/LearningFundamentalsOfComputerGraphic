#pragma once
#include "Image.h"

namespace CommonClass
{

/*!
    \brief sampling configurations    
*/
struct SampleState
{
    enum WrapMode
    {
        Clamp = 0,
        Loop
    };

public:
    SampleState(WrapMode wrapMode = WrapMode::Loop): m_wrapMode(wrapMode) {}

    /*!
        \brief fix uv by the sampling status, ensure u/v will be inside [0.0, 1.0]
        if the uv will be clamped, return false.
    */
    void FixUV(const Types::F32 u, const Types::F32 v, Types::F32& outU, Types::F32& outV) const;

public:
    WrapMode m_wrapMode;
};

class Texture : public Image
{
public:
    /*!
        \brief load image file from the path, using stb_image to load image.
    */
    bool LoadFile(const std::string& file);

    /*!
        \brief sample by uv
    */
    RGBA Sample(const Types::F32 u, const Types::F32 v, const SampleState& sampleState = SampleState());

};// class Texture

}// namespace CommonClass
