#pragma once
#include <functional>
#include "CommonTypes.h"
#include "Transform.h"
#include "ScreenSpaceVertexTemplate.h"
#include "ColorTemplate.h"

namespace CommonClass
{

/*!
    \brief store the differenct size of the vertex in the pipline.
*/
struct VertexLayout
{
    /*!
        \brief the vertex size put into the vertex shader, should be the times of four (sizeof(float32))
    */
    unsigned int vertexShaderInputSize;

    /*!
        \brief the vertex size put into the pixel shader, should be the times of four (sizeof(float32))
    */
    unsigned int pixelShaderInputSize;
};

/*!
    \brief enumration of primitive type,
    such as line list or triangle list
*/
enum PrimitiveType
{
    UNKNOWN = 0,
    LINE_LIST,
    TRIANGLE_LIST
};

/*!
    \brief the face to be culled.
*/
enum CullFace
{
    NONE = 0,
    COUNTER_CLOCK_WISE,
    CLOCK_WISE
};

/*!
    \brief a viewport struct to describe how 
    to project normalized device coordinate into pixel space.
    the viewport is defined by the pixel index starting from Zero.
    e.g.
        a image with 320x128,
        the max viewport should be left(0), right(319), bottom(0), top(127)
*/
struct Viewport
{
    Types::F32 
        left    = 0.0f, 
        right   = 0.0f, 
        top     = 0.0f, 
        bottom  = 0.0f;
};

/*!
    \brief PiplineStateObject is used to store necessary information
    to draw an primitive,
    such as vertex shader/pixel shader/ primitive type.
*/
class PiplineStateObject
{
public:
    /*!
        \brief the primitive type to be drawn.
    */
    PrimitiveType m_primitiveType = PrimitiveType::UNKNOWN;

    /*!
        \brief indicate which fase to be culled,
        default to be ClockWise.
    */
    CullFace m_cullFace = CLOCK_WISE;

    /*!
        \brief the vertex size of different of stage in the pipeline.
        the pipeline will using the destination vertex size to create vertices.
    */
    VertexLayout m_vertexLayout;
    
    /*!
        \brief the viewport for mapping ndc space to screen space.
    */
    Viewport m_viewport;

    /*!
        \brief the viewport transformation matrix.
    */
    Transform     m_viewportTransform;

    /*!
        \brief the pixel shader to compute the color.
        \paramType ScreenSpaceVertexTemplate 
    */
    std::function< RGBA(const ScreenSpaceVertexTemplate *) > m_pixelShader = nullptr;

    /*!
        \brief 
    */
    std::function< void(const unsigned char *, ScreenSpaceVertexTemplate*)> m_vertexShader = nullptr;

public:
    PiplineStateObject();
    ~PiplineStateObject();

    /*!
        \brief set a view port to the pipeline state.
        the pso will compute a viewport transformation and store it.
    */
    void SetViewport(Viewport viewport);
};

} // namespace CommonClass

