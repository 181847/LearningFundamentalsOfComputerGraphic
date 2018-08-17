#include "GeomentryBuilder.h"
#include <array>
#include <assert.h>

namespace CommonClass
{


void GeometryBuilder::BuildCube(const Types::F32 & oneSide, std::vector<CommonClass::vector3>* outVertices, std::vector<unsigned int>* outIndices)
{
    using namespace CommonClass;

    assert(outVertices != nullptr && outIndices != nullptr);

    const Types::F32 half = oneSide / 2;
    // using right hand space
    // build top face: +y
    

    /*
                +Y
                |\
                |
                |
                |
                |
                O----------------> +X
               /
              /
             /
           |/
          +Z

      [1]----->[2]
       |        |  \
       |  top   |   \
       |start   V    \
      [0]<-----[3]    V
       \      [5]------>[6]
        \      A         |
         \     |   bottom|
          \    |start    V
           V  [4]<------[7]
    */
    
    // set positions
    std::vector<vector3>& posArr = *outVertices;
    posArr.resize(8);
    // up
    posArr[0] = vector3(-half, +half, +half);
    posArr[1] = vector3(-half, +half, -half);
    posArr[2] = vector3(+half, +half, -half);
    posArr[3] = vector3(+half, +half, +half);
    // down
    posArr[4] = vector3(-half, -half, +half);
    posArr[5] = vector3(-half, -half, -half);
    posArr[6] = vector3(+half, -half, -half);
    posArr[7] = vector3(+half, -half, +half);

    // set index
    outIndices->clear();
    // up
    outIndices->push_back(0);
    outIndices->push_back(3);
    outIndices->push_back(2);
    outIndices->push_back(0);
    outIndices->push_back(2);
    outIndices->push_back(1);

    // left
    outIndices->push_back(0);
    outIndices->push_back(5);
    outIndices->push_back(4);
    outIndices->push_back(0);
    outIndices->push_back(1);
    outIndices->push_back(5);

    // right
    outIndices->push_back(3);
    outIndices->push_back(7);
    outIndices->push_back(6);
    outIndices->push_back(3);
    outIndices->push_back(6);
    outIndices->push_back(2);

    // front
    outIndices->push_back(0);
    outIndices->push_back(4);
    outIndices->push_back(7);
    outIndices->push_back(0);
    outIndices->push_back(7);
    outIndices->push_back(3);

    // back
    outIndices->push_back(2);
    outIndices->push_back(6);
    outIndices->push_back(5);
    outIndices->push_back(2);
    outIndices->push_back(5);
    outIndices->push_back(1);

    // bottom
    outIndices->push_back(4);
    outIndices->push_back(6);
    outIndices->push_back(7);
    outIndices->push_back(4);
    outIndices->push_back(5);
    outIndices->push_back(6);

}

MeshData GeometryBuilder::BuildCylinder(
    const Types::F32& topRadius,
    const Types::F32& bottomRadius,
    const Types::F32& height,
    const Types::U32& slice,
    const Types::U32& stack,
    const bool        needCap)
{
    using namespace Types;

    MeshData retData;
    retData.m_indices.clear();
    retData.m_vertices.clear();


    // push vetex
    const F32 deltaRadius = (topRadius - bottomRadius) / stack; // radius diff between stacks
    const F32 deltaRadio = 2.0f * Constant::PI_F / slice; // delta radio in one circle
    const F32 deltaStack = height / stack;
    const U32 numSideVertex = slice * (stack + 1);

    F32 radius = bottomRadius;
    F32 radio;
    F32 x, z;
    F32 y = 0.0f;
    for (U32 stackIndex = 0; stackIndex <= stack; ++stackIndex)
    {

        radio = 0.0f;
        for (U32 sliceIndex = 0; sliceIndex < slice; ++sliceIndex)
        {
            x = std::cosf(radio) * radius;
            z = - std::sinf(radio) * radius;

            retData.m_vertices.push_back(vector3(x, y, z));

            radio += deltaRadio;
        }

        radius += deltaRadius;
        y += deltaStack;
    }// end for stackIndex

    // push indices

    // a ----b
    // |    /|
    // |   / |
    // |  /  |
    // | /   |
    // |/    |
    // c ----d
    U32 a, b,
        c, d;


    for (U32 stackIndex = 0; stackIndex < stack; ++stackIndex)
    {
        for (U32 sliceIndex = 0; sliceIndex < slice; ++sliceIndex)
        {
            if (sliceIndex == slice - 1)
            {
                a = (stackIndex + 1) * slice + sliceIndex;      b = a - sliceIndex;
                c = stackIndex * slice + sliceIndex;            d = c - sliceIndex;
            }
            else
            {
                a = (stackIndex + 1) * slice + sliceIndex;      b = a + 1;
                c = stackIndex * slice + sliceIndex;            d = c + 1;
            }

            // counter clockwise
            retData.m_indices.push_back(a);
            retData.m_indices.push_back(c);
            retData.m_indices.push_back(b);
            // counter clockwise
            retData.m_indices.push_back(b);
            retData.m_indices.push_back(c);
            retData.m_indices.push_back(d);
        }// end for slices
    }// end for stacks

    if (!needCap)
    {
        return retData;
    }

    U32 accumulateIndex = retData.m_vertices.size();
    

    // cap vertices
    radio = 0.0f;
    for (U32 sliceIndex = 0; sliceIndex < slice; ++sliceIndex)
    {
        x = std::cosf(radio);
        z = - std::sinf(radio);

        // top
        retData.m_vertices.push_back(vector3(x * topRadius,     height, z * topRadius));
        // bottom
        retData.m_vertices.push_back(vector3(x * bottomRadius,  0.0f,   z * bottomRadius));
        
        radio += deltaRadio;
    }// end for
    const U32 topCenter = retData.m_vertices.size();
    const U32 bottomCenter = topCenter + 1;
    retData.m_vertices.push_back(vector3(0.0f, height, 0.0f));
    retData.m_vertices.push_back(vector3(0.0f, 0.0f,   0.0f));

    //   topCenter
    //     /\                         
    //    /  \                        
    //   /    \                       
    //  /      \                      
    // a ------ b
    //                             
    //                             
    //                             
    // c ------- d                   
    //  \       /                     
    //   \     /                      
    //    \   /                       
    //     \ /                        
    //    bottomCenter                        
    
    // cap indices
    for (U32 sliceIndex = 0; sliceIndex < slice; ++sliceIndex)
    {
        if (sliceIndex == slice - 1)
        {
            a = accumulateIndex + sliceIndex * 2 + 0;   b = a - sliceIndex * 2;
            c = accumulateIndex + sliceIndex * 2 + 1;   d = c - sliceIndex * 2;
        }
        else
        {
            a = accumulateIndex + sliceIndex * 2 + 0;   b = a + 2;
            c = accumulateIndex + sliceIndex * 2 + 1;   d = c + 2;
        }

        retData.m_indices.push_back(topCenter);
        retData.m_indices.push_back(a);
        retData.m_indices.push_back(b);
        retData.m_indices.push_back(bottomCenter);
        retData.m_indices.push_back(d);
        retData.m_indices.push_back(c);
    }

    return retData;
}

CommonClass::MeshData GeometryBuilder::BuildSphere(const Types::F32& radius, const Types::F32& slice, const Types::F32& stack)
{
    using namespace Types;

    MeshData retData;
    retData.m_indices.clear();
    retData.m_vertices.clear();

    const F32 DELTA_SLICE_RADIO = Constant::PI_F * 2.0f / slice;
    const F32 DELTA_STACK_RADIO = Constant::PI_F / stack;
    const F32 usingRightHand = -1.0f;

    F32 x, y, z;

    // A  Y
    // |
    // |
    // |             
    // |             
    // |------------------> X
    // |\             
    // | \            
    // |--\  verticalRadio         
    // |   \          
    // |    \             
    // |     \            
    // |      \           
    // |                 
    // |   
    F32 verticalRadio = DELTA_STACK_RADIO;
    F32 horizontalRadio = 0.0f;
    F32 horizontalRadius = 0.0f;
    
    for (U32 stackIndex = 0; stackIndex < stack - 1; ++stackIndex) // loop (stack - 1)
    {
        y = -std::cosf(verticalRadio) * radius; // height

        horizontalRadio = 0.0f;
        horizontalRadius = std::sinf(verticalRadio) * radius; // stack radius
        for (U32 sliceIndex = 0; sliceIndex < slice; ++sliceIndex)
        {
            x = std::cosf(horizontalRadio) * horizontalRadius;
            z = - std::sinf(horizontalRadio) * horizontalRadius;

            retData.m_vertices.push_back(vector3(x, y, z));

            horizontalRadio += DELTA_SLICE_RADIO;
        }

        verticalRadio += DELTA_STACK_RADIO;
    }// end for stackIndex

    // extra two vertex
    retData.m_vertices.push_back(vector3(0.0f, -radius, 0.0f)); // push bottom vertex
    retData.m_vertices.push_back(vector3(0.0f, +radius, 0.0f)); // push top vertex

     // push indices
     // a ----b
     // |    /|
     // |   / |
     // |  /  |
     // | /   |
     // |/    |
     // c ----d
    U32 a, b,
        c, d;

    // middle faces
    for (U32 stackIndex = 0; stackIndex < stack - 2; ++stackIndex)
    {
        for (U32 sliceIndex = 0; sliceIndex < slice; ++sliceIndex)
        {
            if (sliceIndex == slice - 1)
            {
                a = (stackIndex + 1) * slice + sliceIndex;      b = a - sliceIndex;
                c = stackIndex * slice + sliceIndex;            d = c - sliceIndex;
            }
            else
            {
                a = (stackIndex + 1) * slice + sliceIndex;      b = a + 1;
                c = stackIndex * slice + sliceIndex;            d = c + 1;
            }

            // counter clockwise
            retData.m_indices.push_back(a);
            retData.m_indices.push_back(c);
            retData.m_indices.push_back(b);
            // counter clockwise
            retData.m_indices.push_back(b);
            retData.m_indices.push_back(c);
            retData.m_indices.push_back(d);
        }// end for slices
    }// end for stacks

    // cap bottom and top
    U32 accumulateIndex = retData.m_vertices.size() - 2; // substract top and bottom vertex
    const U32 bottomCenter = accumulateIndex;
    const U32 topCenter    = accumulateIndex + 1;

    //   topCenter
    //     /\                         
    //    /  \                        
    //   /    \                       
    //  /      \                      
    // a ------ b
    //                             
    //                             
    //                             
    // c ------- d                   
    //  \       /                     
    //   \     /                      
    //    \   /                       
    //     \ /                        
    //    bottomCenter                        

    // cap indices
    for (U32 sliceIndex = 0; sliceIndex < slice; ++sliceIndex)
    {
        if (sliceIndex == slice - 1)
        {
            a = accumulateIndex - slice + sliceIndex;   b = a - slice;
            c = sliceIndex;                             d = 0;
        }
        else
        {
            a = accumulateIndex - slice + sliceIndex;   b = a + 1;
            c = sliceIndex;                             d = c + 1;
        }

        retData.m_indices.push_back(topCenter);
        retData.m_indices.push_back(a);
        retData.m_indices.push_back(b);
        retData.m_indices.push_back(bottomCenter);
        retData.m_indices.push_back(d);
        retData.m_indices.push_back(c);
    }

    return retData;
}

}// namespace CommonClass
