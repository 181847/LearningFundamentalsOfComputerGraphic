#include "ColorTemplate.h"

namespace CommonClass{

ColorTemplate<false> Cast(const ColorTemplate<true>& color)
{
    return ColorTemplate<false>(color.m_chas.m_r, color.m_chas.m_g, color.m_chas.m_b);
}

ColorTemplate<true> Cast(const ColorTemplate<false>& color, const Types::F32 alpha)
{
    return ColorTemplate<true>(color.m_chas.m_r, color.m_chas.m_g, color.m_chas.m_b, alpha);
}

template class ColorTemplate<true>;
template class ColorTemplate<false>;

static_assert(sizeof(ColorTemplate<true>) == sizeof(Types::F32) * 4, "sizeof ColorTemplate<true> (RGBA) is wrong");
static_assert(sizeof(ColorTemplate<false>) == sizeof(Types::F32) * 3, "sizeof ColorTemplate<false> (RGB) is wrong");

}// namespace CommonClass

