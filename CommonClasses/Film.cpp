#include "Film.h"

namespace CommonClass
{

Film::Film(const Types::U32 pixelWidth, const Types::U32 pixelHeight, const Types::F32 left, const Types::F32 right, const Types::F32 top, const Types::F32 bottom)
	:Image(pixelWidth, pixelHeight), m_left(left), m_right(right), m_top(top), m_bottom(bottom)
{
}

Film::~Film()
{
}

void Film::GetPixelUV(const Types::U32 x, const Types::U32 y, Types::F32 & outU, Types::F32 & outV)
{
	outU = m_left + (m_right - m_left) * (x / m_width);
	outV = m_bottom + (m_top - m_bottom) * (y / m_height);
}

} // namespace CommonClass
