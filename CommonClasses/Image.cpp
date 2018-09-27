#include "Image.h"
#include <assert.h>
#include <svpng\svpng.inc>

namespace CommonClass
{

Image::Image(const Types::U32 width, const Types::U32 height, const RGBA& initColor)
{
    Init(width, height, initColor);
}

Image::Image(Image && moveObj)
{
    this->m_height      = moveObj.m_height;
    this->m_width       = moveObj.m_width;
    this->m_canvas      = std::move(moveObj.m_canvas);
    this->m_floatCanvas = std::move(moveObj.m_floatCanvas);
}

Image::Image()
    :m_width(0), m_height(0), m_canvas(0)
{
    // empty
}

Image& Image::operator=(const Image&& moveObj)
{
    this->m_height      = moveObj.m_height;
    this->m_width       = moveObj.m_width;
    this->m_canvas      = std::move(moveObj.m_canvas);
    this->m_floatCanvas = std::move(moveObj.m_floatCanvas);
    return *this;
}

Image::~Image()
{
    // empty
}

void Image::Init(const Types::U32 width, const Types::U32 height, const RGBA& initColor /*= RGBA::BLACK*/)
{
    assert(width > 0 && height > 0);

    m_width = width;
    m_height = height;
    m_canvas.resize(m_width * m_height);
    m_floatCanvas.resize(m_width * m_height);
    vector4 initFloatPixel = vector4::WHITE;

    auto initPixel = CastPixel(initColor);
    // set all pixel to solid black
    for (auto & pixel : m_canvas)
    {
        pixel = initPixel;
    }

    for (auto & pixel : m_floatCanvas)
    {
        pixel = initFloatPixel;
    }
}

bool Image::IsValid() const
{
    return m_width > 0 
        && m_height > 0 
        && !m_canvas.empty();
}

void Image::SaveTo(const std::wstring & filePath)
{
    FILE * outputFile;
    // Warning!! Must open the file with the Binary mode, or the image will be broken.
    //errno_t error = fopen_s(&outputFile, filePath.c_str(), "wb");
    errno_t error = _wfopen_s(&outputFile, filePath.c_str(), L"wb");
    if (error != 0)
    {
        // some error happend.
        if (nullptr != outputFile)
        {
            fclose(outputFile);
        }

        throw std::exception("PngImage openning image file failed - unicode version");
    }

    // update byte pixels for output.
    FloatPixelToBytePixel();
    svpng(outputFile, m_width, m_height, reinterpret_cast<const unsigned char *>(m_canvas.data()), 1);

    fclose(outputFile);
}

void Image::SaveTo(const std::string& filePath)
{
    FILE * outputFile;
    // Warning!! Must open the file with the Binary mode, or the image will be broken.
    //errno_t error = fopen_s(&outputFile, filePath.c_str(), "wb");
    errno_t error = fopen_s(&outputFile, filePath.c_str(), "wb");
    if (error != 0)
    {
        // some error happend.
        if (nullptr != outputFile)
        {
            fclose(outputFile);
        }

        throw std::exception("PngImage openning image file failed - ascii version");
    }

    // update byte pixels for output.
    FloatPixelToBytePixel();
    svpng(outputFile, m_width, m_height, reinterpret_cast<const unsigned char *>(m_canvas.data()), 1);

    fclose(outputFile);
}

//void Image::ClearPixel(const Pixel& pixel)
//{
//    for (auto& innerPixel : m_canvas)
//    {
//        innerPixel = pixel;
//    }
//}

void Image::ClearPixel(const vector4& pixel)
{
    for (auto& innerPixel : m_floatCanvas)
    {
        innerPixel = pixel;
    }
}

const Types::U32 Image::To1DArrIndex(const Types::U32 x, const Types::U32 y) const
{
    assert((0 <= x && x < m_width) && (0 <= y && y < m_height)
        && "pixel index out of range.");

    // Note, the layout of pixels in m_canvas is:
    // as the index of m_canvas increase
    //   |--pMem starts from here
    //   |
    //   |
    //  |V-----------------------|
    // 0|P >===[column first]==>>|
    // 1|                        |
    // 2|                        |
    // 3|       m_canvas         |
    // 4|                        |
    //  |------------------------|

    // but our parameter y is from bottom to top,
    // so we must flip the y.
    return (m_height - 1 - y) * m_width + x;
}

void Image::FloatPixelToBytePixel()
{
    for (int i = m_floatCanvas.size() - 1; i >= 0; --i)
    {
        m_canvas[i] = m_floatCanvas[i];
    }
}

void Image::BytePixelToFloatPixel()
{
    for (int i = m_floatCanvas.size() - 1; i >= 0; --i)
    {
        m_floatCanvas[i] = m_canvas[i].ToVector<vector4>();
    }
}

//void Image::SetPixel(const Types::U32 x, const Types::U32 y, const RGBA & pixel)
//{
//    Pixel& modifiedPixel = m_canvas[To1DArrIndex(x, y)];
//    modifiedPixel = CastPixel(pixel);
//    /*modifiedPixel.m_r = static_cast<Types::U8>(pixel.m_chas.m_r * 255);
//    modifiedPixel.m_g = static_cast<Types::U8>(pixel.m_chas.m_g * 255);
//    modifiedPixel.m_b = static_cast<Types::U8>(pixel.m_chas.m_b * 255);
//    modifiedPixel.m_a = static_cast<Types::U8>(pixel.m_chas.m_a * 255);*/
//}
//
//void Image::SetPixel(const Types::U32 x, const Types::U32 y, const RGB & pixel)
//{
//    Pixel& modifiedPixel = m_canvas[To1DArrIndex(x, y)];
//    auto resultPixel = CastPixel(Cast(pixel));
//    modifiedPixel.m_r = resultPixel.m_r;
//    modifiedPixel.m_g = resultPixel.m_g;
//    modifiedPixel.m_b = resultPixel.m_b;
//    /*modifiedPixel.m_r = static_cast<Types::U8>(pixel.m_chas.m_r * 255);
//    modifiedPixel.m_g = static_cast<Types::U8>(pixel.m_chas.m_g * 255);
//    modifiedPixel.m_b = static_cast<Types::U8>(pixel.m_chas.m_b * 255);*/
//}

void Image::SetPixel(const Types::U32 x, const Types::U32 y, const vector4& pixel)
{
    vector4& modifiedPixel = m_floatCanvas[To1DArrIndex(x, y)];
    modifiedPixel = pixel;
    ClampChannels(modifiedPixel);
}

void Image::SetPixel(const Types::U32 x, const Types::U32 y, const vector3& pixel)
{
    vector4& modifiedPixel = m_floatCanvas[To1DArrIndex(x, y)];
    modifiedPixel = pixel;
    ClampChannels(modifiedPixel);
}

void Image::SetAlpha(const Types::U32 x, const Types::U32 y, const Types::F32 & alpha)
{
    m_canvas[To1DArrIndex(x, y)].m_a = static_cast<Types::U8>(alpha * 255);
    vector4& modifedPixel = m_floatCanvas[To1DArrIndex(x, y)];
    modifedPixel.m_w = MathTool::clamp(alpha, 0.0f, 1.0f);
}

CommonClass::vector4 Image::GetPixel(const Types::U32 x, const Types::U32 y) const
{
    return m_floatCanvas[To1DArrIndex(x, y)];
}

unsigned char * Image::GetRawData()
{
    return reinterpret_cast<unsigned char*>(m_canvas.data());
}

}// namespace CommonClass
