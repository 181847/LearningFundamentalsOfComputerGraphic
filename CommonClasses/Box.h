#pragma once
#include "vector3.h"

namespace CommonClass
{

/*!
    \brief a Box is axis align box, defined by two point.
*/
class Box
{
public:
    vector3 m_minPoint;
    vector3 m_maxPoint;

public:
    Box(const vector3& minPoint, const vector3& maxPoint);
    ~Box();
};

} // namespace CommonClass

