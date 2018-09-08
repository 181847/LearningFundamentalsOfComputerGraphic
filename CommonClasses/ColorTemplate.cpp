#include "ColorTemplate.h"

namespace CommonClass {

Pixel::Pixel(Types::U8 r /*= 0*/, Types::U8 g /*= 0*/, Types::U8 b /*= 0*/, Types::U8 a /*= 0*/)
    :m_r(r), m_g(g), m_b(b), m_a(a)
{
    // empty
}

ColorTemplate<false> Cast(const ColorTemplate<true>& color)
{
    return ColorTemplate<false>(color.m_chas.m_r, color.m_chas.m_g, color.m_chas.m_b);
}

ColorTemplate<true> Cast(const ColorTemplate<false>& color, const Types::F32 alpha)
{
    return ColorTemplate<true>(color.m_chas.m_r, color.m_chas.m_g, color.m_chas.m_b, alpha);
}

Pixel CastPixel(const ColorTemplate<true>& color)
{
    return Pixel(
        static_cast<Types::U8>(color.m_chas.m_r * 255), 
        static_cast<Types::U8>(color.m_chas.m_g * 255), 
        static_cast<Types::U8>(color.m_chas.m_b * 255), 
        static_cast<Types::U8>(color.m_chas.m_a * 255));
}

CommonClass::RGBA CastPixel(const Pixel& pixel)
{
    const Types::F32 reciprocal8BitsOne = 1.0f / 255;

    const Types::F32 
        floatRed    (pixel.m_r * reciprocal8BitsOne),
        floatGreen  (pixel.m_g * reciprocal8BitsOne),
        floatBlue   (pixel.m_b * reciprocal8BitsOne),
        floatAlpha  (pixel.m_a * reciprocal8BitsOne);

    return RGBA(floatRed, floatGreen, floatBlue, floatAlpha);
}

template class ColorTemplate<true>;
template class ColorTemplate<false>;

static_assert(sizeof(ColorTemplate<true>) == sizeof(Types::F32) * 4, "sizeof ColorTemplate<true> (RGBA) is wrong");
static_assert(sizeof(ColorTemplate<false>) == sizeof(Types::F32) * 3, "sizeof ColorTemplate<false> (RGB) is wrong");

}// namespace CommonClass

