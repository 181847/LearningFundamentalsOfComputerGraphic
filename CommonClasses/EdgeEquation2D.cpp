#include "EdgeEquation2D.h"

namespace CommonClass
{

EdgeEquation2D::EdgeEquation2D(const hvector & p1, const hvector & p2)
    :m_A(p1.m_y - p2.m_y), m_B(p2.m_x - p1.m_x), m_C(p1.m_x * p2.m_y - p2.m_x * p1.m_y)
{
}

EdgeEquation2D::~EdgeEquation2D()
{
}

Types::F32 EdgeEquation2D::eval(const Types::F32 & x, const Types::F32 & y)
{
    return m_A * x + m_B * y + m_C;
}

}// namespace CommonClass
