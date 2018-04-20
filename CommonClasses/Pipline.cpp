#include "Pipline.h"

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

    unsigned char * pSrcFloat = vertices->GetBuffer();
    unsigned char * pDestFloat = viewportTransData->GetBuffer();

    for (unsigned int i = 0; i < numVertices; ++i)
    {
        ScreenSpaceVertexTemplate* pSrcVertex = reinterpret_cast<ScreenSpaceVertexTemplate * >(pSrcFloat);

        ScreenSpaceVertexTemplate* pDestVertex = reinterpret_cast<ScreenSpaceVertexTemplate * >(pDestFloat);

        pDestVertex->m_posH = viewportTransformMat * pSrcVertex->m_posH;

        // move to next data.
        pSrcFloat += vsInputStride;
        pDestFloat += psInputStride;
    }

    DrawLineList(indices, std::move(viewportTransData));
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

    for (auto x = x0; x <= x1; ++x)
    {
        if (steep)
        {
            m_backBuffer->SetPixel(y, x, RGB::BLACK);
        }
        else
        {
            m_backBuffer->SetPixel(x, y, RGB::BLACK);
        }

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

