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
       \     [5]------>[6]
        \     A         |
         \    |   bottom|
          \   |start    V
           V [4]<------[7]
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

CommonClass::MeshData GeometryBuilder::BuildCube(const Types::F32 width, const Types::F32 height, const Types::F32 depth)
{
    using namespace Types;
    MeshData retData;

    vector3 halfVec(std::abs(width), std::abs(height), std::abs(depth));
    halfVec = halfVec * 0.5f;

    vector3 normal;
    vector3 pos;

    // top
    normal = vector3::AXIS_Y;
    pos = halfVec;
    retData.m_vertices.push_back({ pos, normal, vector2(0.75f, 0.50f) });
    pos.m_z *= -1.0f;
    retData.m_vertices.push_back({ pos, normal, vector2(0.75f, 0.75f) });
    pos.m_x *= -1.0f;
    retData.m_vertices.push_back({ pos, normal, vector2(0.50f, 0.75f) });
    pos.m_z *= -1.0f;
    retData.m_vertices.push_back({ pos, normal, vector2(0.50f, 0.50f) });

    // bottom
    normal = -1.0f * vector3::AXIS_Y;
    pos = -1.0f * halfVec;
    pos.m_x *= -1.0f;
    retData.m_vertices.push_back({ pos, normal, vector2(0.75f, 0.00f) });
    pos.m_z *= -1.0f;
    retData.m_vertices.push_back({ pos, normal, vector2(0.75f, 0.25f) });
    pos.m_x *= -1.0f;
    retData.m_vertices.push_back({ pos, normal, vector2(0.50f, 0.25f) });
    pos.m_z *= -1.0f;
    retData.m_vertices.push_back({ pos, normal, vector2(0.50f, 0.00f) });

    // left
    normal = -1.0f * vector3::AXIS_X;
    pos = -1.0f * halfVec;
    pos.m_z *= -1.0f;
    retData.m_vertices.push_back({ pos, normal, vector2(0.50f, 0.25f) });
    pos.m_y *= -1.0f;
    retData.m_vertices.push_back({ pos, normal, vector2(0.50f, 0.50f) });
    pos.m_z *= -1.0f;
    retData.m_vertices.push_back({ pos, normal, vector2(0.25f, 0.50f) });
    pos.m_y *= -1.0f;
    retData.m_vertices.push_back({ pos, normal, vector2(0.25f, 0.25f) });

    // right
    normal = vector3::AXIS_X;
    pos = -1.0f * halfVec;
    pos.m_x *= -1.0f;
    retData.m_vertices.push_back({ pos, normal, vector2(1.00f, 0.25f) });
    pos.m_y *= -1.0f;
    retData.m_vertices.push_back({ pos, normal, vector2(1.00f, 0.50f) });
    pos.m_z *= -1.0f;
    retData.m_vertices.push_back({ pos, normal, vector2(0.75f, 0.50f) });
    pos.m_y *= -1.0f;
    retData.m_vertices.push_back({ pos, normal, vector2(0.75f, 0.25f) });

    // front
    normal = vector3::AXIS_Z;
    pos = halfVec;
    pos.m_y *= -1.0f;
    retData.m_vertices.push_back({ pos, normal, vector2(0.75f, 0.25f) });
    pos.m_y *= -1.0f;
    retData.m_vertices.push_back({ pos, normal, vector2(0.75f, 0.50f) });
    pos.m_x *= -1.0f;
    retData.m_vertices.push_back({ pos, normal, vector2(0.50f, 0.50f) });
    pos.m_y *= -1.0f;
    retData.m_vertices.push_back({ pos, normal, vector2(0.50f, 0.25f) });

    // back
    normal = vector3::AXIS_Z;
    pos = -1.0f * halfVec;
    retData.m_vertices.push_back({ pos, normal, vector2(0.25f, 0.25f) });
    pos.m_y *= -1.0f;
    retData.m_vertices.push_back({ pos, normal, vector2(0.25f, 0.50f) });
    pos.m_x *= -1.0f;
    retData.m_vertices.push_back({ pos, normal, vector2(0.00f, 0.50f) });
    pos.m_y *= -1.0f;
    retData.m_vertices.push_back({ pos, normal, vector2(0.00f, 0.25f) });

    for (U32 face = 0; face < 6; ++face)
    {
        U32 baseIndex = face * 4;

        // first tri in quad
        retData.m_indices.push_back(baseIndex);
        retData.m_indices.push_back(baseIndex + 1);
        retData.m_indices.push_back(baseIndex + 2);

        // second tri in quad
        retData.m_indices.push_back(baseIndex);
        retData.m_indices.push_back(baseIndex + 2);
        retData.m_indices.push_back(baseIndex + 3);
    }

    assert(retData.m_vertices.size() == 24 && retData.m_indices.size() == 36);
    return retData;
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
    
    // fix normal on sides
    const F32 fixedNormalH = height, fixedNormalB = bottomRadius - topRadius;
    const F32 recipcol_l = 1.0f / std::sqrt(fixedNormalH * fixedNormalH + fixedNormalB * fixedNormalB);
    F32 fixedNormalRadius = fixedNormalH * recipcol_l;
    F32 fixedNormalHeight = fixedNormalB * recipcol_l;
    
    F32 radius = bottomRadius;
    F32 radio;
    F32 x, z;
    F32 y = 0.0f;
    for (U32 stackIndex = 0; stackIndex <= stack; ++stackIndex)
    {

        radio = 0.0f;
        for (U32 sliceIndex = 0; sliceIndex < slice; ++sliceIndex)
        {
            x = std::cosf(radio);
            z = - std::sinf(radio);

            vector3 pos = vector3(x * radius, y, z * radius);
            vector3 normal = vector3(x * fixedNormalRadius, fixedNormalHeight, z * fixedNormalRadius);
            retData.m_vertices.push_back({ pos, normal });

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
        z = -std::sinf(radio);

        // top
        retData.m_vertices.push_back({ vector3(x * topRadius,     height, z * topRadius   ), vector3(0.0f, +1.0f, 0.0f) });
        // bottom
        retData.m_vertices.push_back({ vector3(x * bottomRadius,    0.0f, z * bottomRadius), vector3(0.0f, -1.0f, 0.0f) });
        
        radio += deltaRadio;
    }// end for

    const U32 topCenter = retData.m_vertices.size();
    const U32 bottomCenter = topCenter + 1;
    retData.m_vertices.push_back({ vector3(0.0f, height, 0.0f), vector3(0.0f, +1.0f, 0.0f) });
    retData.m_vertices.push_back({ vector3(0.0f, 0.0f,   0.0f), vector3(0.0f, -1.0f, 0.0f) });

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

CommonClass::MeshData GeometryBuilder::BuildSphere(const Types::F32& radius, const Types::U32& slice, const Types::U32& stack)
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
        y = -std::cosf(verticalRadio); // unit height

        horizontalRadio = 0.0f;
        horizontalRadius = std::sinf(verticalRadio); // stack radius
        for (U32 sliceIndex = 0; sliceIndex < slice; ++sliceIndex)
        {
            x = std::cosf(horizontalRadio) * horizontalRadius;  // unit x
            z = - std::sinf(horizontalRadio) * horizontalRadius; // unit z

            retData.m_vertices.push_back(
                {   vector3(x, y, z) * radius,  // stretch position by radius
                    vector3(x,y,z) });          // normal

            horizontalRadio += DELTA_SLICE_RADIO;
        }

        verticalRadio += DELTA_STACK_RADIO;
    }// end for stackIndex

    // extra two vertex
    retData.m_vertices.push_back({ vector3(0.0f, -radius, 0.0f), vector3(0.0f, -1.0f, 0.0f) }); // push bottom vertex
    retData.m_vertices.push_back({ vector3(0.0f, +radius, 0.0f), vector3(0.0f, +1.0f, 0.0f) }); // push top vertex

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

CommonClass::MeshData GeometryBuilder::BuildGeoSphere(const Types::F32& radius, const Types::U32 subdivide /*= 0*/)
{
    using namespace Types;
    MeshData retData;

    const F32 X = 0.525731f;
    const F32 Z = 0.850651f;
    
    std::array<vector3, 12> pos = 
    {
        vector3(-X,   0.0f,  Z  ), vector3(X,   0.0f,  Z ),
        vector3(-X,   0.0f, -Z  ), vector3(X,   0.0f, -Z ),
        vector3(0.0f,  Z,    X  ), vector3(0.0f,  Z,  -X ),
        vector3(0.0f, -Z,    X  ), vector3(0.0f, -Z,  -X ),
        vector3( Z,    X,   0.0f), vector3(-Z,    X,  0.0f),
        vector3( Z,   -X,   0.0f), vector3(-Z,   -X,  0.0f)
    };// end array
    std::array<U32, 60> indicesOfPos =
    {
        1,4,0,   4,9,0, 4,5,9,  8,5,4,  1,8,4,
        1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
        3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
        10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
    };// end array

    retData.m_vertices.resize(pos.size());
    for (unsigned int i = 0; i < pos.size(); ++i)
    {
        retData.m_vertices[i] = { pos[i], pos[i]/* as normal */ };
    }
    retData.m_indices.assign(indicesOfPos.begin(), indicesOfPos.end());

    for (U32 i = 0; i < subdivide; ++i)
    {
        Subdivide(retData);
    }

    // reproject all vertex into the sphere.
    // and fix the normal to unit.
    for (auto& vertex : retData.m_vertices)
    {
        vertex.m_pos = Normalize(vertex.m_pos) * radius;
        vertex.m_normal = Normalize(vertex.m_normal);
    }

    return retData;
}

void GeometryBuilder::Subdivide(MeshData & target)
{
    using namespace Types;
    
    assert(target.m_indices.size() % 3 == 0 && "subdivision failed, incomplete triangle list");

    std::vector<MeshData::Vertex>& Vertices = target.m_vertices;     // reference to all vertices
    std::vector<U32> Indices = std::move(target.m_indices); // take over old indices
    target.m_indices.clear();                               // ensure all old indices are cleared

    const U32 NUM_ALL_TRIANGLE = Indices.size() / 3;
    const U32 NUM_PREV_VERTEX = Vertices.size();
    U32    I1,    I2,    I3;          // old vertex index
    U32 newI1, newI2, newI3; // interpolation vertex index

    for (U32 i = 0; i < NUM_ALL_TRIANGLE; ++i)
    {
        I1 = Indices[i * 3    ];
        I2 = Indices[i * 3 + 1];
        I3 = Indices[i * 3 + 2];

        MeshData::Vertex v1, v2, v3;

        // new vertex
        v1 = (Vertices[I2] + Vertices[I3]);
        v2 = (Vertices[I1] + Vertices[I3]);
        v3 = (Vertices[I1] + Vertices[I2]);
        v1.m_pos = v1.m_pos * 0.5f;
        v2.m_pos = v2.m_pos * 0.5f;
        v3.m_pos = v3.m_pos * 0.5f;
        v1.m_normal = v1.m_normal * 0.5f;
        v2.m_normal = v2.m_normal * 0.5f;
        v3.m_normal = v3.m_normal * 0.5f;

        // new corresponding index
        newI1 = Vertices.size();
        newI2 = newI1 + 1;
        newI3 = newI1 + 2;

        // append new vertex
        Vertices.push_back(v1);
        Vertices.push_back(v2);
        Vertices.push_back(v3);

                       
        //         <I1>                         
        //         /  \                          
        //        /    \                         
        // <newI3>------<newI2>                      
        //      /\      /\                           
        //     /  \    /  \                          
        //    /    \  /    \                         
        //  <I2>---<newI1>---<I3>

        // those faces will have the same winding order as before.
        // four triangles
        target.m_indices.push_back(I1);
        target.m_indices.push_back(newI3);
        target.m_indices.push_back(newI2);

        target.m_indices.push_back(I2);
        target.m_indices.push_back(newI1);
        target.m_indices.push_back(newI3);

        target.m_indices.push_back(I3);
        target.m_indices.push_back(newI2);
        target.m_indices.push_back(newI1);

        target.m_indices.push_back(newI1);
        target.m_indices.push_back(newI2);
        target.m_indices.push_back(newI3);
    }
}

CommonClass::MeshData::Vertex operator+(const MeshData::Vertex& v1, const MeshData::Vertex& v2)
{
    return {v1.m_pos + v2.m_pos, v1.m_normal + v2.m_normal};
}

}// namespace CommonClass
