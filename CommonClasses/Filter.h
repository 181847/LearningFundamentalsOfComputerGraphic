#pragma once
#include "Image.h"

namespace CommonClass
{

/*!
    \brief a Filter is used to convolve a image and return another image.
*/
class Filter
{
protected:
    unsigned int m_step     = 1;
    unsigned int m_width    = 3;
    unsigned int m_height   = 3;
    unsigned int m_padding  = 0;

public:
    /*!
        \brief the help struct to store 2D location.
    */
    struct Loc
    {
        union
        {
            struct
            {
                int X;
                int Y;
            };
            int ARR[2];
        };

        bool operator == (const Loc& other)
        {
            return X == other.X && Y == other.Y;
        }

        /*!
            \brief think this Loc as a boundary in {(0, 0), ..., (X - 1, Y - 1)},
            return whether the point is in the set.
        */
        bool IsInclude(const Loc& point) const
        {
            return 
                ! (point.X < 0
                || point.X >= X
                || point.Y < 0
                || point.Y >= Y);
        }

        enum
        {
            UP = 0,
            DOWN,
            LEFT,
            RIGHT
        };
        
        template<int D>
        Loc Go(int step = 1) const
        {
            return {
                X + step * (       (D >> 1) * ((D & 1) * +2 - 1) ),
                Y + step * ( (1 - (D >> 1)) * ((D & 1) * -2 + 1) )
            };
        }
    };

public:
    Filter(
        const unsigned int& step, 
        const unsigned int& width, 
        const unsigned int& height,
        const unsigned int& padding);

    ~Filter();

    /*!
        \brief convolve the image and return a new image.
    */
    Image Convolve(const Image& img);

protected:
    /*!
        \brief for each convolve the pixel in location(x,y)
        \param location the location is the virtual location in the img region,
               it can extend the image size, take the location as the left bottom corner
               of the filter and filter once of the img.
    */
    virtual vector4 Step(const Image& img, const Loc& location);

    /*!
        \brief try to get the pixel color of the img, if location is out of the boundary,
        return some padding value, this function can be override by sub classes.
    */
    virtual vector4 TryGetPixel(const Image& img, const Loc& location);

    /*!
        \brief calculate the convolution result image size.
    */
    Loc ConvolutionSize(const Loc& imgSize) const;

public:
    /*!
        \brief generate sample positions by the top left corner of one step of convolution.
    */
    std::vector<Loc> GenerateSampleLocations(const Loc& bottomLeft);
};

}// namespace CommonClass
