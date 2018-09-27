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
    Types::F32 sqLength = dotProd(toCamera, toCamera);

    // camera location overlap with target, using default viewing position.
    if (sqLength < 1e-8f)
    {
        m_u = vector3::AXIS_X;
        m_v = vector3::AXIS_Y;
        m_w = vector3::AXIS_Z;
    }
    else
    {
        m_w = toCamera / std::sqrt(sqLength);

        // when look up vector overlap with axis W
        vector3 dummyU = crossProd(m_lookUp, m_w);
        if (dummyU == vector3::ZERO)
        {
            // current look up vector has some error, try other look up vector.
            vector3 secondLookUp = vector3::AXIS_X;
            if (m_w == secondLookUp)
            {
                secondLookUp = vector3::AXIS_Y;
            }

            dummyU = crossProd(secondLookUp, m_w);
        }

        m_u = Normalize(dummyU);
        m_v = Normalize(crossProd(m_w, m_u));
    }

    CheckPerpendicular();
}

}// namespace CommonClass
