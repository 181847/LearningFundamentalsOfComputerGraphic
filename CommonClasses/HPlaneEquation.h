#pragma once
#include "ScreenSpaceVertexTemplate.h"
#include "F32Buffer.h"
#include <memory>
#include <array>

namespace CommonClass
{

/*!
    \brief store 0/1/2 triangle
    3 vertex for one triangle
    4 vertex for two triangle, who are sharing one edge and two vertex.
*/
class TrianglePair
{
public:
    /*!
        \brief count the triangle stored in the TrianglePair
    */
    enum Count
    {
        ZERO = 0,   // the pair have no triangle
        ONE,        // the pair have only one triangle
        TWO         // two triangle exist
    };

    /*!
       \brief the fixed index to access vertex data.
       ONE_TRI_X is for TrianglePair that only have one triangle.
       TWO_TRI_X_X is for TrianglePair that have two triangle, TWO_TRI_1_X is for the first triangle.
    */
    enum Index
    {
        ONE_TRI_1 = 0,
        ONE_TRI_2 = 1,
        ONE_TRI_3 = 2,
        TWO_TRI_1_1 = 0,
        TWO_TRI_1_2 = 1,
        TWO_TRI_1_3 = 2,
        TWO_TRI_2_1 = 1,
        TWO_TRI_2_2 = 3,
        TWO_TRI_2_3 = 2
    };

public:
    /*!
        \brief construct the pairs
        \param count the triangle count, may be ZERO, ONE, TWO
        \param the singleVertexSizeInByte the size of the vertex in triangle.
        if count is ZERO, won't allocate any memory.
        if the count is ONE the vertex buffer will allocate 3 * singleVertexSizeInByte.
        if count is TWO , will allocate 6 * singleVertexSizeByte.
    */
    TrianglePair(const Count count, const size_t singleVertexSizeInByte);

    /*!
        \brief move other's data.
    */
    TrianglePair(TrianglePair&& other);
    /*!
        \brief move other's data.
    */
    TrianglePair& operator = (TrianglePair&& other);

    TrianglePair(const TrianglePair&) = delete;
    TrianglePair operator = (const TrianglePair&) = delete;

    /*!
        \brief get raw vertex address, in the buffer
        \param index the index of the vertex in the buffer, start from zero.
        if the TrianglePair have ONE triangle, index can be {0, 1, 2}.
        if the TrianglePair have TWO triangle, index can be {0, 1, 2, 3}.
    */
    unsigned char * GetVertexPointer(const unsigned short index);

public:
    /*!
        \brief single vertex size.
    */
    size_t m_vertexSizeInByte;

    /*!
        \brief count how many triangle is stored.
    */
    Count m_count;

    /*!
        \brief the buffer store triangle vertex.
    */
    std::unique_ptr<F32Buffer> m_vertices;
};

class HPlaneEquation
{
public:
    TrianglePair CutTriangle(
        const ScreenSpaceVertexTemplate*    pv1,
        const ScreenSpaceVertexTemplate*    pv2,
        const ScreenSpaceVertexTemplate*    pv3,
        const unsigned int                  realVertexSizeBytes);

    /*!
        \brief evaluate the homogeneous point is whether inside the 4D homogeneous plane
        \param pointH the position in the homogeneous coordinate
        generally speaking, the positive result mean that inside the plane.
    */
    virtual Types::F32 eval(const hvector& pointH) = 0;

    /*!
        \brief calculate the interpolate coefficient
        \param point1~2 two point in homogeneous coordinate
    */
    virtual Types::F32 cutCoefficient(const hvector& point1, const hvector& point2) = 0;
};

}// namespace CommonClass
