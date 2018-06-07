#pragma once
/*!
    \brief this head file define some help function, such as a function to format std::string as printf
*/

#include <string>
#include <memory>
#include <functional>
#include "CommonTypes.h"

/*!
    \brief printf for std::string
    Usage:
        std::string s = string_format("%f, %d -- [%f, %f]", 1.0, 2, 3.3f, 4.4f);
        
        s == "1.0, 2 -- [3.3, 4.4]";
    The code is coming from: https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
*/
template<typename ... Args>
std::string string_format(const std::string& fmt, Args ... args)
{
    const size_t size = 1 + snprintf(nullptr, 0, fmt.c_str(), args ...);
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, fmt.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1); // construct with out the last '\0'
}


/*!
    \brief help to generate sphere ray lines to be drawn.
    \param drawLine the function to receive the line start and end points.
    \param centerLocationX the center x location of the sphere.
    \param centerLocationY the center y location of the sphere.
    \param segmentLength the length of each segment
    \param startInnerRadius the minimum radius of the spheres
    \param numRounds how many rounds to draw.
*/
void SphereRay(
    std::function<void(const Types::F32 x0, const Types::F32 y0, const Types::F32 x1, const Types::F32 y1, const unsigned int roundIndex, const unsigned int lineIndex)>
    drawLine,
    const Types::F32 centerLocationX,
    const Types::F32 centerLocationY,
    const Types::F32 segmentLength = 35.0f,
    const Types::F32 startInnerRadius = 30.0f,
    const Types::U32 numRounds = 5,
    const Types::F32 radioOffset = 0.0f)
{
    Types::F32 deltaTheta = 2.0f * Types::Constant::PI_F / 64.0f;
    Types::F32 theta = 0.0f;
    Types::F32 rIn = startInnerRadius;
    Types::F32 rOut = rIn + segmentLength;

    Types::F32 x0, y0, x1, y1;
    for (unsigned int i = 0; i < numRounds; ++i)
    {
        for (unsigned int j = 0; j < 64; ++j)
        {
            //BREAK_POINT_IF(i == 3 && j == 0 && centerLocationX == 0.5f);
            theta = radioOffset + deltaTheta * j + (i % 2) * deltaTheta * 0.5f;
            x0 = centerLocationX + rIn  * std::cos(theta);
            y0 = centerLocationY + rIn  * std::sin(theta);
            x1 = centerLocationX + rOut * std::cos(theta);
            y1 = centerLocationY + rOut * std::sin(theta);

            drawLine(x0, y0, x1, y1, i, j);
        }
        rIn = rOut;
        rOut += segmentLength;
    } // end outer for loop
}
#define HELP_SPHERE_RAY_LAMBDA_PARAMETERS const Types::F32 x0, const Types::F32 y0, const Types::F32 x1, const Types::F32 y1, const unsigned int roundIndex, const unsigned int lineIndex

void SphereRayTri(
    std::function<void(
        const Types::F32 x0, const Types::F32 y0,
        const Types::F32 x1, const Types::F32 y1,
        const Types::F32 x2, const Types::F32 y2,
        const unsigned int roundIndex, const unsigned int triIndex)>
    drawTri,
    const Types::F32 centerLocationX,
    const Types::F32 centerLocationY,
    const Types::F32 segmentLength = 35.0f,
    const Types::F32 startInnerRadius = 30.0f,
    const Types::U32 numRounds = 5,
    const Types::F32 radioOffset = 0.0f)
{
    Types::F32 deltaTheta   = 2.0f * Types::Constant::PI_F / 64.0f;
    Types::F32 halfDT       = deltaTheta * 0.5f;
    Types::F32 quadDT       = deltaTheta * 0.25f;
    Types::F32 rIn          = startInnerRadius;
    Types::F32 rOut         = rIn + segmentLength;

    Types::F32 x0, y0, x1, y1, x2, y2;
    for (unsigned int i = 0; i < numRounds; ++i)
    {
        for (unsigned int j = 0; j < 64; ++j)
        {
            Types::F32 offSetOfTheta = radioOffset + (i % 2) * halfDT;
            Types::F32 theta1 = offSetOfTheta + deltaTheta * j;
            Types::F32 theta2 = theta1 + deltaTheta;
            Types::F32 theta3 = theta1 + halfDT;

            x0 = centerLocationX + rIn  * std::cos(theta2);
            y0 = centerLocationY + rIn  * std::sin(theta2);
            x1 = centerLocationX + rIn  * std::cos(theta1);
            y1 = centerLocationY + rIn  * std::sin(theta1);
            x2 = centerLocationX + rOut * std::cos(theta3);
            y2 = centerLocationY + rOut * std::sin(theta3);

            drawTri(x0, y0, x1, y1, x2, y2, i, j);
        }
        rIn = rOut;
        rOut += segmentLength;
    } // end outer for loop
}
#define HELP_SPHERE_RAY_TRI_LAMBDA_PARAMETERS const Types::F32 x0, const Types::F32 y0, const Types::F32 x1, const Types::F32 y1, const Types::F32 x2, const Types::F32 y2, const unsigned int roundIndex, const unsigned int lineIndex
