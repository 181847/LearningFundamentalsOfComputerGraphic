#include "Pipline.h"
#include <array>
#include "DebugHelpers.h"

namespace CommonClass
{

Pipline::Pipline()
{
}

Pipline::~Pipline()
{
}

void Pipline::SetBackBuffer(std::unique_ptr<RasterizeImage> backBuffer)
{
    m_backBuffer = std::move(backBuffer);
}

void Pipline::SetPSO(std::unique_ptr<PiplineStateObject> pso)
{
    m_pso = std::move(pso);
}

void Pipline::DrawInstance(const std::vector<unsigned int>& indices, const F32Buffer * vertices)
{
    if (nullptr == m_pso.get())
    {
        throw std::exception("lack of pipline state object");
    }

    if (m_pso->m_primitiveType != PrimitiveType::LINE_LIST)
    {
        throw std::exception("unsupported primitive type");
    }

    if (m_pso->m_pixelShader == nullptr)
    {
        throw std::exception("pipline state object lack of pixel shader.");
    }

    const unsigned int vsInputStride = m_pso->m_vertexLayout.vertexShaderInputSize;
    const unsigned int psInputStride = m_pso->m_vertexLayout.pixelShaderInputSize;

    std::vector<unsigned int> clippedIndices;   // the index data that has been clipped.
    std::unique_ptr<F32Buffer> clippedLineData; // the vertex data that has been clipped.
    // clip all the line
    ClipLineList(indices, vertices, psInputStride, &clippedIndices, &clippedLineData);

    const unsigned int numIndices = clippedIndices.size();
    const unsigned int numBytes = clippedLineData->GetSizeOfByte();

    // now the pipline is not complete, so for the simplification, we assume all the vertex is in the same size.
    assert(vsInputStride == psInputStride);
    assert(numBytes % vsInputStride == 0 && "vertics data error, cannot ensure each vertex data is complete.");

    // compute the number of vertex.
    const unsigned int numVertices = numBytes / psInputStride;
    // create shader input buffer.
    auto viewportTransData = std::make_unique<F32Buffer>(numVertices * psInputStride);

    unsigned char * pSrcFloat = clippedLineData->GetBuffer();
    unsigned char * pDestFloat = viewportTransData->GetBuffer();

    Transform& viewportTransformMat = m_pso->m_viewportTransform;
    for (unsigned int i = 0; i < numVertices; ++i)
    {
        ScreenSpaceVertexTemplate* pSrcVertex = reinterpret_cast<ScreenSpaceVertexTemplate * >(pSrcFloat);

        ScreenSpaceVertexTemplate* pDestVertex = reinterpret_cast<ScreenSpaceVertexTemplate * >(pDestFloat);

        pDestVertex->m_posH = viewportTransformMat * pSrcVertex->m_posH;

        // move to next data.
        pSrcFloat += vsInputStride;
        pDestFloat += psInputStride;
    }

    DrawLineList(clippedIndices, std::move(viewportTransData));
}

void Pipline::DrawLineList(const std::vector<unsigned int>& indices, const std::unique_ptr<F32Buffer> lineEndPointList)
{
    const unsigned int numIndices = indices.size();
    if (numIndices % 2 != 0)
    {
        throw std::exception("indices of line list is not even");
    }

    const unsigned int vertexStride = m_pso->m_vertexLayout.pixelShaderInputSize;
    const unsigned int numVertices = lineEndPointList->GetSizeOfByte() / vertexStride;
    unsigned char * pDataStart = lineEndPointList->GetBuffer();

    // for each two points, draw a segment
    for (unsigned int i = 0; i < numIndices - 1; i += 2)
    {
        const ScreenSpaceVertexTemplate* pv1 = reinterpret_cast<const ScreenSpaceVertexTemplate *>(pDataStart + indices[i    ] * vertexStride);
        const ScreenSpaceVertexTemplate* pv2 = reinterpret_cast<const ScreenSpaceVertexTemplate *>(pDataStart + indices[i + 1] * vertexStride);

       DrawBresenhamLine(pv1, pv2);
    }
}

void Pipline::DrawBresenhamLine(const ScreenSpaceVertexTemplate* pv1, const ScreenSpaceVertexTemplate* pv2)
{
    Types::I32 x0, y0, x1, y1;
    x0 = static_cast<Types::I32>(pv1->m_posH.m_x);
    y0 = static_cast<Types::I32>(pv1->m_posH.m_y);
    x1 = static_cast<Types::I32>(pv2->m_posH.m_x);
    y1 = static_cast<Types::I32>(pv2->m_posH.m_y);

    bool steep = std::abs(y1 - y0) > abs(x1 - x0);
    if (steep)
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    Types::I32 dx = x1 - x0;
    Types::I32 twoDy = std::abs(2 * (y1 - y0));
    Types::I32 yi = y1 > y0 ? 1 : -1;
    Types::I32 twoDx = 2 * dx;
    Types::I32 y = y0;
    Types::I32 error = twoDy - dx;
    Types::I32 twoDyMinusTwoDx = error - dx;

    // get pixel shader
    auto & pixelShader = m_pso->m_pixelShader;

    // create interpolated vertex buffer
    const unsigned int PSVInputeSize = m_pso->m_vertexLayout.pixelShaderInputSize;
    F32Buffer pixelShaderInputBuffer(PSVInputeSize);
    // reinterpret it as ScreenSpaceVertexTemplate.
    ScreenSpaceVertexTemplate* pPSVInput = reinterpret_cast<ScreenSpaceVertexTemplate *>(pixelShaderInputBuffer.GetBuffer());

    // prepare interpolation coefficience.
    Types::F32 u = 1.0f, du = 1.0f / dx;    // "u" is used to interpolate between (x0, y0) and (x1, y1);
    for (auto x = x0; x <= x1; ++x)
    {
        Interpolate2(pv1, pv2, pPSVInput, u, PSVInputeSize);
        if (steep)
        {
            //m_backBuffer->SetPixel(y, x, RGB::BLACK);
            m_backBuffer->SetPixel(y, x, pixelShader(pPSVInput));
        }
        else
        {
            //m_backBuffer->SetPixel(x, y, RGB::BLACK);
            m_backBuffer->SetPixel(x, y, pixelShader(pPSVInput));
        }

        // update interpolation coefficience.
        u -= du;

        if (error > 0)
        {
            y += yi;
            error += twoDyMinusTwoDx;
        }
        else
        {
            error += twoDy;
        }
    }
}

bool Pipline::ClipLineInHomogenousClipSpace(
    const ScreenSpaceVertexTemplate* pv1,
    const ScreenSpaceVertexTemplate* pv2,
    ScreenSpaceVertexTemplate* pOutV1,
    ScreenSpaceVertexTemplate* pOutV2, 
    const unsigned int realVertexSize)
{
    assert(pv1 != nullptr && pv2 != nullptr && pOutV1 != nullptr && pOutV2 != nullptr && "null pointer error");
    assert(pv1 != pOutV1 && pv2 != pOutV2 && pv1 != pOutV2 && pv2 != pOutV1 && "pointer address conflict error, the data will be wrong");

    std::array<Types::F32, 6> p;
    std::array<Types::F32, 6> q;
    Types::F32 t0 = 0.0f;       // start point 
    Types::F32 t1 = 1.0f;       // end point
    Types::F32 tempT = 0.0f;    // temp interpolate coefficience

    const Types::F32 deltaX = pv2->m_posH.m_x - pv1->m_posH.m_x;
    const Types::F32 deltaY = pv2->m_posH.m_y - pv1->m_posH.m_y;
    const Types::F32 deltaZ = pv2->m_posH.m_z - pv1->m_posH.m_z;
    const Types::F32 deltaW = pv2->m_posH.m_w - pv1->m_posH.m_w;

    p[0] = -(deltaX + deltaW);
    p[1] = deltaX - deltaW;
    p[2] = -(deltaY + deltaW);
    p[3] = deltaY - deltaW;
    p[4] = -(deltaZ + deltaW);
    p[5] = deltaZ - deltaW;

    q[0] = pv1->m_posH.m_x + pv1->m_posH.m_w;
    q[1] = pv1->m_posH.m_w - pv1->m_posH.m_x;
    q[2] = pv1->m_posH.m_y + pv1->m_posH.m_w;
    q[3] = pv1->m_posH.m_w - pv1->m_posH.m_y;
    q[4] = pv1->m_posH.m_z + pv1->m_posH.m_w;
    q[5] = pv1->m_posH.m_w - pv1->m_posH.m_z;

    for (unsigned int i = 0; i < 6; ++i)
    {
        if (std::abs(p[i] - 0.0f) < Types::Constant::EPSILON_F32)
        {
            if (q[i] < 0.0f)
            {
                return false;
            }
        }
        else if (p[i] > 0.0f)
        {
            tempT = q[i] / p[i];
            if (tempT < t0)
            {
                // new end point is after start point.
                // early reject
                return false;
            }
            else if (tempT < t1)
            {
                // update t1, new end point
                t1 = tempT;
            }
        }
        else // p[i] < 0.0f
        {
            tempT = q[i] / p[i];
            if (tempT > t1)
            {
                // new start point is beyond end point.
                // early reject
                return false;
            }
            else if (tempT > t0)
            {
                // update t1, new start point
                t0 = tempT;
            }
        }
    }
    
    if (t0 == 0.0f)
    {
        memcpy(pOutV1, pv1, realVertexSize);
    }
    else
    {
        // warning, pOutV1 = (1 - t0) * pv1 + t0 * pv2
        // this means:
        // if t0 == 0, then pOutV1 = pv1
        // if t0 == 1, then pOutV1 = pv2
        // but this is opposited to interpolation between two value,
        //      where if u == 1, then pOutV1 = pv1
        //            if u == 0, then pOutV1 = pv2
        // so here we flip the vertex order, to get correct interpolation result.
        Interpolate2(pv2, pv1, pOutV1, t0, realVertexSize);
    }
    
    if (t1 == 1.0f)
    {
        memcpy(pOutV2, pv2, realVertexSize);
    }
    else
    {
        // warning, pOutV1 = (1 - t0) * pv1 + t0 * pv2
        // this means:
        // if t0 == 0, then pOutV1 = pv1
        // if t0 == 1, then pOutV1 = pv2
        // but this is opposited to interpolation between two value,
        //      where if u == 1, then pOutV1 = pv1
        //            if u == 0, then pOutV1 = pv2
        // so here we flip the vertex order, to get correct interpolation result.
        Interpolate2(pv2, pv1, pOutV2, t1, realVertexSize);
    }

    BREAK_POINT_IF(
        pOutV1->m_posH.m_x > 1.0f || pOutV1->m_posH.m_y > 1.0f || pOutV1->m_posH.m_z > 1.0f
        || pOutV2->m_posH.m_x > 1.0f || pOutV2->m_posH.m_y > 1.0f || pOutV2->m_posH.m_z > 1.0f);

    return true;
}

void Pipline::ClipLineList(
    const std::vector<unsigned int>&    indices, 
    const F32Buffer *                   vertices,
    const unsigned int                  realVertexSize, 
    std::vector<unsigned int> *         pClippedIndices, 
    std::unique_ptr<F32Buffer> *        pClippedVertices)
{
    // For each line segment, the clip test will be executed, and return a new line segment,
    // now we don't have optimization on reusing vertex by same index.
    
    assert(realVertexSize % 4 == 0                                      && "vertex size is not the times of four");
    assert(pClippedIndices != nullptr && pClippedVertices != nullptr    && "argument nullptr error");
    assert(indices.size() % 2 == 0                                      && "line indices is not pairs");

    pClippedIndices->clear();   // empty the output indice buffer.
    unsigned int numLineSegment = indices.size() / 2; // number of all line segments

    // create a buffer stream which can hold every individual vertex for each line segment.
    // each end point will have its own vertex.
    auto clippedStream = std::make_unique<F32Buffer>(realVertexSize * 2 * numLineSegment);

    const unsigned int twoRealVertexSizeBytes = 2 * realVertexSize;

    unsigned char * pSrcVerticesAddr    = vertices->GetBuffer();                    // source vertex start address

    unsigned char * pStartSrcVertex     = nullptr;                              // src start vertex location
    unsigned char * pEndSrcVertex       = nullptr;                              // src end vertex location 
    unsigned char * pStartClippedVertex = clippedStream->GetBuffer();           // output clipped start vertex
    unsigned char * pEndClippedVertex   = pStartClippedVertex + realVertexSize; // output clipped end vertex

    unsigned int    startClippedVertexIndex = 0;
    unsigned int    endClippedVertexIndex = 1;
    unsigned int    numClippedVertex = 0;   // how many vertex is added to clippedStream

    bool canDrawThisSegment = false;    // for each clipping test, is this line can be draw,(or the line is rejected).

    for (unsigned int i = 0; i < numLineSegment; ++i)
    {
        // find source vertices of line segment
        pStartSrcVertex = GetVertexPtrAt(pSrcVerticesAddr, indices[ i * 2 ],     realVertexSize);
        pEndSrcVertex   = GetVertexPtrAt(pSrcVerticesAddr, indices[ i * 2 + 1 ], realVertexSize);

        // clipping test
        canDrawThisSegment = ClipLineInHomogenousClipSpace(
            reinterpret_cast<ScreenSpaceVertexTemplate *>(pStartSrcVertex), 
            reinterpret_cast<ScreenSpaceVertexTemplate *>(pEndSrcVertex), 
            reinterpret_cast<ScreenSpaceVertexTemplate *>(pStartClippedVertex), 
            reinterpret_cast<ScreenSpaceVertexTemplate *>(pEndClippedVertex), 
            realVertexSize);

        if (canDrawThisSegment)
        {
            // output vertex address move on
            pStartClippedVertex += twoRealVertexSizeBytes;
            pEndClippedVertex   += twoRealVertexSizeBytes;

            // push vertex index
            pClippedIndices->push_back(startClippedVertexIndex);
            pClippedIndices->push_back(endClippedVertexIndex);

            // increase vertexIndex to next
            startClippedVertexIndex += 2;
            endClippedVertexIndex   += 2;

            numClippedVertex += 2;
        } // end if canDrawThisSegment
    }// end for line segments

    // after clipping, is the clipped vertices cost same space as we allocated?
    const unsigned int realClippedStreamSize = numClippedVertex * realVertexSize;
    if (realClippedStreamSize < clippedStream->GetSizeOfByte())
    {
        // if the vertices of visible line segment use less space than clippedStream stream
        // creat another buffer stream which will only contain the useable data.
        auto shrinkClippedStream = std::make_unique<F32Buffer>(realClippedStreamSize);
        memcpy(shrinkClippedStream->GetBuffer(), clippedStream->GetBuffer(), shrinkClippedStream->GetSizeOfByte());
        
        // return vertex data
        *pClippedVertices = std::move(shrinkClippedStream);
    }
    else if (realClippedStreamSize == clippedStream->GetSizeOfByte())
    {
        *pClippedVertices = std::move(clippedStream);
    }
    else
    {
        assert(false && "Warning! After clipping, we get more vertices than expected, there must be some error.");
    }
}

F32Buffer::F32Buffer(unsigned int sizeInByte)
    :m_sizeInByte(sizeInByte)
{
    assert(sizeInByte % 4 == 0 && "size of float is wrong, sizeInByte should be times of four");
    m_pBuffer = new Types::F32[m_sizeInByte / 4];
}

F32Buffer::F32Buffer(F32Buffer && moveOtherBuffer)
{
    m_pBuffer = moveOtherBuffer.m_pBuffer;
    m_sizeInByte = moveOtherBuffer.m_sizeInByte;
    moveOtherBuffer.m_pBuffer = nullptr;
    moveOtherBuffer.m_sizeInByte = 0;
}

F32Buffer::~F32Buffer()
{
    if (m_pBuffer != nullptr)
    {
        delete[] m_pBuffer;
    }
}

unsigned char * F32Buffer::GetBuffer() const
{
    return reinterpret_cast<unsigned char*>(m_pBuffer);
}

unsigned int F32Buffer::GetNumFloats() const
{
    return m_sizeInByte / 4;
}

unsigned int F32Buffer::GetSizeOfByte() const
{
    return m_sizeInByte;
}



} // namespace CommonClass

