#pragma once
#include "ColorTemplate.h"
#include <string>
#include <vector>

namespace CommonClass
{

class Image
{
public:

protected:
    /*!
        \brief 1D array (access data through 2D method) store pixels
    */
    std::vector<Pixel> m_canvas;

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
        \brief is this image contain validate data.
    */
    bool IsValid() const;
    
    /*!
        \brief save the image to the file unicode version
        \param filePath the file path to save the image
        this function will overwrite the file with same name.
    */
    void SaveTo(const std::wstring& filePath) const;
    
    /*!
        \brief save the image to the file ascii version
        \param filePath the file path to save the image
        this function will overwrite the file with same name.
    */
    void SaveTo(const std::string& filePath) const;

    /*!
        \brief clear all pixel color.
    */
    void ClearPixel(const Pixel& pixel);

    /*!
        \brief set one pixel with RGBA
        \param x column of the pixel, from left to right
        \param y row of the pixel, from bottom to top
        \param pixel the pixel value
    */
    void SetPixel(const Types::U32 x, const Types::U32 y, const RGBA& pixel);

    /*!
        \brief set one pixel with RGB
        \param x column of the pixel, from left to right
        \param y row of the pixel, from bottom to top
        \param pixel the pixel value, only rgb is setted
    */
    void SetPixel(const Types::U32 x, const Types::U32 y, const RGB& pixel);

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
    RGBA GetPixel(const Types::U32 x, const Types::U32 y) const;

    /*!
        \brief get the raw pixel value, whose channel is stored in one byte
        \param x column of the pixel, from left to right
        \param y row of the pixel, from bottom to top
    */
    const Pixel& GetRawPixel(const Types::U32 x, const Types::U32 y) const;

    Types::U32 GetWidth() const { return m_width; }

    Types::U32 GetHeight() const { return m_height; }
    
    /*!
        \brief return the pixel data address of current image
    */
    unsigned char * GetRawData();

protected:
    /*!
        \brief reset the image.
        \param width width of the image
        \param height height of the image
    */
    void Init(const Types::U32 width, const Types::U32 height, const RGBA& initColor = RGBA::BLACK);

    /*!
        \brief convert 2D indices into 1D index to search pixels in the m_canvas.
        \param x horizontal value from left to right
        \param y vertical value from bottom to top
    */
    const Types::U32 To1DArrIndex(const Types::U32 x, const Types::U32 y) const;



};

}// namespace CommonClass
