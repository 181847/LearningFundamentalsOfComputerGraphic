#include "PiplineStateObject.h"

namespace CommonClass
{

PiplineStateObject::PiplineStateObject()
{
}


PiplineStateObject::~PiplineStateObject()
{
}

void PiplineStateObject::SetViewport(Viewport viewport)
{
    m_viewport = viewport;
    m_viewportTransform = 
        Transform::Viewport(
            m_viewport.left,   m_viewport.right,
            m_viewport.bottom, m_viewport.top);
}

} // namespace CommonClass
