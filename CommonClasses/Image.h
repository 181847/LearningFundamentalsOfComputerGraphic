#pragma once
#include "ColorTemplate.h"
#include "vector4.h"
#include "vector3.h"
#include <string>
#include <vector>

namespace CommonClass
{

class Image
{
public:

protected:
    /*!
        \brief store pixels, (1 byte for each channel).
    */
    std::vector<Pixel> m_canvas;

    /*!
        \brief store pixels, (1 float for each channel).
    */
    std::vector<vector4> m_floatCanvas;

    /*!
        \brief the width and height of the image
        they should only be set once in the constructor of the Image.
    */
    Types::U32 m_width, m_height;

public:
    /*!
        \brief create a image with the size, the color is initialized to [0, 0, 0, 0] (* BLACK and OPAQUE *)
        \param width width of the image
        \param height height of the image
    */
    Image(const Types::U32 width, const Types::U32 height, const RGBA& initColor = RGBA::BLACK);
    Image();
    Image(const Image&) = delete;
    Image(Image&& moveObj);
    Image& operator=(const Image&) = delete;
    Image& operator=(const Image&&);
    ~Image();

    /*!
        \brief is this image contain validate data(some time, the image is just created by default constructor, and has no pixel stored in it.)
    */
    bool IsValid() const;
    
    /*!
        \brief save the image to the file unicode version
        \param filePath the file path to save the image
        this function will overwrite the file with same name.
    */
    void SaveTo(const std::wstring& filePath);
    /*!
        \brief save the image to the file ascii version
        \param filePath the file path to save the image
        this function will overwrite the file with same name.
    */
    void SaveTo(const std::string& filePath);

    /*!
        \brief clear all pixel color.
    */
    //void ClearPixel(const Pixel&   pixel);
    void ClearPixel(const vector4& pixel);

    /*!
        \brief set one pixel with RGBA
        \param x column of the pixel, from left to right
        \param y row of the pixel, from bottom to top
        \param pixel the pixel value
    */
    //void SetPixel(const Types::U32 x, const Types::U32 y, const RGBA& pixel);
    void SetPixel(const Types::U32 x, const Types::U32 y, const vector4& pixel);

    /*!
        \brief set one pixel with RGB
        \param x column of the pixel, from left to right
        \param y row of the pixel, from bottom to top
        \param pixel the pixel value, only rgb is setted
    */
    //void SetPixel(const Types::U32 x, const Types::U32 y, const RGB& pixel);
    void SetPixel(const Types::U32 x, const Types::U32 y, const vector3& pixel);

    /*!
        \brief set alpha of one pixel with single value.
        \param x column of the pixel, from left to right
        \param y row of the pixel, from bottom to top
        \param alpha the new value of the alpha channel.
    */
    void SetAlpha(const Types::U32 x, const Types::U32 y, const Types::F32& alpha);

    /*!
        \brief get one pixel value
        \param x column of the pixel, from left to right
        \param y row of the pixel, from bottom to top
    */
    vector4 GetPixel(const Types::U32 x, const Types::U32 y) const;

    Types::U32 GetWidth() const { return m_width; }
    Types::U32 GetHeight() const { return m_height; }
    
    /*!
        \brief return byte pixel address for entire image, in which each channel in pixel is stored by one byte,
        each pixel has four channel, pixels placed from top left go right, and then step down.
        There is no padding between each row.
    */
    unsigned char * GetRawData();

    /*!
        \brief reset the image.
        \param width width of the image
        \param height height of the image
    */
    void Init(const Types::U32 width, const Types::U32 height, const RGBA& initColor = RGBA::BLACK);

    /*!
        \brief update float pixel to byte pixel, (usage: get ready to store image to file)
    */
    void FloatPixelToBytePixel();

    /*!
        \brief byte pixel to float pixel, (usage: load image from file).
    */
    void BytePixelToFloatPixel();

protected:
    /*!
        \brief convert 2D indices into 1D index to search pixels in the m_canvas.
        \param x horizontal value from left to right
        \param y vertical value from bottom to top
    */
    const Types::U32 To1DArrIndex(const Types::U32 x, const Types::U32 y) const;

    /*!
        \brief ensure value of each channel is in [0.0, 1.0f]
        v should has a member named as m_arr, whose type is float[]
    */
    template<typename FloatArrType>
    static void ClampChannels(FloatArrType& v)
    {
        for (int i = 0; i < (sizeof(v.m_arr) >> 2); ++i)
        {
            if (v.m_arr[i] < 0.0f)
            {
                v.m_arr[i] = 0.0f;
            }
            else if (v.m_arr[i] > 1.0f)
            {
                v.m_arr[i] = 1.0f;
            }
        }// end for
    }

};

}// namespace CommonClass
