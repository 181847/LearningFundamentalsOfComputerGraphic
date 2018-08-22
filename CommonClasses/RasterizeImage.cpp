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
CommonClass::RGB Interpolate(const CommonClass::RGB& src, const CommonClass::RGB& dest, const Types::F32 t)
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

const Types::F32 RasterizeImage::NORMALIZED_X_MIN = 0.0f;
const Types::F32 RasterizeImage::NORMALIZED_X_MAX = 1.0f;
const Types::F32 RasterizeImage::NORMALIZED_Y_MIN = 0.0f;
const Types::F32 RasterizeImage::NORMALIZED_Y_MAX = 1.0f;

RasterizeImage::RasterizeImage(const Types::U32 pixelWidth, const Types::U32 pixelHeight, const RGBA& initColor)
    :Image(pixelWidth, pixelHeight, initColor)
{
    // empty
}

RasterizeImage::~RasterizeImage()
{
    // empty
}

void RasterizeImage::SetScissor(const ScissorRect & rect)
{
    assert(rect.m_minX < rect.m_maxX
        && rect.m_minY < rect.m_maxY
        && rect.m_minX >= 0
        && rect.m_maxX <= m_width - 1
        && rect.m_minY >= 0
        && rect.m_maxY <= m_height - 1);
    
    Types::F32 reciprocalWidth  = 1.0f / (m_width  - 1);
    Types::F32 reciprocalHeight = 1.0f / (m_height - 1);

    m_normalizedScissor.m_minX = rect.m_minX * reciprocalWidth;
    m_normalizedScissor.m_maxX = rect.m_maxX * reciprocalWidth;
    m_normalizedScissor.m_minY = rect.m_minY * reciprocalHeight;
    m_normalizedScissor.m_maxY = rect.m_maxY * reciprocalHeight;
}

void RasterizeImage::DrawLine(
    const Types::F32 x0, const Types::F32 y0, 
    const Types::F32 x1, const Types::F32 y1, 
    const CommonClass::RGB & color)
{
    Types::F32 p1 = x0 - x1;
    Types::F32 p2 = -p1;
    Types::F32 p3 = y0 - y1;
    Types::F32 p4 = -p3;

    Types::F32 q1 = x0 - m_normalizedScissor.m_minX;
    Types::F32 q2 = m_normalizedScissor.m_maxX - x0;
    Types::F32 q3 = y0 - m_normalizedScissor.m_minY;
    Types::F32 q4 = m_normalizedScissor.m_maxY - y0;

    Types::F32 posArr[5], negArr[5];
    unsigned int posind = 1, negind = 1;
    posArr[0] = 1.0f;
    negArr[0] = 0.0f;

    if ((p1 == 0.0f && q1 < 0.0f) || (p3 == 0.0f && q3 < 0))
    {
        // discard whole segment, parellele to clipping window
        return;
    }

    if (p1 != 0)
    {
        Types::F32 r1 = q1 / p1;
        Types::F32 r2 = q2 / p2;
        if (p1 < 0)
        {
            negArr[negind++]  = r1;
            posArr[posind++] = r2;
        }
        else
        {
            negArr[negind++]  = r2;
            posArr[posind++] = r1;
        }
    }

    if (p3 != 0)
    {
        Types::F32 r3 = q3 / p3;
        Types::F32 r4 = q4 / p4;
        if (p3 < 0)
        {
            negArr[negind++]  = r3;
            posArr[posind++] = r4;
        }
        else
        {
            negArr[negind++]  = r4;
            posArr[posind++] = r3;
        }
    }

    Types::F32 xn1, yn1, xn2, yn2;
    Types::F32 rn1, rn2;
    rn1 = LiangBarskyCliping::maxi(negArr, negind);
    rn2 = LiangBarskyCliping::mini(posArr, posind);

    if (rn1 > rn2)
    {
        // line is out of the clipping window
        return;
    }

    xn1 = x0 + p2 * rn1;
    yn1 = y0 + p4 * rn1;

    xn2 = x0 + p2 * rn2;
    yn2 = y0 + p4 * rn2;

    DrawBresenhamLine(
        static_cast<Types::I32>(xn1 * (m_width  - 1)), 
        static_cast<Types::I32>(yn1 * (m_height - 1)),
        static_cast<Types::I32>(xn2 * (m_width  - 1)),
        static_cast<Types::I32>(yn2 * (m_height - 1)),
        color
    );
}

