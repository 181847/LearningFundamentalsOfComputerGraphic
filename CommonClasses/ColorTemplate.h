#pragma once
#include "CommonTypes.h"
#include <type_traits>
#include <assert.h>

/*!
	\brief a helper struct to assist the ColorTemplate to access individual channels of an color.
	this struct support RGB three channels to read.
*/
struct Channel3
{
public:
	Types::F32 m_r, m_g, m_b;
};
static_assert(sizeof(Channel3) == sizeof(Types::F32) * 3, "size of Channel3 is wrong");


/*!
	\brief a helper struct to assist the ColorTemplate to access individual channels of an color.
	this struct support RGBA four channels to read.
*/
struct Channel4
	:public Channel3
{
public:
	Types::F32 m_a;
};
static_assert(sizeof(Channel4) == sizeof(Types::F32) * 4, "size of Channel4 is wrong");


/*!
	\brief a ColorTemplate is the template of the RGB and RGBA.
	the difference between them is whether they have alpha channel.
	the RGB type can be used to compute color,
	the RGBA type can be used to out put image file or do the blending works.
*/
template<bool HAVE_ALPHA>
class ColorTemplate
{
public:
	/*!
		\brief give the specialization class a alias of the CURRENT type.
	*/
	using AliasOfThisType = ColorTemplate<HAVE_ALPHA>;

	union
	{
		typename std::conditional<HAVE_ALPHA, Channel4, Channel3>::type m_chas;
		Types::F32 m_arr[HAVE_ALPHA ? 4 : 3];
	};
	

	/*!
		\brief enumerate the channels: R->red, G->green, B->blue, A->Alpha
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
		\brief set one channel value
		\param value new value of one channel, this value will be clampped between [0.0f, 1.0f]
	*/
	template<Types::U8 ch>
	void SetChannel(const Types::F32 & value);

	
	/*!
		\brief clamp the value to [MIN_CHANNEL_VALUE, MAX_CHANNEL_VALUE].
		\param value the value will be clampped.
	*/
	static Types::F32 ClampChannel(const Types::F32 & value);
};

template<bool HAVE_ALPHA>
const Types::F32	ColorTemplate<HAVE_ALPHA>::MIN_CHANNEL_VALUE = 0.0f;

template<bool HAVE_ALPHA>
const Types::F32	ColorTemplate<HAVE_ALPHA>::MAX_CHANNEL_VALUE = 1.0f;

template<bool HAVE_ALPHA>
const Types::F32	ColorTemplate<HAVE_ALPHA>::HALF_CHANNEL_VALUE = 0.5f;

template<bool HAVE_ALPHA>
const Types::F32	ColorTemplate<HAVE_ALPHA>::ALPHA_CHANNEL_OPAQUE = 1.0f;

template<bool HAVE_ALPHA>
const Types::F32	ColorTemplate<HAVE_ALPHA>::ALPHA_CHANNEL_TRANSPARENT = 0.0f;

template<>
template<Types::U8 ch>
void ColorTemplate<true>::SetChannel(const Types::F32 & value)
{
	const Types::F32 clamppedValue = ClampChannel(value);

	switch (ch)
	{
	case AliasOfThisType::R:
		m_chas.m_r = clamppedValue;
		break;

	case AliasOfThisType::G:
		m_chas.m_g = clamppedValue;
		break;

	case AliasOfThisType::B:
		m_chas.m_b = clamppedValue;
		break;

	case AliasOfThisType::A:
		m_chas.m_a = clamppedValue;
		break;

	default:
		assert(false && "unexpected channel to set");
		break;
	}
}

template<>
template<Types::U8 ch>
void ColorTemplate<false>::SetChannel(const Types::F32 & value)
{
	const Types::F32 clamppedValue = ClampChannel(value);

	switch (ch)
	{
	case AliasOfThisType::R:
		m_chas.m_r = clamppedValue;
		break;

	case AliasOfThisType::G:
		m_chas.m_g = clamppedValue;
		break;

	case AliasOfThisType::B:
		m_chas.m_b = clamppedValue;
		break;

	default:
		assert(false && "unexpected channel to set");
		break;
	}
}

template<bool HAVE_ALPHA>
Types::F32 ColorTemplate<HAVE_ALPHA>::ClampChannel(const Types::F32 & value)
{
	if (value < MIN_CHANNEL_VALUE)
	{
		return 0.0f;
	}
	else if (value > MAX_CHANNEL_VALUE)
	{
		return 1.0f;
	}
	else
	{
		return value;
	}
}
