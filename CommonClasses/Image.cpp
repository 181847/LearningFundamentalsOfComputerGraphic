#include "Image.h"
#include <svpng\svpng.inc>

namespace CommonClass
{

Image::Image(const Types::U32 width, const Types::U32 height)
	:m_width(width), m_height(height), m_canvas(width * height)
{
	// empty
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

void Image::SetPixel(const Types::U32 x, const Types::U32 y, const RGBA & pixel)
{
	Pixel& modifiedPixel = m_canvas[y * m_width + x];
	modifiedPixel.m_r = static_cast<Types::U8>(pixel.m_r * 255);
	modifiedPixel.m_g = static_cast<Types::U8>(pixel.m_g * 255);
	modifiedPixel.m_b = static_cast<Types::U8>(pixel.m_b * 255);
	modifiedPixel.m_a = static_cast<Types::U8>(pixel.m_a * 255);
}

RGBA Image::GetPixel(const Types::U32 x, const Types::U32 y) const
{
	const Pixel& returnedPixel = m_canvas[y * m_width + x];

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
