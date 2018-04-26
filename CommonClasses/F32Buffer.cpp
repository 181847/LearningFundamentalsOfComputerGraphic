#include "F32Buffer.h"
#include <assert.h>


namespace CommonClass
{

    
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
