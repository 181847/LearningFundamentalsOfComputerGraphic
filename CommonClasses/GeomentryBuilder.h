#include <vector>

#include "vector3.h"
/*!
    \brief this file declare some tools for generate basic geometries like cube, sphere, grid
*/

class GeometryBuilder
{
public:
    /*!
        \brief build a cube whose center is in the origin.
        \param oneSide one side of the cube,
        the count-clockwise is the front face.
    */
    static void BuildCube(const Types::F32& oneSide, std::vector<CommonClass::vector3> * outVertices, std::vector<unsigned int> * outIndices);
};