#include "HPlaneEquation.h"
#include <assert.h>

namespace CommonClass
{

TrianglePair::TrianglePair(const Count count, const size_t singleVertexSizeInByte)
    :m_count(count), m_vertexSizeInByte(singleVertexSizeInByte)
{
    switch (m_count)
    {
    case ZERO:
        // do nothing
        break;

    case ONE:
        m_vertices = std::make_unique<F32Buffer>(3 * m_vertexSizeInByte);
        break;

    case TWO:
        m_vertices = std::make_unique<F32Buffer>(4 * m_vertexSizeInByte);
        
    default:
        break;
    }
}

TrianglePair::TrianglePair(TrianglePair && other)
    :m_count(other.m_count), 
    m_vertexSizeInByte(other.m_vertexSizeInByte),
    m_vertices(std::move(other.m_vertices))
{
    // empty
}

TrianglePair& TrianglePair::operator=(TrianglePair && other)
{
    m_count             = other.m_count;
    m_vertexSizeInByte  = other.m_vertexSizeInByte;
    m_indices           = std::move(other.m_indices);
    m_vertices          = std::move(other.m_vertices);
    return *this;
}

unsigned char * TrianglePair::GetVertexPointer(const unsigned short index)
{
    // 1. m_count MUST NOT be zero, or no vertex exist
    // 2. if m_count = ONE(1), index <= 2
    // 3. if m_count = TWO(2), index <= 3
    assert(m_count > 0 && index < (2 + m_count));
    return GetVertexPtrAt(m_vertices->GetBuffer(), index, m_vertexSizeInByte);
}

TrianglePair HPlaneEquation::CutTriangle(
    const ScreenSpaceVertexTemplate * pv1, 
    const ScreenSpaceVertexTemplate * pv2, 
    const ScreenSpaceVertexTemplate * pv3, 
    const unsigned int realVertexSizeBytes)
{
    std::array<const ScreenSpaceVertexTemplate *, 3> pVertices={ pv1, pv2, pv3 };

    std::array<bool, 3> bInsides = { false, false, false };

    // record inside state and count
    unsigned short insideCount = 0;
    for (unsigned int i = 0; i < pVertices.size(); ++i)
    {
        if (
            bInsides[i] = ( eval(pVertices[i]->m_posH) > 0.0f )
           )
        {
            ++insideCount;
        }
    }// end for pVertices


    switch (insideCount)
    {
    case 0:
        // triangle reject, return nothing.
        return TrianglePair(TrianglePair::ZERO, 0);
        break;// break case 0

    case 1:
        {// case 1

            // only one vertex inside the plane, so we just need to store only one triangle (three vertex)
            TrianglePair tp(TrianglePair::ONE, realVertexSizeBytes);

            // find the index of the vertex which is inside the plane.
            int insideIndex = 0;
            for (; insideIndex < 3; ++insideIndex)
            {
                if (bInsides[insideIndex]) 
                    break;
            }// end for insideIndex

            // find the index of outside vertices
            std::array<int, 2> outsideIndices = {
                (insideIndex + 1) % 3,
                (insideIndex + 2) % 3
            };

            const int indexStoreFirstV = 0;
            // first Vertex
            memcpy(tp.GetVertexPointer(indexStoreFirstV), pVertices[insideIndex], realVertexSizeBytes);

            // rest two Vertex
            Types::F32 interpolateCoefficient;
            for (unsigned int i = 0; i < outsideIndices.size(); ++i)
            {
                interpolateCoefficient = cutCoefficient(pVertices[insideIndex]->m_posH, pVertices[outsideIndices[i]]->m_posH);
                Interpolate2(
                    pVertices[insideIndex], pVertices[outsideIndices[i]],
                    reinterpret_cast<ScreenSpaceVertexTemplate*>(tp.GetVertexPointer(indexStoreFirstV + i)), // 
                    interpolateCoefficient,
                    realVertexSizeBytes);
            }
            return tp; // function will RETURN here
        }// end case 1
        break; // break case 1

    case 2:
        {// case 2

            // two vertices are inside the plane, split the triangle into two triangles.
            TrianglePair tp(TrianglePair::TWO, realVertexSizeBytes);

            int outsideIndex;
            // Trick notes:
            //   we want to find insideIndex1 and insideIndex2, and the second insideIndex is on the right side of the former.
            //   for example(maybe):
            //      | insideIndex1 | insideIndex2 |
            //      |            0 |            1 |
            //      |            1 |            2 |
            //      |            2 |            0 |
            //  in order to persist the order of two indices,
            //  my solution is rather than searching for first inside1 (which may break the order),
            //  but finding the first outside, then the inside is on the right hand of outside
            for (outsideIndex = 0; outsideIndex < 3; ++outsideIndex)
            {
                if ( ! bInsides[outsideIndex])
                    break;
            }// end for insideIndex

            std::array<int, 2> insideIndices = {
                (outsideIndex + 1) % 3,
                (outsideIndex + 2) % 3
            };

            // copy two inside vertex
            memcpy(tp.GetVertexPointer(0), pVertices[insideIndices[0]], realVertexSizeBytes);
            memcpy(tp.GetVertexPointer(1), pVertices[insideIndices[1]], realVertexSizeBytes);

            // rest two Vertex
            Types::F32 interpolateCoefficient;
            for (unsigned int i = 0; i < insideIndices.size(); ++i)
            {
                interpolateCoefficient = cutCoefficient(pVertices[insideIndices[i]]->m_posH, pVertices[outsideIndex]->m_posH);
                Interpolate2(
                    pVertices[insideIndices[i]], pVertices[outsideIndex],
                    reinterpret_cast<ScreenSpaceVertexTemplate*>(tp.GetVertexPointer(2 + i)), // first two vertex memory has been occupied.
                    interpolateCoefficient,
                    realVertexSizeBytes);
            }
            return tp;// function will RETURN here.
        }// end case 2
        break; // break case 2
       
    case 3:
        {// case 3
            // three vertices are all inside the plane, just copy the vertex data.
            TrianglePair tp(TrianglePair::ONE, realVertexSizeBytes);

            for (int i = 0; i < 3; ++i)
            {
                memcpy(tp.GetVertexPointer(i), pVertices[i], realVertexSizeBytes);
            }
            return tp;
        }// end case 3
        break;// break case 3
    }// end switch insideCount

    assert(false && "error, should NOT reach here");
    return TrianglePair(TrianglePair::ZERO, realVertexSizeBytes);
}

}// namespace CommonClass
