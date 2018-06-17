#include "ScreenSpaceVertexTemplate.h"
#include <assert.h>

namespace CommonClass
{
    
unsigned int ScreenSpaceVertexTemplate::NumRestFloat(const unsigned int realVertexSizeInBytes)
{
    assert(realVertexSizeInBytes % 4 == 0 && "the vertex size in Bytes is not the times of four.");
    return (realVertexSizeInBytes - sizeof(m_posH)) / 4;
}


void Interpolate2(const ScreenSpaceVertexTemplate * pv1, const ScreenSpaceVertexTemplate * pv2, ScreenSpaceVertexTemplate * pResult, const Types::F32 t, const unsigned int realVertexSizeInBytes)
{
    assert(pv1      != nullptr);
    assert(pv2      != nullptr);
    assert(pResult  != nullptr);
    assert(realVertexSizeInBytes >= sizeof(ScreenSpaceVertexTemplate::m_posH));
    const unsigned int numRestFloats = ScreenSpaceVertexTemplate::NumRestFloat(realVertexSizeInBytes);

    const Types::F32 v = 1.0f - t;

    // interpolate screen space coordinate.
    // WARNING!! (scalar * hvector) or (hvector * scalar) will not affect w component, so we multiply their components manually.
    //pResult->m_posH = t * pv1->m_posH + v * pv2->m_posH;
    for (int i = 0; i < 4; ++i)
    {
        pResult->m_posH.m_arr[i] = v * pv1->m_posH.m_arr[i] + t * pv2->m_posH.m_arr[i];
    }

    // interpolate rest floats.
    for (unsigned int i = 0; i < numRestFloats; ++i)
    {
        pResult->m_restDates[i] = v * pv1->m_restDates[i] + t * pv2->m_restDates[i];
    } // end for
}

void Interpolate3(
    const ScreenSpaceVertexTemplate*    pv1,
    const ScreenSpaceVertexTemplate*    pv2,
    const ScreenSpaceVertexTemplate*    pv3,
    ScreenSpaceVertexTemplate*          pResult,
    const Types::F32                    alpha,
    const Types::F32                    beta,
    const Types::F32                    gamma,
    const unsigned int                  realVertexSizeInBytes)
{
    
    assert(pv1      != nullptr);
    assert(pv2      != nullptr);
    assert(pv3      != nullptr);
    assert(pResult  != nullptr);
    assert(realVertexSizeInBytes >= sizeof(ScreenSpaceVertexTemplate::m_posH));
    const unsigned int numRestFloats = ScreenSpaceVertexTemplate::NumRestFloat(realVertexSizeInBytes);

    for (int i = 0; i < 4; ++i)
    {
        pResult->m_posH.m_arr[i] = alpha * pv1->m_posH.m_arr[i] + beta * pv2->m_posH.m_arr[i] + gamma * pv3->m_posH.m_arr[i];
    }

    // interpolate rest floats.
    for (unsigned int i = 0; i < numRestFloats; ++i)
    {
        pResult->m_restDates[i] = alpha * pv1->m_restDates[i] + beta * pv2->m_restDates[i] + gamma * pv3->m_restDates[i];
    } // end for
}

} // namespace CommonClass
