#pragma once
#include "hvector.h"

namespace CommonClass
{

/*!
    \brief ScreenSpaceVertexTemplate is used in the rasterization stage.
    It was designed to have one hvector (the location in screen space), and a F32 array,
    the F32 array is for user access rest float data that will be interpolated before the pixel shader.
*/
struct ScreenSpaceVertexTemplate
{
public:
    CommonClass::hvector m_posH;
    Types::F32 m_restDates[1];

    /*!
        \brief return how many floats are left besides m_posH (hvector)
    */
    static unsigned int NumRestFloat(const unsigned int realVertexSizeInBytes);
};
// insurance
static_assert(sizeof(ScreenSpaceVertexTemplate) == (4 + 1) * sizeof(Types::F32), "size of template Vertex is wrong.");


/*!
    \brief interpolate attributes bewteen two vertex
    \param pv1 the first vertex
    \param pv2 the second vertex
    \param result the interpolation result
    \param t the interpolation coefficience (pv1-<0>---------<1>-pv2)
    \param realVertexSizeInBytes the real size of the vertex, all the vertex attributes are the float type.
*/
void Interpolate2(
    const ScreenSpaceVertexTemplate*    pv1, 
    const ScreenSpaceVertexTemplate*    pv2, 
    ScreenSpaceVertexTemplate*          pResult, 
    const Types::F32                    t, 
    const unsigned int                  realVertexSizeInBytes);

/*!
    \brief interpolate attributes bewteen three vertex
    \param pv1~3 triangle vertex
    \param result the interpolation result
    \param alpha coefficient of pv1, if alpha == 1 then the result is aligned with pv1
    \param beta  coefficient of pv2, if beta  == 1 then the result is aligned with pv2
    \param gamma coefficient of pv3, if gamma == 1 then the result is aligned with pv3
    \param realVertexSizeInBytes the real size of the vertex, all the vertex attributes are the float type.
*/
void Interpolate3(
    const ScreenSpaceVertexTemplate*    pv1,
    const ScreenSpaceVertexTemplate*    pv2,
    const ScreenSpaceVertexTemplate*    pv3,
    ScreenSpaceVertexTemplate*          pResult,
    const Types::F32                    alpha,
    const Types::F32                    beta,
    const Types::F32                    gamma,
    const unsigned int                  realVertexSizeInBytes);

} // namespace CommonClass