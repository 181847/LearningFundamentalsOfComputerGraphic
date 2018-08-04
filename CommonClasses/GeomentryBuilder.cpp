#include "GeomentryBuilder.h"
#include <array>
#include <assert.h>

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
