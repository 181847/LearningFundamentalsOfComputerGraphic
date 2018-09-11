#pragma once
#include "Image.h"
#include "vector2.h"
#include "vector3.h"

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

    /*!
        \brief perlin noise sampled by three dimension, you can also use this as 1D or 2D noise generator.
        \param x sampling parameter, if the noise value is relative to time, you can set time to x.
        \param y same as param x
        \param z same as param x
    */
    static Types::F32 PerlinNoise(const Types::F32 x, const Types::F32 y = 0.0f, const Types::F32 z = 0.0f);
    static Types::F32 PerlinNoise(const CommonClass::vector2 xy, const Types::F32 z = 0.0f);
    static Types::F32 PerlinNoise(const CommonClass::vector3 xyz);

};// class Texture

}// namespace CommonClass
