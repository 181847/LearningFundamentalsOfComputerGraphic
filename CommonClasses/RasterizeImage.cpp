#include "RasterizeImage.h"


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