#pragma once
#include "CommonTypes.h"

namespace CommonClass
{

class RGBA
{
public:
	union
	{
		struct
		{
			Types::F32 m_r, m_g, m_b, m_a;
		};
		Types::F32 m_arr[4];
	};

	/*!
		\brief enumerate the channels
	*/
	enum
	{
		R = 0,
		G,
		B,
		A
	};

	/*!
		\brief some useful constant value
	*/
	const static Types::F32 MIN_CHANNEL_VALUE;
	const static Types::F32 MAX_CHANNEL_VALUE;
	const static Types::F32 HALF_CHANNEL_VALUE;
	const static Types::F32 ALPHA_CHANNEL_OPAQUE;
	const static Types::F32 ALPHA_CHANNEL_TRANSPARENT;

	/*!
		\brief some useful color value.
	*/
	const static RGBA RED;
	const static RGBA GREEN;
	const static RGBA BLUE;
	const static RGBA YELLOW;
	const static RGBA CYAN;
	const static RGBA MAGENTA;
	const static RGBA WHITE;
	const static RGBA BLACK;


	/*!
		\brief decide the precision of the almost equal floats��less it is, more acurate the equal comparing will be.
	*/
	static const Types::I32 COMPARE_ULP = 8;

public:
	RGBA();
	RGBA(const Types::F32 & r, const Types::F32 & g, const Types::F32 & b, const Types::F32 & a = ALPHA_CHANNEL_OPAQUE);
	~RGBA();

	/*!
		\brief set one channel value
		\param value new value of one channel
	*/
	template<Types::U8 ch>
	void SetChannel(const Types::F32 & value);

	/*!
		\brief let the rgb channel equal to the source
		\param source the pixel hold desired rgb value
		it will keep the original alpha value.
	*/
	void AssignRGB(const RGBA & source);

	/*!
		\brief increase rgb value by the source
		\param source the pixel hold rgb value to add
		if overflow, clamp to max value,
		ignore alpha channel.
	*/
	void AddRGB(const RGBA & source);
	
	/*!
		\brief decrease rgb value by the source
		\param source the pixel hold rgb value to add
		if negative overflow, clamp to min value,
		ignore alpha channel.
	*/
	void SubRGB(const RGBA & source);

	/*!
		\brief scale the this pixel's rgb value by another pixel's correspond rgb value
		\param scalePixel how much to scale the rgb
		alpha will be ignored.
	*/
	void MulRGB(const RGBA & scalePixel);

	/*!
		\brief multiply by a float.
		\param scalar scale value, a negative value will cause exception.
	*/
	void MulRGB(const Types::F32 & scalar);

	/*!
		\brief divided by a float
		\param scalar divided value, a negative value will cause exception.
	*/
	void DivRGB(const Types::F32 & scalar);

	/*!
		\brief add to the alpha, clamp it between [0.0f, 1.0f]
	*/
	void AddAlpha(const Types::F32 & alpha);
	
	/*!
		\brief substract the alpha, clamp it between [0.0f, 1.0f]
	*/
	void SubAlpha(const Types::F32 & alpha);
	
	/*!
		\brief multiply by the alpha, clamp it between [0.0f, 1.0f]
	*/
	void MulAlpha(const Types::F32 & scale);
	
	/*!
		\brief divided by the alpha, clamp it between [0.0f, 1.0f]
	*/
	void DivAlpha(const Types::F32 & scale);

	/*!
		\brief clamp the value to [0.0f, 1.0f].
		\param value the value will be clampped.
	*/
	static Types::F32 ClampChannel(const Types::F32 & value);
};
/* ensurance the size of RGBA */
static_assert(sizeof(RGBA) == sizeof(Types::F32) * 4, "size of RGBA is wrong");

bool operator == (const RGBA & a, const RGBA & b); 
bool operator != (const RGBA & a, const RGBA & b);

template<Types::U8 ch>
inline void RGBA::SetChannel(const Types::F32 & value)
{
	const Types::F32 clamppedValue = ClampChannel(value);

	switch (ch)
	{
	case RGBA::R:
		m_r = clamppedValue;
		break;

	case RGBA::G:
		m_g = clamppedValue;
		break;

	case RGBA::B:
		m_b = clamppedValue;
		break;

	case RGBA::A:
		m_a = clamppedValue;
		break;
	}
}

/*!
	\brief two RGBA are almost equal
	\param a compared pixel
	\param b compared pixel
	\param ulp the precision of compare, lower and more accurate.
*/
bool AlmostEqual(const RGBA & a, const RGBA & b, int ulp = RGBA::COMPARE_ULP);

}// namespace CommonClass
