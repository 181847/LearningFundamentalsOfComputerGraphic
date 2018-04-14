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

namespace LiangBarskyCliping
{

/*!
    \brief max value in the arr for n elements.
    \param arr start of the array
    \param n num elements to compare
*/
Types::F32 maxi(const Types::F32 arr[], const unsigned int n)
{
    // m is initialized to 0, this is designed by the Liang-Barsky-Algorithm
    Types::F32 m = 0.0f;
    for (unsigned int i = 0; i < n; ++i)
    {
        if (m < arr[i])
        {
            m = arr[i];
        }
    }
    return m;
}


/*!
    \brief max value in the arr for n elements.
    \param arr start of the array
    \param n num elements to compare
*/
Types::F32 mini(const Types::F32 arr[], const unsigned  n)
{
    // m is initialized to 1.0f, this is designed by the Liang-Barsky-Algorithm
    Types::F32 m = 1.0f;
    for (unsigned int i = 0; i < n; ++i)
    {
        if (m > arr[i])
        {
            m = arr[i];
        }
    }
    return m;
}

}

namespace CommonClass
{

RasterizeImage::RasterizeImage(const Types::U32 pixelWidth, const Types::U32 pixelHeight, const RGBA& initColor)
    :Image(pixelWidth, pixelHeight, initColor)
{
    // empty
}

RasterizeImage::~RasterizeImage()
{
    // empty
}

void RasterizeImage::DrawLine(const Types::F32 x0, const Types::F32 y0, const Types::F32 x1, const Types::F32 y1, const RGB & color)
{
    Types::F32 p1 = x0 - x1;
    Types::F32 p2 = -p1;
    Types::F32 p3 = y0 - y1;
    Types::F32 p4 = -p3;

    Types::F32 q1 = x0 - NORMALIZED_X_MIN;
    Types::F32 q2 = NORMALIZED_X_MAX - x0;
    Types::F32 q3 = y0 - NORMALIZED_Y_MIN;
    Types::F32 q4 = NORMALIZED_Y_MAX - y0;

    Types::F32 posArr[5], negArr[5];
    unsigned int posind = 1, ngind = 1;
    posArr[0] = 1.0f;
    negArr[0] = 0.0f;
    
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

void RasterizeImage::DrawBresenhamLine_inOneCall(Types::I32 x0, Types::I32 y0, Types::I32 x1, Types::I32 y1, const RGB & color)
{

//#define MY_PROFILE

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
    
#ifdef MY_PROFILE

    Types::I32 deltaX = x1 - x0;
    Types::I32 deltaY = std::abs(y1 - y0);
    Types::I32 twoDy = 2 * deltaY;
    Types::I32 twoDx = 2 * deltaX;
    Types::I32 deltaErr = twoDy;
    Types::I32 y = y0;
    Types::I32 error = -deltaX;
    Types::I32 ystep = y0 < y1 ? 1 : -1;

    for (auto x = x0; x <= x1; ++x)
    {
        if (steep)
        {
            SetPixel(y, x, color);
        }
        else
        {
            SetPixel(x, y, color);
        }

        error += twoDy;

        if (error > 0)
        {
            y += ystep;
            error -= twoDx;
        }
    }

#else

    Types::I32 dx = x1 - x0;
    Types::I32 twoDy = std::abs(2 * (y1 - y0));
    Types::I32 yi = y1 > y0 ? 1 : -1;
    Types::I32 twoDx = 2 * dx;
    Types::I32 y = y0;
    Types::I32 error = twoDy - dx;
    Types::I32 twoDyMinusTwoDx = error - dx;

    for (auto x = x0; x <= x1; ++x)
    {
        if (steep)
        {
            SetPixel(y, x, color);
        }
        else
        {
            SetPixel(x, y, color);
        }

        if (error > 0)
        {
            y += yi;
            error += twoDyMinusTwoDx;
        }
        else
        {
            error += twoDy;
        }
    }

#endif// MY_PROFILE

    
    
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
    Types::I32 dx = x1 - x0;
    Types::I32 twoDy = 2 * (y1 - y0);
    Types::I32 yi = 1;
    if (twoDy < 0)
    {
        yi = -1;
        twoDy = -twoDy;
    }
    Types::I32 y = y0;
    Types::I32 D = twoDy - dx;
    Types::I32 twoDyMinusTwoDx = D - dx;    // equals ( 2 * dy - 2 * dx )
    for (Types::I32 x = x0; x <= x1; ++x)
    {
        SetPixel(x, y, color);
        if (D > 0)
        {
            y += yi;
            D += twoDyMinusTwoDx;
        }
        else
        {
            D += twoDy;
        }
    }
}

void RasterizeImage::plotLineHeight(const Types::I32 x0, const Types::I32 y0, const Types::I32 x1, const Types::I32 y1, const RGB & color)
{
    Types::I32 dy = y1 - y0;
    Types::I32 twoDx = 2 * (x1 - x0);
    //Types::I32 twoDy = 2 * dy;
    Types::I32 xi = 1;
    if (twoDx < 0)
    {
        xi = -1;
        twoDx = -twoDx;
    }

    Types::I32 D = twoDx - dy;
    Types::I32 x = x0;
    Types::I32 twoDxMinusTwoDy = D - dy;    // equals ( 2 * dx - 2 * dy )
    for (Types::I32 y = y0; y <= y1; ++y)
    {
        SetPixel(x, y, color);
        if (D > 0)
        {
            x += xi;
            D += twoDxMinusTwoDy;
        }
        else
        {
            D += twoDx;
        }
    }
}

} // namespace CommonClass