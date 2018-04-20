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

        m_backBuffer->DrawBresenhamLine(
            static_cast<Types::I32>(pv1->m_posH.m_x),
            static_cast<Types::I32>(pv1->m_posH.m_y),
            static_cast<Types::I32>(pv2->m_posH.m_x),
            static_cast<Types::I32>(pv2->m_posH.m_y));
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

