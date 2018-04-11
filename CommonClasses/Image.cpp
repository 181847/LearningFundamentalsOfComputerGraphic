#include "Image.h"
#include <assert.h>
#include <svpng\svpng.inc>

namespace CommonClass
{

Image::Image(const Types::U32 width, const Types::U32 height, const RGBA& initColor)
	:m_width(width), m_height(height), m_canvas(width * height)
{
    // set all pixel to solid black
    for (auto & pixel : m_canvas)
    {
        pixel.m_r = static_cast<Types::U8>(initColor.m_chas.m_r * 255);
        pixel.m_g = static_cast<Types::U8>(initColor.m_chas.m_g * 255);
        pixel.m_b = static_cast<Types::U8>(initColor.m_chas.m_b * 255);
        pixel.m_a = static_cast<Types::U8>(initColor.m_chas.m_a * 255);
    }
}

Image::~Image()
{
	// empty
}

void Image::SaveTo(const std::string & filePath) const
{
	FILE * outputFile;
	// Warning!! Must open the file with the Binary mode, or the image will be broken.
	errno_t error = fopen_s(&outputFile, filePath.c_str(), "wb");
	if (error != 0)
	{
		// some error happend.
		if (nullptr != outputFile)
		{
			fclose(outputFile);
		}

		throw std::exception("PngImage openning image file failed.");
	}

	svpng(outputFile, m_width, m_height, reinterpret_cast<const unsigned char *>(m_canvas.data()), 1);

	fclose(outputFile);
}

const Types::U32 Image::To1DArrIndex(const Types::U32 x, const Types::U32 y) const
{
	assert((0 <= x && x < m_width) && (0 <= y && y < m_height)
		&& "pixel index out of range.");

	return (m_height - 1 - y) * m_width + x;
}

void Image::SetPixel(const Types::U32 x, const Types::U32 y, const RGBA & pixel)
{
    Pixel& modifiedPixel = m_canvas[To1DArrIndex(x, y)];
    modifiedPixel.m_r = static_cast<Types::U8>(pixel.m_chas.m_r * 255);
    modifiedPixel.m_g = static_cast<Types::U8>(pixel.m_chas.m_g * 255);
    modifiedPixel.m_b = static_cast<Types::U8>(pixel.m_chas.m_b * 255);
    modifiedPixel.m_a = static_cast<Types::U8>(pixel.m_chas.m_a * 255);
}

void Image::SetPixel(const Types::U32 x, const Types::U32 y, const RGB & pixel)
{
    Pixel& modifiedPixel = m_canvas[To1DArrIndex(x, y)];
    modifiedPixel.m_r = static_cast<Types::U8>(pixel.m_chas.m_r * 255);
    modifiedPixel.m_g = static_cast<Types::U8>(pixel.m_chas.m_g * 255);
    modifiedPixel.m_b = static_cast<Types::U8>(pixel.m_chas.m_b * 255);
}

void Image::SetAlpha(const Types::U32 x, const Types::U32 y, const Types::F32 & alpha)
{
    m_canvas[To1DArrIndex(x, y)].m_a = static_cast<Types::U8>(alpha * 255);
}

RGBA Image::GetPixel(const Types::U32 x, const Types::U32 y) const
{
	const Pixel& returnedPixel = m_canvas[To1DArrIndex(x, y)];

	const Types::F32 reciprocal8BitsOne = 1.0f / 255;
	
	const Types::F32 floatRed  (returnedPixel.m_r * reciprocal8BitsOne),
					 floatGreen(returnedPixel.m_g * reciprocal8BitsOne),
					 floatBlue (returnedPixel.m_g * reciprocal8BitsOne),
					 floatAlpha(returnedPixel.m_g * reciprocal8BitsOne);

	return RGBA(floatRed, floatGreen, floatBlue, floatAlpha);
}

Image::Pixel::Pixel(Types::U8 r, Types::U8 g, Types::U8 b, Types::U8 a)
	:m_r(r), m_g(g), m_b(b), m_a(a)
{
	// empty
}

}// namespace CommonClass
