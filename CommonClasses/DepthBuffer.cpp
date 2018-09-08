#include "DepthBuffer.h"
#include "assert.h"

namespace CommonClass
{

DepthBuffer::DepthBuffer(const Types::U32 width, const Types::U32 height)
    :m_width(width), m_height(height)
{
    assert(m_width * m_height != 0);
    m_pBuffer = new Types::F32[m_width * m_height];
    SetAll(0.0f);
}

DepthBuffer::DepthBuffer(DepthBuffer && moveObj)
{
    this->m_height = moveObj.m_height;
    this->m_width = moveObj.m_width;

    assert(moveObj.m_pBuffer);
    this->m_pBuffer = moveObj.m_pBuffer;
    moveObj.m_pBuffer = nullptr;
}

DepthBuffer::~DepthBuffer()
{
    if (m_pBuffer)
    {
        delete[] m_pBuffer;
        m_pBuffer = nullptr;
    }
}

const Types::F32 DepthBuffer::ValueAt(const Types::U32 x, const Types::U32 y) const
{
    assert(x < m_width && y < m_height);
    return m_pBuffer[x + m_width * y];
}

Types::F32 & DepthBuffer::Value(const Types::U32 x, const Types::U32 y)
{
    assert(x < m_width && y < m_height);
    return m_pBuffer[x + m_width * y];
}

void DepthBuffer::SetAll(const Types::F32 val)
{
    for (Types::U32 i = 0; i < m_width * m_height; ++i)
    {
        m_pBuffer[i] = val;
    }
}

Image ToImage(const DepthBuffer & buffer, Types::F32 maxValue)
{
    const Types::U32 WIDTH(buffer.GetWidth()), HEIGHT(buffer.GetHeight());
    Image retImg(WIDTH, HEIGHT);

    for (Types::U32 y = 0; y < HEIGHT; ++y)
    {
        for (Types::U32 x = 0; x < WIDTH; ++x)
        {
            Types::F32 value = buffer.ValueAt(x, y) / maxValue;
            retImg.SetPixel(x, y, RGBA(value, value, value));
        }
    }
    return retImg;
}

}// namespace CommonClass
