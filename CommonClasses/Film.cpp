#include "Film.h"

namespace CommonClass
{

Film::Film(const Types::U32 pixelWidth, const Types::U32 pixelHeight, const Types::F32 left, const Types::F32 right, const Types::F32 bottom, const Types::F32 top)
	:Image(pixelWidth, pixelHeight), m_left(left), m_right(right), m_bottom(bottom), m_top(top)
{
}

Film::~Film()
{
}

void Film::GetPixelUV(const Types::U32 x, const Types::U32 y, Types::F32 & outU, Types::F32 & outV)
{
	outU = m_left + (m_right - m_left) * (1.0f * x / m_width);
	outV = m_bottom + (m_top - m_bottom) * (1.0f * y / m_height);
}

} // namespace CommonClass
