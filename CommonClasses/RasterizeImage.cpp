#include "RasterizeImage.h"

namespace WuXiaolin
{

/*!
    \brief integer part of x
*/
Types::U32 ipart(const Types::F32 x)
{
    return static_cast<Types::U32>(std::floor(x));
}

Types::F32 round(const Types::F32 x)
{
    return std::floor(x + 0.5f);
}

/*!
    \brief fractional part of x
*/
Types::F32 fpart(const Types::F32 x)
{
    return x - std::floor(x);
}

Types::F32 rfpart(const Types::F32 x)
{
    return 1.0f - fpart(x);
}

/*!
    \brief interpolate between two color.
    \param src source color
    \param dest destination color
    \param t interpolation parameter
    t * src + (1 - t) * dest.
*/
RGB Interpolate(const RGB& src, const RGB& dest, const Types::F32 t)
{
    return t * src + (1.0f - t) * dest;
}

}

namespace CommonClass
{

RasterizeImage::RasterizeImage(const Types::U32 pixelWidth, const Types::U32 pixelHeight, const RGBA& initColor)
    :Image(pixelWidth, pixelHeight, initColor)
{
}

RasterizeImage::~RasterizeImage()
{
    // empty
}

void RasterizeImage::DrawBresenhamLine(const Types::I32 x0, const Types::I32 y0, const Types::I32 x1, const Types::I32 y1, const RGB & color)
{
    if (IsOutOfRange(x0, y0) || IsOutOfRange(x1, y1))
    {
        throw std::exception("RasterizeImage draw bresenham line failed, point location out of range.");
    }
    if (std::abs(y1 - y0) < std::abs(x1 - x0))
    {
        if (x0 > x1)
        {
            plotLineLow(x1, y1, x0, y0, color);
        }
        else
        {
            plotLineLow(x0, y0, x1, y1, color);
        }
    }
    else // slope between (-inf, -1] [1, +inf)
    {
        if (y0 > y1)
        {
            plotLineHeight(x1, y1, x0, y0, color);
        }
        else
        {
            plotLineHeight(x0, y0, x1, y1, color);
        }
    }
}

void RasterizeImage::DrawWuXiaolinLine(Types::F32 x0, Types::F32 y0, Types::F32 x1, Types::F32 y1, const RGB& foreColor, const RGB& backgroundColor)
{
    bool steep = std::abs(y1 - y0) > abs(x1 - x0);

    if (steep)
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    Types::F32 dx = x1 - x0;
    Types::F32 dy = y1 - y0;
    Types::F32 gradient = dy / dx;

    if (dx == 0.0f)
    {
        gradient = 1.0f;
    }

    // handle first endpoint
    Types::F32 xend = WuXiaolin::round(x0);
    Types::F32 yend = y0 + gradient * (xend - x0);
    Types::F32 xgap = WuXiaolin::rfpart(x0 + 0.5f);
    Types::U32 xpxl1 = static_cast<Types::U32>(xend);
    Types::U32 ypxl1 = WuXiaolin::ipart(yend);
    Types::F32 blendAlpha = 0.0f;
    if (steep)
    {
        SetPixel(ypxl1,     xpxl1, WuXiaolin::Interpolate(foreColor, backgroundColor, WuXiaolin::rfpart(yend) * xgap));
        SetPixel(ypxl1 + 1, xpxl1, WuXiaolin::Interpolate(foreColor, backgroundColor, WuXiaolin:: fpart(yend) * xgap));
    }
    else
    {
        SetPixel(xpxl1,     ypxl1, WuXiaolin::Interpolate(foreColor, backgroundColor, WuXiaolin::fpart(yend) * xgap));
        SetPixel(xpxl1 + 1, ypxl1, WuXiaolin::Interpolate(foreColor, backgroundColor, WuXiaolin::fpart(yend) * xgap));
    }

    Types::F32 intery = yend + gradient;

    // handle second endpoint
    xend = WuXiaolin::round(x1);
    yend = y1 + gradient * (xend - x1);
    xgap = WuXiaolin::fpart(x1 + 0.5f);
    Types::U32 xpxl2 = static_cast<Types::U32>(xend);
    Types::U32 ypxl2 = WuXiaolin::ipart(yend);

    if (steep)
    {
        SetPixel(ypxl2,     xpxl2, WuXiaolin::Interpolate(foreColor, backgroundColor, WuXiaolin::rfpart(yend) * xgap));
        SetPixel(ypxl2 + 1, xpxl2, WuXiaolin::Interpolate(foreColor, backgroundColor, WuXiaolin::fpart(yend) * xgap));
    }
    else
    {
        SetPixel(xpxl2,     ypxl2, WuXiaolin::Interpolate(foreColor, backgroundColor, WuXiaolin::rfpart(yend) * xgap));
        SetPixel(xpxl2 + 1, ypxl2, WuXiaolin::Interpolate(foreColor, backgroundColor, WuXiaolin::fpart(yend) * xgap));
    }

    // main loop
    if (steep)
    {
        for (Types::U32 x = xpxl1 + 1; x <= xpxl2 - 1; ++x)
        {
            SetPixel(WuXiaolin::ipart(intery),      x, WuXiaolin::Interpolate(foreColor, backgroundColor, WuXiaolin::rfpart(intery)));
            SetPixel(WuXiaolin::ipart(intery) + 1,  x, WuXiaolin::Interpolate(foreColor, backgroundColor, WuXiaolin:: fpart(intery)));
            intery = intery + gradient;
        }
    }
    else
    {
        for (Types::U32 x = xpxl1 + 1; x <= xpxl2 - 1; ++x)
        {
            SetPixel(x, WuXiaolin::ipart(intery),      WuXiaolin::Interpolate(foreColor, backgroundColor, WuXiaolin::rfpart(intery)));
            SetPixel(x, WuXiaolin::ipart(intery) + 1,  WuXiaolin::Interpolate(foreColor, backgroundColor, WuXiaolin:: fpart(intery)));
            intery = intery + gradient;
        }
    }
}

bool RasterizeImage::IsOutOfRange(const Types::I32 x, const Types::I32 y)
{
    if (x < 0 || x > static_cast<Types::I32>(m_width - 1)
        || y < 0 || y > static_cast<Types::I32>(m_height - 1))
    {
        return true;
    }
    return false;
}

void RasterizeImage::plotLineLow(const Types::I32 x0, const Types::I32 y0, const Types::I32 x1, const Types::I32 y1, const RGB & color)
{
    /*
    plotLineLow(x0, y0, x1, y1)
        dx = x1 - x0
        dy = y1 - y0
        yi = 1
        if dy < 0
            yi = -1
            dy = -dy
        end if
        D = 2 * dy - dx
        y = y0

        for x from x0 to x1
            plot(x, y)
            if D > 0
                y = y + yi
                D = D - 2 * dx
            end if
            D = D + 2 * dy
    */

    Types::I32 dx = x1 - x0;
    Types::I32 dy = y1 - y0;
    Types::I32 yi = 1;
    if (dy < 0)
    {
        yi = -1;
        dy = -dy;
    }
    Types::I32 y = y0;
    Types::I32 D = 2 * dy - dx;
    for (Types::I32 x = x0; x < x1; ++x)
    {
        SetPixel(x, y, color);
        if (D > 0)
        {
            y += yi;
            D -= 2 * dx;
        }
        D += 2 * dy;
    }
}

void RasterizeImage::plotLineHeight(const Types::I32 x0, const Types::I32 y0, const Types::I32 x1, const Types::I32 y1, const RGB & color)
{
    /*
    plotLineHigh(x0,y0, x1,y1)
    dx = x1 - x0
    dy = y1 - y0
    xi = 1
    if dx < 0
        xi = -1
        dx = -dx
    end if
    D = 2*dx - dy
    x = x0

    for y from y0 to y1
        plot(x,y)
        if D > 0
            x = x + xi
            D = D - 2*dy
        end if
        D = D + 2*dx
    */
    Types::I32 dx = x1 - x0;
    Types::I32 dy = y1 - y0;
    Types::I32 xi = 1;
    if (dx < 0)
    {
        xi = -1;
        dx = -dx;
    }

    Types::I32 D = 2 * dx - dy;
    Types::I32 x = x0;
    for (Types::I32 y = y0; y <= y1; ++y)
    {
        SetPixel(x, y, color);
        if (D > 0)
        {
            x += xi;
            D -= 2 * dy;
        }
        D += 2 * dx;
    }
}

} // namespace CommonClass