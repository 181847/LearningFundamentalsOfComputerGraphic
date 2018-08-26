#include "CameraFrame.h"

namespace CommonClass
{

CameraFrame::CameraFrame(const vector3 & origin, const vector3 & lookAt, const vector3 & lookUp)
    :Frame(origin), m_lookUp(Normalize(lookUp))
{
    m_w = Normalize(m_origin - lookAt);// notic: right hand system is used, Z(or w) should point out of the screen
    m_u = Normalize(crossProd(m_lookUp, m_w));
    m_v = Normalize(crossProd(m_w, m_u));

    CheckPerpendicular();
}

CameraFrame::~CameraFrame()
{
}

}// namespace CommonClass
