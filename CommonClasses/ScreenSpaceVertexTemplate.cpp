#include "ScreenSpaceVertexTemplate.h"
#include <assert.h>

namespace CommonClass
{
    
unsigned int ScreenSpaceVertexTemplate::NumRestFloat(const unsigned int realVertexSizeInBytes)
{
    assert(realVertexSizeInBytes % 4 == 0 && "the vertex size in Bytes is not the times of four.");
    return (realVertexSizeInBytes - sizeof(m_posH)) / 4;
}


void Interpolate2(const ScreenSpaceVertexTemplate * pv1, const ScreenSpaceVertexTemplate * pv2, ScreenSpaceVertexTemplate * pResult, const Types::F32 u, const unsigned int realVertexSizeInBytes)
{
    const unsigned int numRestFloats = ScreenSpaceVertexTemplate::NumRestFloat(realVertexSizeInBytes);

    const Types::F32 v = 1.0f - u;

    pResult->m_posH = u * pv1->m_posH + v * pv2->m_posH;

    for (unsigned int i = 0; i < numRestFloats; ++i)
    {
        pResult->m_restDates[i] = u * pv1->m_restDates[i] + v * pv2->m_restDates[i];
    }

}

} // namespace CommonClass
