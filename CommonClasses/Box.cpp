#include "Box.h"


namespace CommonClass
{


Box::Box(const vector3 & minPoint, const vector3 & maxPoint)
    :m_minPoint(minPoint), m_maxPoint(maxPoint)
{
}

Box::~Box()
{
}

} // namespace CommonClass
