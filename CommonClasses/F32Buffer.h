#pragma once
#include "CommonTypes.h"

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

} // namespace CommonClass
