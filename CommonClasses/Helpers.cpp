#include "Helpers.h"



void SphereRay(std::function<void(const Types::F32 x0, const Types::F32 y0, const Types::F32 x1, const Types::F32 y1, const unsigned int roundIndex, const unsigned int lineIndex)> drawLine, const Types::F32 centerLocationX, const Types::F32 centerLocationY, const Types::F32 segmentLength /*= 35.0f*/, const Types::F32 startInnerRadius /*= 30.0f*/, const Types::U32 numRounds /*= 5*/, const Types::F32 radioOffset /*= 0.0f*/)
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

void SphereRayTri(std::function<void(const Types::F32 x0, const Types::F32 y0, const Types::F32 x1, const Types::F32 y1, const Types::F32 x2, const Types::F32 y2, const unsigned int roundIndex, const unsigned int triIndex)> drawTri, const Types::F32 centerLocationX, const Types::F32 centerLocationY, const Types::F32 segmentLength /*= 35.0f*/, const Types::F32 startInnerRadius /*= 30.0f*/, const Types::U32 numRounds /*= 5*/, const Types::F32 radioOffset /*= 0.0f*/)
{
    Types::F32 deltaTheta = 2.0f * Types::Constant::PI_F / 64.0f;
    Types::F32 halfDT = deltaTheta * 0.5f;
    Types::F32 quadDT = deltaTheta * 0.25f;
    Types::F32 rIn = startInnerRadius;
    Types::F32 rOut = rIn + segmentLength;

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
