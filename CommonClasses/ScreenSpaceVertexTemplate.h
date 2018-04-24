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
// ensurance
static_assert(sizeof(ScreenSpaceVertexTemplate) == (4 + 1) * sizeof(Types::F32), "size of template Vertex is wrong.");


/*!
    \brief interpolate attributes bewteen two vertex
    \param pv1 the first vertex
    \param pv2 the second vertex
    \param result the interpolation result
    \param u the interpolation coefficience (should be [0, 1])
    \param realVertexSizeInBytes the real size of the vertex, all the vertex attributes are the float type.
*/
void Interpolate2(
    const ScreenSpaceVertexTemplate* pv1, 
    const ScreenSpaceVertexTemplate* pv2, 
    ScreenSpaceVertexTemplate* pResult, 
    const Types::F32 u, 
    const unsigned int realVertexSizeInBytes);

} // namespace CommonClass