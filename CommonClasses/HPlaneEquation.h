#pragma once
#include "ScreenSpaceVertexTemplate.h"
#include "F32Buffer.h"
#include <memory>
#include <array>
#include <assert.h>

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
        \templateParam POINTER_T the pointer type, e.g. void => void * , int => int *
        if the TrianglePair have ONE triangle, index can be {0, 1, 2}.
        if the TrianglePair have TWO triangle, index can be {0, 1, 2, 3}.
    */
    template<typename POINTER_T = unsigned char>
    POINTER_T * GetVertexPointer(const unsigned short index)
    {
        // 1. m_count MUST NOT be zero, or no vertex exist
        // 2. if m_count = ONE(1), index <= 2
        // 3. if m_count = TWO(2), index <= 3
        assert(m_count > 0 && index < (2 + m_count));
        return GetVertexPtrAt<POINTER_T>(m_vertices->GetBuffer(), index, m_vertexSizeInByte);
    }

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

    virtual ~HPlaneEquation() {}

private:
    /*!
        \brief evaluate the homogeneous point is whether inside the 4D homogeneous plane
        \param pointH the position in the homogeneous coordinate
        generally speaking, the positive result mean that inside the plane.
    */
    virtual Types::F32 eval(const vector4& pointH) = 0;

    /*!
        \brief calculate the interpolate coefficient
        \param point1~2 two point in homogeneous coordinate
        the function don't have to consider the situation where no real solution exist,
        because the coefficient should be calculated only by that the segment is acrossing the plane.

    */
    virtual Types::F32 cutCoefficient(const vector4& point1, const vector4& point2) = 0;
};

/*!
    \brief this class is used to cut triangle by W = 0 plane.
    and ensure all the homogenouse have a Negative W.
*/
class WZeroHPlaneEquation : public HPlaneEquation
{
public:
    Types::F32 eval(const vector4& pointH) override;
    
    Types::F32 cutCoefficient(const vector4& point1, const vector4& point2) override;
};

/*!
    \brief near frustum plane cut equation, this assume all the vector4 have a positive w.
*/
class ZeroNearPlaneEquation : public HPlaneEquation
{
public:
    Types::F32 eval(const vector4& pointH) override;
    Types::F32 cutCoefficient(const vector4& point1, const vector4& point2) override;
};

/*!
    \brief ChooseSign is used for compile time sign choose.
    for example:
        ChooseSign<true>::Sign() will return +1.0f in compile time.
        ChooseSign<false>::Sign() will return -1.0f.
*/
template<bool CHOOSE_POSITIVE>
class ChooseSign
{
public:
    static constexpr Types::F32 Sign()
    {
        return +1.0f;
    }
};
template<>
class ChooseSign<false>
{
public:
    static constexpr Types::F32 Sign()
    {
        return -1.0f;
    }
};


/*!
    \brief help marcos for FrustumHPlaneEquation
*/
#define LEFT_FRUSTUM_PLANE    0, true
#define RIGHT_FRUSTUM_PLANE   0, false
#define BOTTOM_FRUSTUM_PLANE  1, true
#define TOP_FRUSTUM_PLANE     1, false
#define NEAR_FRUSTUM_PLANE    2, true
#define FAR_FRUSTUM_PLANE     2, false
/*!
    \brief each plane of the frustum in the homogeneous space.
    \templateParam XYZ, choose the plane is perpendicular to which axis.X(0), Y(1), Z(2)
    \templateParam B_LEFT_BOTTOM_FAR is the plane left/bottom/far plane?
*/
template<unsigned short XYZ, bool B_LEFT_BOTTOM_NEAR>
class FrustumHPlaneEquation : public HPlaneEquation
{
public:
    /*
        \brief if the plane is the higher bound, return 
    */
    const float SIGN = ChooseSign<B_LEFT_BOTTOM_NEAR>::Sign();

    /*!
        \brief which axis is choose in the plane,
        if XYZ = 0, the axis means X, so we will get the value of pointX.m_x, whose index inside the vector4 is 0.
        and so on.
    */
    const unsigned short CHOOSE_AXIS = XYZ;

    Types::F32 eval(const vector4& pointH) override
    {
        const Types::F32 axis(pointH.m_arr[CHOOSE_AXIS]);
        return pointH.m_w + SIGN * axis;
    }

    Types::F32 cutCoefficient(const vector4& point1, const vector4& point2) override
    {
        Types::F32 w1(point1.m_w),                  w2(point2.m_w);

        Types::F32 axis1(point1.m_arr[CHOOSE_AXIS]),    axis2(point2.m_arr[CHOOSE_AXIS]);

        Types::F32 numerator   = ( - SIGN) * axis1 - w1;
        Types::F32 denominator = w2 - w1 + SIGN * (axis2 - axis1);

        return numerator / denominator;
    }
};

}// namespace CommonClass
