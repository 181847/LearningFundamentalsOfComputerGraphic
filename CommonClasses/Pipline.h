#pragma once
#include <memory>

#include "RasterizeImage.h"
#include "PiplineStateObject.h"


namespace CommonClass
{

    
/*!
    \brief the F32Buffer is used to create buffer of floats.
*/
struct F32Buffer
{
private:
    Types::F32 * m_pBuffer = nullptr;
    unsigned int m_sizeInByte = 0;

public:
    /*!
        \brief construct the float buffer.
        \param sizeInByte bytes size of the buffer
        this buffer is consist of floats, but here we specific the size In BYTE,
        so sizeInByte must be the times of 4 (sizeof(float32))
    */
    F32Buffer(unsigned int sizeInByte);
    F32Buffer(F32Buffer && moveOtherBuffer);
    F32Buffer(const F32Buffer&) = delete;
    F32Buffer& operator = (const F32Buffer&) = delete;
    ~F32Buffer();

    /*!
        \brief get buffer pointer.
    */
    unsigned char * GetBuffer() const;

    /*!
        \brief return the size of the buffer in bytes.
    */
    unsigned int GetNumFloats() const;

    /*!
        \brief return the size of the buffer in Bytes.
    */
    unsigned int GetSizeOfByte() const;
};

struct ScreenSpaceVertexTemplate
{
public:
    CommonClass::hvector m_posH;
    Types::F32 m_restDates[1];
};
// ensurance
static_assert(sizeof(ScreenSpaceVertexTemplate) == (4 + 1) * sizeof(Types::F32), "size of template Vertex is wrong.");

    
/*!
    \brief abstraction of graphic pipline
*/
class Pipline
{
#ifdef _DEBUG
public:
#else
protected:
#endif
    /*!
        \brief store the pixels.
    */
    std::unique_ptr<RasterizeImage> m_backBuffer;
    
protected:
    /*!
        \brief a state object contain necessary info about how to draw primitives.
    */
    std::shared_ptr<PiplineStateObject> m_pso;

public:
    Pipline();
    ~Pipline();

    /*!
        \brief bind a back buffer to the pipline to store the rasterized image.
    */
    void SetBackBuffer(std::unique_ptr<RasterizeImage> backBuffer);

    /*!
        \brief set a pipline state object.
    */
    void SetPSO(std::unique_ptr<PiplineStateObject> pso);

    /*!
        \brief draw all the vertices with next data.
        \param indices the indices of all the vertices
        \param vertices the vertex data to be drawn, the data can be line list or triangle list.
    */
    void DrawInstance(const std::vector<unsigned int>& indices, const F32Buffer* vertices);

    /*!
        \brief draw line lists with out shader processing.
        \param indices indices of the line segments, length of it should be even
        \param vertices endpoints of line segments. they should have been transfered to screen space.
    */
    void DrawLineList(const std::vector<unsigned int>& indices, const std::unique_ptr<F32Buffer> lineEndPointList);
};

inline void Pipline::DrawInstance(const std::vector<unsigned int>& indices, const F32Buffer* vertices)
{
    if (nullptr == m_pso.get())
    {
        throw std::exception("lack of pipline state object");
    }

    if (m_pso->m_primitiveType != PrimitiveType::LINE_LIST)
    {
        throw std::exception("unsupported primitive type");
    }

    Transform& viewportTransformMat = m_pso->m_viewportTransform;

    const unsigned int numBytes = vertices->GetSizeOfByte();
    const unsigned int vsInputStride = m_pso->m_vertexLayout.vertexShaderInputSize;
    const unsigned int psInputStride = m_pso->m_vertexLayout.pixelShaderInputSize;

    // now the pipline is not complete, so for the simplification, we assume all the vertex is in the same size.
    assert(vsInputStride == psInputStride);
    assert(numBytes % vsInputStride == 0 && "vertics data error, cannot ensure each vertex data is complete.");

    // compute the number of vertex.
    const unsigned int numVertices = numBytes / vsInputStride;
    // create shader input buffer.
    auto viewportTransData = std::make_unique<F32Buffer>(numVertices * psInputStride);

    unsigned char * pSrcFloat  = vertices->GetBuffer();
    unsigned char * pDestFloat = viewportTransData->GetBuffer();

    for (unsigned int i = 0; i < numVertices; ++i)
    {
        ScreenSpaceVertexTemplate* pSrcVertex = reinterpret_cast<ScreenSpaceVertexTemplate * >(pSrcFloat);

        ScreenSpaceVertexTemplate* pDestVertex = reinterpret_cast<ScreenSpaceVertexTemplate * >(pDestFloat);

        pDestVertex->m_posH = viewportTransformMat * pSrcVertex->m_posH;
        
        // move to next data.
        pSrcFloat  += vsInputStride;
        pDestFloat += psInputStride;
    }

    DrawLineList(indices, std::move(viewportTransData));
}

} // namespace CommonClass
