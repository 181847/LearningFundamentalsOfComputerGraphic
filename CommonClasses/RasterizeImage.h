#pragma once
#include "CommonTypes.h"
#include "Image.h"

namespace CommonClass
{

/*!
    \brief RasterizeImage used to draw lines/triangles...
*/
class RasterizeImage
    :public Image
{
public:
    /*!
        \brief construct a RasterizeImage with pixel width and height
        \param pixelWidth num pixels in width
        \param pixelHeight num pixels in height
        \param initColor the initialization color of the image.
    */
    RasterizeImage(const Types::U32 pixelWidth, const Types::U32 pixelHeight, const RGBA& initColor = RGBA::BLACK);
    RasterizeImage(const RasterizeImage&) = delete;
    RasterizeImage& operator = (const RasterizeImage&) = delete;
    ~RasterizeImage();

    /*!
        \brief draw a line in the image.
        \param (x1, y1) start point
        \param (x2, y2) end point
        \param color the color of the line
        reference from: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
    */
    void DrawBresenhamLine(const Types::I32 x0, const Types::I32 y0, const Types::I32 x1, const Types::I32 y1, const RGB& color = RGB::BLACK);

    /*!
        \brief draw a line using Xiaolin Wu's line algorithm
        \param (x1, y1) start point
        \param (x2, y2) end point
        \param foreColor the color of the line
        \param backColor background color
        reference from: https://en.wikipedia.org/wiki/Xiaolin_Wu%27s_line_algorithm
    */
    void DrawWuXiaolinLine(Types::F32 x0, Types::F32 y0, Types::F32 x1, Types::F32 y1, const RGB& foreColor = RGB::BLACK, const RGB& backgroundColor = RGB::WHITE);

private:
    /*!
        \brief return whether the x are out of the image space.
        basically the x is bounded to [0, m_width - 1]
        the y is bounded to [0, m_height - 1]
    */
    bool IsOutOfRange(const Types::I32 x, const Types::I32 y);
    
    /*!
        \brief plot line for the low slope line (-1, +1), assists function DrawBresenhamLine()
    */
    void plotLineLow(const Types::I32 x0, const Types::I32 y0, const Types::I32 x1, const Types::I32 y1, const RGB& color = RGB::BLACK);

    /*!
        \brief plot line for the height slope line (-inf, -1] && [+1, inf), assists function DrawBresenhamLine()
    */
    void plotLineHeight(const Types::I32 x0, const Types::I32 y0, const Types::I32 x1, const Types::I32 y1, const RGB& color = RGB::BLACK);
};

} // name CommonClass
