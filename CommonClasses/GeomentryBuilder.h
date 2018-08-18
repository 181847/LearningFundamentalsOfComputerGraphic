#include <vector>

#include "vector3.h"
/*!
    \brief this file declare some tools for generate basic geometries like cube, sphere, grid
*/

namespace CommonClass
{

struct MeshData
{
    std::vector<CommonClass::vector3> m_vertices;
    std::vector<unsigned int> m_indices;
};

class GeometryBuilder
{
public:
    /*!
        \brief build a cube whose center is in the origin.
        \param oneSide one side of the cube,
        the count-clockwise is the front face.
    */
    static void BuildCube(const Types::F32& oneSide, std::vector<CommonClass::vector3> * outVertices, std::vector<unsigned int> * outIndices);

    /*!
        \brief build cylinder
        \param topRadius the radius on the top
        \param bottomRadius
        \param height the height of the cylinder
        \param slice split the circle into pieces
        \param stack split height
        \param needCap close bottom and top circle
    */
    static MeshData BuildCylinder(
        const Types::F32& topRadius, 
        const Types::F32& bottomRadius, 
        const Types::F32& height,
        const Types::U32& slice, 
        const Types::U32& stack,
        const bool        needCap);

    /*!
        \brief build a sphere 
        \param radius of sphere
        \param slice split vertically
        \param stack split horizontally
    */
    static MeshData BuildSphere(
        const Types::F32& radius,
        const Types::F32& slice,
        const Types::F32& stack);

    /*!
        \brief build a sphere by icosahedron
        \param subdivide subdivision level of the sphere, 0 means no subdivision.
    */
    static MeshData BuildGeoSphere(const Types::F32& radius, const Types::U32 subdivide = 0);

protected:
    /*!
        \brief subdivide the mesh data by interpolate vertex in each triangle, subdivides each triangle to four sub triangle.
        only triangle list supported.
    */
    static void Subdivide(MeshData& target);
};

}// namespace CommonClass