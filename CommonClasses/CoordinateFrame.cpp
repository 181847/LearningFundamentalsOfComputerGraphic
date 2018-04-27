#include "CoordinateFrame.h"

namespace CommonClass
{

CoordinateFrame::CoordinateFrame(const vector3 u, const vector3 v, const vector3 e)
    :m_e(e)
{
    m_u = Normalize(u);
    m_v = Normalize(v - dotProd(m_u, v) * m_u);
    m_w = crossProd(m_u, m_v);
}

CoordinateFrame::~CoordinateFrame()
{
}

Transform CoordinateFrame::GetTransformLocalToWorld()
{
    return Transform(
        m_u.m_x, m_v.m_x, m_w.m_x, m_e.m_x,
        m_u.m_y, m_v.m_y, m_w.m_y, m_e.m_y,
        m_u.m_z, m_v.m_z, m_w.m_z, m_e.m_z,
        0,       0,       0,       1);
}

Transform CoordinateFrame::GetTransformWorldToLocal()
{
    return Transform(
        m_u.m_x, m_u.m_x, m_u.m_x, - m_e.m_x,
        m_v.m_y, m_v.m_y, m_v.m_y, - m_e.m_y,
        m_w.m_z, m_w.m_z, m_w.m_z, - m_e.m_z,
        0,       0,       0,       1);
}

} // namespace CommonClass
