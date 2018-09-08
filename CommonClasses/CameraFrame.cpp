#include "CameraFrame.h"

namespace CommonClass
{

CameraFrame::CameraFrame(const vector3 & origin, const vector3 & lookAt, const vector3 & lookUp)
    :Frame(origin), m_lookUp(Normalize(lookUp)), m_lookAt(lookAt)
{
    RebuildFrameDetail();
}

CameraFrame::~CameraFrame()
{
}

void CameraFrame::RebuildFrameDetail()
{
    vector3 toCamera = m_origin - m_lookAt;
    Types::F32 length = dotProd(toCamera, toCamera);

    // deal with the situation where origin overlaps looking target, 
    if (length < 1e-8f)
    {
        m_w = vector3::AXIS_Z;
    }
    else
    {
        m_w = Normalize(m_origin - m_lookAt);// notic: right hand system is used, Z(or w) should point out of the screen
    }

    m_u = Normalize(crossProd(m_lookUp, m_w));
    m_v = Normalize(crossProd(m_w, m_u));

    CheckPerpendicular();
}

}// namespace CommonClass
