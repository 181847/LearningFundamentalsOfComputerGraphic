#include "Filter.h"
#include "array"
#include "DebugConfigs.h"

namespace CommonClass
{

Filter::Filter(const unsigned int & step, const unsigned int & width, const unsigned int & height, const unsigned int & padding)
    :m_step(step), m_width(width), m_height(height), m_padding(padding)
{
    // empty
}

Filter::~Filter()
{
    // empty
}

Image Filter::Convolve(const Image & img)
{
    const Loc IMG_S = { static_cast<int>(img.GetWidth()), static_cast<int>(img.GetHeight()) };
    const Loc CON_S = ConvolutionSize(IMG_S);

    Image convolutionResult(CON_S.X, CON_S.Y, RGBA::BLACK);

    Loc start = { -static_cast<int>(m_padding), -static_cast<int>(m_padding) };

    Loc rowHead = start;
    for (unsigned int y = 0; y < CON_S.Y; ++y)
    {
        Loc horizontalScaner = rowHead;
        for (unsigned int x = 0; x < CON_S.X; ++x)
        {
            convolutionResult.SetPixel(x, y, Step(img, horizontalScaner));
            horizontalScaner = horizontalScaner.Go<Loc::RIGHT>(static_cast<int>(m_step));
            if (x % 200 == 0)
            {
                printf("x = %4d, y = %4d\r", x, y);
            }
        }
        rowHead = rowHead.Go<Loc::UP>(static_cast<int>(m_step));
    }

    return convolutionResult;
}

RGBA Filter::Step(const Image& img, const Loc& location)
{
    auto sampleLocs = GenerateSampleLocations(location);

    std::array<float, 4> channels = {0.0f, 0.0f, 0.0f, 0.0f};

    for (auto const& pos : sampleLocs)
    {
        auto sample = TryGetPixel(img, pos);

        for (int i = 0; i < 4; ++i)
        {
            channels[i] += sample.m_arr[i];
        }
    }
    
    const int numSamples = sampleLocs.size();
    for (auto& ch : channels)
    {
        ch /= numSamples;
    }
    
    return RGBA(channels[0], channels[1], channels[2], channels[3]);
}

RGBA Filter::TryGetPixel(const Image& img, const Loc& location)
{
    const Loc IMG_S = { img.GetWidth(), img.GetHeight() };
    if (!IMG_S.IsInclude(location)) // if out of boundary
    {
        return RGBA::BLACK;
    }
    else
    {
        return img.GetPixel(location.X, location.Y);
    }
}

Filter::Loc Filter::ConvolutionSize(const Loc& imgSize) const
{
    Loc ret;
    ret.X = std::floor((imgSize.X + 2 * m_padding -  m_width) * 1.0f / m_step + 1);
    ret.Y = std::floor((imgSize.Y + 2 * m_padding - m_height) * 1.0f / m_step + 1);
    return ret;
}

std::vector<CommonClass::Filter::Loc> Filter::GenerateSampleLocations(const Loc& bottomLeft)
{
    std::vector<Loc> retList;
    
    Loc step;
    Loc RowHead = bottomLeft;
    for (unsigned int y = 0; y < m_height; ++y)
    {
        Loc horizontalScaner = RowHead;

        for (unsigned int x = 0; x < m_width; ++x)
        {
            retList.push_back(horizontalScaner);
            horizontalScaner = horizontalScaner.Go<Loc::RIGHT>();
        }

        RowHead = RowHead.Go<Loc::UP>();
    }

    return retList;
}

}// namespace CommonClass
