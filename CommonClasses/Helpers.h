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
    const Types::F32 radioOffset = 0.0f);
#define HELP_SPHERE_RAY_LAMBDA_PARAMETERS const Types::F32 x0, const Types::F32 y0, const Types::F32 x1, const Types::F32 y1, const unsigned int roundIndex, const unsigned int lineIndex

/*!
    \brief help to generate sphere ray lines to be drawn.
    \param drawTri the function to receive the triangle data and index(not the index of vertex) about the triangle.
    \param centerLocationX the center x location of the sphere.
    \param centerLocationY the center y location of the sphere.
    \param segmentLength the length of each segment
    \param startInnerRadius the minimum radius of the spheres
    \param numRounds how many rounds to draw.
*/
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
    const Types::F32 radioOffset = 0.0f);
#define HELP_SPHERE_RAY_TRI_LAMBDA_PARAMETERS const Types::F32 x0, const Types::F32 y0, const Types::F32 x1, const Types::F32 y1, const Types::F32 x2, const Types::F32 y2, const unsigned int roundIndex, const unsigned int lineIndex
