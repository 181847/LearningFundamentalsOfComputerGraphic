#pragma once

#include "CommonTypes.h"
#include "hvector.h"

namespace CommonClass
{

/*!
    \brief EdgeEquation2D is a container of a 2D line equation, 
    this will be used in the rasterization of triangle in Pipline,
    to assist computing barycentric coordinate of pixels.
*/
class EdgeEquation2D
{
public:

    /*!
        \brief construct the equation by two point, we will only used the x\y to build the equation
        \param p1p2 two point on the line, here we only care about the x\y
    */
    EdgeEquation2D(const hvector& p1, const hvector& p2);
    ~EdgeEquation2D();

    /*!
        \brief evaluate the equation at (x, y)
        \param x/y the 2D coordinate
        \return m_A * x + m_B * y + m_C
        if (x,y) is on the line, return 0
        if (x,y) is on the left side of p1-->p2, return a positive float
        if (x,y) is on the right side of p1-->p2, return a negative float
    */
    Types::F32 eval(const Types::F32& x, const Types::F32& y);

private:
    
    /*!
        \brief coefficient of the line equation: m_A * x + m_B * y + m_C = 0
    */
    Types::F32 m_A, m_B, m_C;
};

}// namespace CommonClass
