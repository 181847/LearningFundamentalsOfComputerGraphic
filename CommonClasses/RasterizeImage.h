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
    const Types::F32 NORMALIZED_X_MIN = 0.0f;
    const Types::F32 NORMALIZED_X_MAX = 1.0f;
    const Types::F32 NORMALIZED_Y_MIN = 0.0f;
    const Types::F32 NORMALIZED_Y_MAX = 1.0f;

public:
    /*!
        \brief construct a RasterizeImage with pixel width and height
        \param pixelWidth num pixels in width
        \param pixelHeight num pixels in height
        \param initColor the initialization color of the image, default to be (black and opaque.)
    */
    RasterizeImage(const Types::U32 pixelWidth, const Types::U32 pixelHeight, const RGBA& initColor = RGBA::BLACK);
    RasterizeImage(const RasterizeImage&) = delete;
    RasterizeImage& operator = (const RasterizeImage&) = delete;
    ~RasterizeImage();

    /*!
        \brief draw line (Bresenham Algorithm) with normalized coordinates, climp the portions out of range.
        \param (x0, y0) start point
        \param (x1, y1) end point
        \param color the color of the line
        the viewed space is x belong [NORMALIZED_X_MIN, NORMALIZED_X_MAX], and y belong [NORMALIZED_Y_MIN, NORMALIZED_Y_MAX].
    */
    void DrawLine(const Types::F32 x0, const Types::F32 y0, const Types::F32 x1, const Types::F32 y1, const RGB& color = RGB::BLACK);

    /*!
        \brief draw a line in the image.
        \param (x1, y1) start point
        \param (x2, y2) end point
        \param color the color of the line
        Warning: the location of the points should be limited in {(x, y)| 0 <= x <= (pixelWidth - 1) && 0 <= y <= (pixelHeight - 1) }
        or the function will throw an EXCEPTION.
        reference from: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
    */
    void DrawBresenhamLine(const Types::I32 x0, const Types::I32 y0, const Types::I32 x1, const Types::I32 y1, const RGB& color = RGB::BLACK);

    /*!
        \brief draw a line in the image.
        \param (x1, y1) start point
        \param (x2, y2) end point
        \param color the color of the line
        Another version of Bresenham algorithm which will do all the staff in one function call,
        this function is made to check out the performance difference from DrawBresenhamLine() which do the algorithm in seperate function call but more elegant in codes.
        Warning: the location of the points should be limited in {(x, y)| 0 <= x <= (pixelWidth - 1) && 0 <= y <= (pixelHeight - 1) }
        or the function will throw an EXCEPTION.
        reference from: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
    */
    void DrawBresenhamLine_inOneCall(Types::I32 x0, Types::I32 y0, Types::I32 x1, Types::I32 y1, const RGB& color = RGB::BLACK);

    /*!
        \brief draw a line using Xiaolin Wu's line algorithm
        \param (x1, y1) start point
        \param (x2, y2) end point
        \param foreColor the color of the line
        \param backColor background color
        beware of that the boundry of the points, this line algorithm use floating point number as location,
        so the boundry is not exactly the [0, 'width/height' - 1]��
        with my simple experiments, it should be in [0, 'width/height' - 2]��
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