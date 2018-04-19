#pragma once
#include "CommonTypes.h"
#include "Transform.h"

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
    \brief a viewport struct to describe how 
    to project normalized device coordinate into pixel space.
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
        \brief the vertex size of different of stage in the pipline.
        the pipline will using the destination vertex size to create vertices.
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

public:
    PiplineStateObject();
    ~PiplineStateObject();

    /*!
        \brief set a view port to the pipline state.
        the pso will compute a viewport transformation and store it.
    */
    void SetViewport(Viewport viewport);
};

} // namespace CommonClass

