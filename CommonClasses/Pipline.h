#pragma once
#include <memory>

#include "RasterizeImage.h"
#include "PiplineStateObject.h"
#include "ScreenSpaceVertexTemplate.h"

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

/*!
    \brief from a start address, get a vertex of specific index, and interpret it to the desired VERTEX_TYPE pointer.
*/
template<typename VERTEX_TYPE = unsigned char>
inline VERTEX_TYPE * GetVertexPtrAt(unsigned char * pStartAddr, unsigned int vertexIndex, unsigned int vertexSizeInByte)
{
    return reinterpret_cast<VERTEX_TYPE * >(pStartAddr + vertexSizeInByte * vertexIndex);
}


    
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

#ifdef _DEBUG
public:
#else
private:
#endif
    /*!
        \brief draw bresenhamLine.
        \param (x0, y0) start point location in screen space
        \param (x1, y1) ens point location in screen space
    */
    void DrawBresenhamLine(const ScreenSpaceVertexTemplate* pv1, const ScreenSpaceVertexTemplate* pv2);

    /*!
        \brief clipping the line in homogenous clip space
        \param pv1 start point of the line
        \param pv2 end point of the line
        \param pOutV1 where to write clipped start point ( or not clipped ), address should't be the same as pv1
        \param pOutV2 where to write clipped end point ( or not clipped ), address should't be the same as pv2
        \return if the line can be drawn.(if the line is rejected, return false)
        we assum the NDC space is in [-1, +1]^3,
        yes, z is mapped to -1 (near plane), +1 (far plane).
    */
    static bool ClipLineInHomogenousClipSpace(
        const ScreenSpaceVertexTemplate* pv1,
        const ScreenSpaceVertexTemplate* pv2,
        ScreenSpaceVertexTemplate* pOutV1,
        ScreenSpaceVertexTemplate* pOutV2,
        const unsigned int realVertexSize);

    /*!
        \brief clipe line list in homogenous clip space, where the perspective divide haven't been done.
        \param indices input line list indices
        \param vertices input vertex data
        \param realVertexSize the vertes size of the input vertices in Bytes
        \param pClippedIndices the index data afther clipping
        \param pClippedVertices the vertex data afther clipping.
    */
    static void ClipLineList(
        const std::vector<unsigned int>& indices, 
        const F32Buffer* vertices,
        const unsigned int realVertexSize,
        std::vector<unsigned int> * pClippedIndices, 
        std::unique_ptr<F32Buffer> * pClippedVertices);
};

} // namespace CommonClass
