#include "RGBA.h"
#include <MyTools\MathTool.h>

namespace CommonClass
{

const Types::F32 RGBA::MIN_CHANNEL_VALUE = 0.0f;
const Types::F32 RGBA::MAX_CHANNEL_VALUE = 1.0f;
const Types::F32 RGBA::HALF_CHANNEL_VALUE = 0xFF >> 1;
const Types::F32 RGBA::ALPHA_CHANNEL_OPAQUE = MAX_CHANNEL_VALUE;
const Types::F32 RGBA::ALPHA_CHANNEL_TRANSPARENT = MIN_CHANNEL_VALUE;

RGBA::RGBA()
	:m_r(MIN_CHANNEL_VALUE),
	 m_g(MIN_CHANNEL_VALUE),
	 m_b(MIN_CHANNEL_VALUE),
	 m_a(ALPHA_CHANNEL_OPAQUE)
{
	// empty
}

RGBA::RGBA(const Types::F32 & r, const Types::F32 & g, const Types::F32 & b, const Types::F32 & a)
	:m_r(ClampChannel(r)),
	 m_g(ClampChannel(g)),
	 m_b(ClampChannel(b)),
	 m_a(ClampChannel(a))
{
	// empty
}

RGBA::~RGBA()
{
	// empty
}

void RGBA::AssignRGB(const RGBA & source)
{
	m_r = ClampChannel(source.m_r);
	m_g = ClampChannel(source.m_g);
	m_b = ClampChannel(source.m_b);
}

void RGBA::AddRGB(const RGBA & source)
{
	m_r = ClampChannel(m_r + source.m_r);
	m_g = ClampChannel(m_g + source.m_g);
	m_b = ClampChannel(m_b + source.m_b);
}

void RGBA::SubRGB(const RGBA & source)
{
	m_r = ClampChannel(m_r - source.m_r);
	m_g = ClampChannel(m_g - source.m_g);
	m_b = ClampChannel(m_b - source.m_b);
}

void RGBA::MulRGB(const RGBA & scalePixel)
{
	m_r = ClampChannel(m_r * scalePixel.m_r);
	m_g = ClampChannel(m_g * scalePixel.m_g);
	m_b = ClampChannel(m_b * scalePixel.m_b);
}

void RGBA::MulRGB(const Types::F32 & scalar)
{
	m_r = ClampChannel(m_r * scalar);
	m_g = ClampChannel(m_g * scalar);
	m_b = ClampChannel(m_b * scalar);
}

void RGBA::DivRGB(const Types::F32 & scalar)
{
	const Types::F32 reciprocalScalar = 1.0f / scalar;
	m_r = ClampChannel(m_r * reciprocalScalar);
	m_g = ClampChannel(m_g * reciprocalScalar);
	m_b = ClampChannel(m_b * reciprocalScalar);
}

void RGBA::AddAlpha(const Types::F32 & alpha)
{
	m_a = ClampChannel(m_a + alpha);
}

void RGBA::SubAlpha(const Types::F32 & alpha)
{
	m_a = ClampChannel(m_a - alpha);
}

void RGBA::MulAlpha(const Types::F32 & scale)
{
	m_a = ClampChannel(m_a * scale);
}

void RGBA::DivAlpha(const Types::F32 & scale)
{
	m_a = ClampChannel(m_a / scale);
}

Types::F32 RGBA::ClampChannel(const Types::F32 & value)
{
	if (value < 0.0f)
	{
		return 0.0f;
	}
	else if (value > 1.0f)
	{
		return 1.0f;
	}
	else
	{
		return value;
	}
}

bool operator==(const RGBA & a, const RGBA & b)
{
	return a.m_r == b.m_r
		&& a.m_g == b.m_g
		&& a.m_b == b.m_b
		&& a.m_a == b.m_a;
}

bool operator!=(const RGBA & a, const RGBA & b)
{
	return a.m_r != b.m_r
		|| a.m_g != b.m_g
		|| a.m_b != b.m_b
		|| a.m_a != b.m_a;
}

bool AlmostEqual(const RGBA & a, const RGBA & b, int ulp)
{
	return MathTool::almost_equal(a.m_r, b.m_r, ulp)
		&& MathTool::almost_equal(a.m_g, b.m_g, ulp)
		&& MathTool::almost_equal(a.m_b, b.m_b, ulp)
		&& MathTool::almost_equal(a.m_a, b.m_a, ulp);
}

}// namespace CommonClass

