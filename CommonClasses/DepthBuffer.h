#pragma once
#include "CommonTypes.h"
#include "Image.h"

#include <functional>

namespace CommonClass
{

/*!
    \brief DepthBuffer is used to store float point type of value which represent the depth value for rendering.
*/
class DepthBuffer
{
protected:

    /*!
        \brief hold the values, the layout should be start from the left to right first, and the bottom to top
        >>>>>>>>>>>>>>>><end>
        ...(up)
        >>>>>>>>>>>>>>>>><up>
        <start>--->>>>>>><up>
        ...
    */
    Types::F32 * m_pBuffer;

    /*!
        \brief buffer size to match the image.
    */
    Types::U32 m_width, m_height;

public:
    DepthBuffer(const Types::U32 width, const Types::U32 height);
    DepthBuffer(const DepthBuffer &) = delete;
    DepthBuffer(DepthBuffer &&);
    DepthBuffer & operator =(const DepthBuffer&) = delete;
    ~DepthBuffer();

    /*!
        \brief get value copy
        \param x start from zero to WIDTH - 1, and locate from left to right
        \param y start from zero to HEIGHT - 1, locate from bottom to top
        for example (0, 0) reference to the bottom left corner,
            (WIDTH - 1, HEIGHT - 1) reference to the top right corner.
    */
    const Types::F32 ValueAt(const Types::U32 x, const Types::U32 y) const;
    
    /*!
        \brief get value and ready to change it.
        \param x start from zero to WIDTH - 1, and locate from left to right
        \param y start from zero to HEIGHT - 1, locate from bottom to top
        for example (0, 0) reference to the bottom left corner,
            (WIDTH - 1, HEIGHT - 1) reference to the top right corner.
    */
    Types::F32 & Value(const Types::U32 x, const Types::U32 y);

    /*!
        \brief set all float to be the same value.
    */
    void SetAll(const Types::F32 val);

    Types::U32 GetWidth() const { return m_width; }

    Types::U32 GetHeight() const { return m_height; }
};

Image ToImage(const DepthBuffer& buffer, Types::F32 maxValue);

}// namespace CommonClass