void RasterizeImage::DrawBresenhamLine(const Types::I32 x0, const Types::I32 y0, const Types::I32 x1, const Types::I32 y1, const CommonClass::RGB & color)
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

void RasterizeImage::DrawWuXiaolinLine(Types::F32 x0, Types::F32 y0, Types::F32 x1, Types::F32 y1, const CommonClass::RGB& foreColor, const CommonClass::RGB& backgroundColor)
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

void RasterizeImage::DrawTriangle(Types::F32 x0, Types::F32 y0, Types::F32 x1, Types::F32 y1, Types::F32 x2, Types::F32 y2, const CommonClass::RGB & color)
{
    if (y0 < y1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    if (y1 < y2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);

        if (y0 < y1)
        {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
    }

    

    if (MathTool::almost_equal(y1, y2, FLOAT_CMP_ULP))
    {
        DrawTri_flatBottom(x0, y0, x1, y1, x2, y2, color);
    }
    else if (MathTool::almost_equal(y0, y1, 8))
    {
        DrawTri_flatTop(x2, y2, x0, y0, x1, y1, color);
    }
    else
    {
        const Types::F32 newX = x0 + (y1 - y0) * (x2 - x0) / (y2 - y0);
        DrawTri_flatBottom(x0, y0, x1, y1, newX, y1);
        DrawTri_flatTop(x2, y2, x1, y1, newX, y1);

    }
}

void RasterizeImage::DrawTri_flatBottom(Types::F32 x0, Types::F32 y0, Types::F32 x1, Types::F32 y1, Types::F32 x2, Types::F32 y2, const CommonClass::RGB & color)
{
    assert(MathTool::almost_equal(y1, y2, FLOAT_CMP_ULP));
    if (x1 > x2)
    {
        std::swap(x1, x2);
    }

    const Types::F32 dxLeft  = (x1 - x0) / (y2 - y0);
    const Types::F32 dxRight = (x2 - x0) / (y1 - y0);

    Types::F32 xLeft  = x0;
    Types::F32 xRight = x0;

    for (int y = static_cast<int>(y0); y >= y1; --y)
    {
        DrawFlatLine(static_cast<Types::U32>(xLeft), static_cast<Types::U32>(xRight), y, color);
        xLeft  -= dxLeft;
        xRight -= dxRight;
    }
}

void RasterizeImage::DrawTri_flatTop(Types::F32 x0, Types::F32 y0, Types::F32 x1, Types::F32 y1, Types::F32 x2, Types::F32 y2, const CommonClass::RGB & color)
{
    assert(MathTool::almost_equal(y1, y2, FLOAT_CMP_ULP));
    if (x1 > x2)
    {
        std::swap(x1, x2);
    }

    const Types::F32 dxLeft  = (x1 - x0) / (y2 - y0);
    const Types::F32 dxRight = (x2 - x0) / (y1 - y0);

    Types::F32 xLeft = x0;
    Types::F32 xRight = x0;

    for (int y = static_cast<int>(y0); y <= y1; ++y)
    {
        DrawFlatLine(static_cast<Types::U32>(xLeft), static_cast<Types::U32>(xRight), y, color);
        xLeft += dxLeft;
        xRight += dxRight;
    }
}

void RasterizeImage::DrawFlatLine(Types::U32 xLeft, Types::U32 xRight, Types::U32 y, const CommonClass::RGB & color)
{
    for (Types::U32 x = xLeft; x <= xRight; ++x)
    {
        SetPixel(x, y, color);
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

void RasterizeImage::plotLineLow(const Types::I32 x0, const Types::I32 y0, const Types::I32 x1, const Types::I32 y1, const CommonClass::RGB & color)
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

void RasterizeImage::plotLineHeight(const Types::I32 x0, const Types::I32 y0, const Types::I32 x1, const Types::I32 y1, const CommonClass::RGB & color)
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