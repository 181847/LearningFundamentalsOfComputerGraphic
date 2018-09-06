#include "Frame.h"
#include <assert.h>

namespace CommonClass
{

Frame::Frame(const vector3& origin, const vector3& u, const vector3& v) : m_origin(origin), m_u(u), m_v(v)
{
    RebuildByUV();
    CheckPerpendicular();
}

Frame::Frame()
    :Frame(vector3(0.0f, 0.0f, 0.0f) /* origin */)
{
    // empty
}

Frame::Frame(const vector3& origin)
    :m_origin(origin), m_u(vector3(1.0f, 0.0f, 0.0f)), m_v(vector3(0.0f, 1.0f, 0.0f)), m_w(vector3(0.0f, 0.0f, 1.0f))
{
    CheckPerpendicular();
}

Frame::~Frame()
{
}

CommonClass::Transform Frame::LocalToWorld()
{
    return Transform(
        m_u.m_x, m_v.m_x, m_w.m_x, m_origin.m_x,
        m_u.m_y, m_v.m_y, m_w.m_y, m_origin.m_y,
        m_u.m_z, m_v.m_z, m_w.m_z, m_origin.m_z,
           0.0f,    0.0f,    0.0f,         1.0f);
}

CommonClass::Transform Frame::WorldToLocal()
{
    Transform inverseRotation = Transform(
        m_u.m_x, m_v.m_x, m_w.m_x, 0.0f,
        m_u.m_y, m_v.m_y, m_w.m_y, 0.0f,
        m_u.m_z, m_v.m_z, m_w.m_z, 0.0f,
           0.0f,    0.0f,    0.0f, 1.0f).T();

    Transform inverseTranslate = Transform::InverseTranslation(m_origin.m_x, m_origin.m_y, m_origin.m_z);

    return inverseRotation * inverseTranslate;
}

void Frame::CheckPerpendicular() const
{
    assert(AlmostPerpendicular(m_u, m_v));
    assert(AlmostPerpendicular(m_u, m_w));
    assert(AlmostPerpendicular(m_v, m_w));
}

void Frame::RebuildByUV()
{
    m_u = Normalize(m_u);
    m_v = Normalize(m_v - m_u * dotProd(m_v, m_u));
    m_w = crossProd(m_u, m_v);
}

CommonClass::vector3 Frame::GetOrigin() const
{
    return m_origin;
}

}// namespace CommonClass
