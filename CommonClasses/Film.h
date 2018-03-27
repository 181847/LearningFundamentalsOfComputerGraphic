#pragma once
#include "Image.h"

namespace CommonClass
{

/*
	\brief Film is used to store color capture in the rendering, it's extend from tth Image class.
	Film add a rendering border, and helper function to get pixel position.
*/
class Film: public Image
{
public:
	const Types::F32 m_left, m_right, m_top, m_bottom;

public:
	/*!
		\brief init Film
		\param pixelWidth num pixels in horizontal
		\param pixelHeigth num pixels in vertical
		\param left/right/top/bottom film borders from origin
	*/
	Film(const Types::U32 pixelWidth, const Types::U32 pixelHeight, const Types::F32 left, const Types::F32 right, const Types::F32 top, const Types::F32 bottom);

	// prevent copy construct and equal assignment.
	Film(const Film&) = delete;
	Film& operator=(const Film&) = delete;

	~Film();

	/*!
		\brief get pixel uv through the pixel index in x, y
		\param x horizontal direction(start from zero) ---->
		\param y vertical direction(start from zero)     |^
		\param outU return value response to x
		\param outV return value response to y
	*/
	void GetPixelUV(const Types::U32 x, const Types::U32 y, Types::F32& outU, Types::F32& outV);
	
};

}// namespace CommonClass

