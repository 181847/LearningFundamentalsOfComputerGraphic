#pragma once
#include <memory>

#include "RasterizeImage.h"
#include "PiplineStateObject.h"
#include "ScreenSpaceVertexTemplate.h"
#include "F32Buffer.h"
#include "HPlaneEquation.h"

namespace CommonClass
{

/*!
    \brief abstraction of graphic pipline
*/
class Pipline
{
#ifdef _DEBUG
public:
#else
protected:
#endif
    /*!
        \brief store the pixels.
    */
    std::unique_ptr<RasterizeImage> m_backBuffer;
    
protected:
    /*!
        \brief a state object contain necessary info about how to draw primitives.
    */
    std::shared_ptr<PiplineStateObject> m_pso;

    std::vector<std::unique_ptr<HPlaneEquation>> m_frustumCutPlanes;

public:
    Pipline();
    ~Pipline();

    /*!
        \brief bind a back buffer to the pipline to store the rasterized image.
    */
    void SetBackBuffer(std::unique_ptr<RasterizeImage> backBuffer);

    /*!
        \brief set a pipline state object.
    */
    void SetPSO(std::unique_ptr<PiplineStateObject> pso);

    /*!
        \brief draw all the vertices with next data.
        \param indices the indices of all the vertices
        \param vertices the vertex data to be drawn, the data can be line list or triangle list.
    */
    void DrawInstance(
        const std::vector<unsigned int>& indices, 
        const F32Buffer*                 vertices);

    // next function will be used in the development phase, which will be public in the debug mode and private in the release mode.
#ifdef _DEBUG
public:
#else
private:
#endif
    /*!
        \brief draw one triangle
        \param pv1~3 three vertex of the triangle, in the screen space(x/y in pixel unit)
        \param realVertexSizeBytes the vertex size of the vertices, in byte unit.
    */
    void DrawTriangle(
        const ScreenSpaceVertexTemplate*    pv1,
        const ScreenSpaceVertexTemplate*    pv2,
        const ScreenSpaceVertexTemplate*    pv3,
        const unsigned int                  realVertexSizeBytes);

    /*!
        \brief find the pixel boundary of the triangle in the screen space
        \param pv1~3 three vertex and the function only care about the first two Float32 in each vertex which mean the {x,y} in screen space
        \param minBound return the min boundary of x/y, 0 -> x, 1 -> y.
        \param maxBound return the max boundary of x/y, 0 -> x, 1 -> y.
        the boundary depends on the viewport of current pipline to avoid out of the region of the backbuffer,
        for example, the image has the size of 512*128,
        then the pixel index should be in the region of [0,511]*[0,127],
        but during the viewport transformation stage, the NDC coordinate may exceed the index range,
          e.g. (-1,-1) in NDC space will be transformed to (-0.5,  -0.5 ) in the screen space,
               (+1,+1) in NDC space will be transformed to (511.5, 127.5) in the screen space
        But in the triangle rasterization stage, we will floor(minBound), ceil(maxBound) to cover the pixels as much as possible,
        which will result in the 'out of range' ERROR in the four edge of the backbuffer,
        As mentioned above, (-0.5, -0.5) will be floored to (-1, -1) which is out of range, and the same applies to (511.5, 127.5),
        so the solution is to check with viewport (which should have a boundary of [0,511]*[0,127] or even smaller) before we return the boundary result.
    */
    void FindTriangleBoundary(
        const ScreenSpaceVertexTemplate*    pv1,
        const ScreenSpaceVertexTemplate*    pv2,
        const ScreenSpaceVertexTemplate*    pv3,
        std::array<Types::U32, 2> *         minBound,
        std::array<Types::U32, 2> *         maxBound);

    /*!
        \brief draw triangle lists with out shader processing.
        \param indices indices of the triangle, whose length should be the times of three
        \param vertices vertices of triangles. they should have been transfered to screen space.
        \param vertexSizeInBytes the vertex size in bytes
        we assume the four float in front of the vertex is the screen space vertex location (x/y in pixel location).
        -0.5 <= x <= pixelWidth - 0.5
        -0.5 <= y <= pixelHeight - 0.5
        -1 <= z <= 1
        w == 1
    */
    void DrawTriangleList(
        const std::vector<unsigned int>&    indices, 
        std::unique_ptr<F32Buffer>          vertices, 
        const unsigned int                  psInputStride);

    /*!
        \brief 1 + 6 plane cut triangle, 
        \param pv1~3 three vertex for triangle
        \param realVertexSizeBytes the byte size of one single vertex
        \param outputStream return the cut result, each result is a TrianglePair that have one or two triangle,(zero is forbidden).
        \param fromPlane the plane index to start cutting(default to be zero: first plane in cutPlanes and to the last one in it), it's unnecessary for normal user to concern about this parameter.
    */
    static void FrustumCutTriangle(
        const ScreenSpaceVertexTemplate*                    pv1,
        const ScreenSpaceVertexTemplate*                    pv2,
        const ScreenSpaceVertexTemplate*                    pv3,
        const unsigned int                                  realVertexSizeBytes,
        std::vector<TrianglePair> *                         outputStream, 
        const std::vector<std::unique_ptr<HPlaneEquation>>& cutPlanes,
        const size_t                                        fromPlane = 0);

private:

    /*!
        \brief draw line lists in screen space
        \param indices indices of the line segments, whose length should be even
        \param vertices endpoints of line segments. they should have been transfered to screen space.
        \param vertexSizeInBytes the vertex size in bytes
        we assume the four float in front of the vertex is the screen space vertex location (x/y in pixel location).
        -0.5 <= x <= pixelWidth - 0.5
        -0.5 <= y <= pixelHeight - 0.5
        -1 <= z <= 1
        w == 1
    */
    void DrawLineList(
        const std::vector<unsigned int>&    indices, 
        const std::unique_ptr<F32Buffer>    lineEndPointList,
        const unsigned int                  vertexSizeInBytes);

    /*!
        \brief draw one bresenhamLine.
        \param (x0, y0) start point location in screen space
        \param (x1, y1) ens point location in screen space
        \param realVertexSizeBytes the real vertex size in bytes.
    */
    void DrawBresenhamLine(
        const ScreenSpaceVertexTemplate*    pv1, 
        const ScreenSpaceVertexTemplate*    pv2, 
        const unsigned int                  realVertexSizeBytes);

    /*!
        \brief clip line in homogeneous clipping space by the plane w = 0
        \param pv1 input start vertex
        \param pv2 input end vertex
        \param pOutV1 output start vertex data
        \param pOutV2 output end vertex data
        \return whether this line is not visible which means that both w <= 0.
        This function will ensure the fourth component of homogeneous location (or so-called 'w') will be Greater than ZERO,
        even the w is exactly zero, we will cut that to a epsilon(maybe +1e-20) to avoid dividing by zero in perspective dividing.
    */
    static bool WClipLineInHomogenousClipSpace(
        const ScreenSpaceVertexTemplate*    pv1,
        const ScreenSpaceVertexTemplate*    pv2,
        ScreenSpaceVertexTemplate*          pOutV1,
        ScreenSpaceVertexTemplate*          pOutV2,
        const unsigned int                  realVertexSize);

    /*!
        \brief clipping the line in homogenous clip space, ensure after perspective divided, x,y,z in [-1, +1], w = 1.
        \param pv1 start point of the line
        \param pv2 end point of the line
        \param pOutV1 where to write clipped start point ( or not clipped ), address should't be the same as pv1
        \param pOutV2 where to write clipped end point ( or not clipped ), address should't be the same as pv2
        \return if the line can be drawn.(if the line is rejected, return false)
        Warnings:
        1. the 'pOutV' address shouldn't equal the address of 'pv'
        2. we assum the NDC space is in [-1, +1]^3, and z should be mapped to -1 (near plane), +1 (far plane).
        3. all the those homogenous location MUST have a positive W component (NOT even zero).
        reference from: ¡¶Some Improvements to a Parametric Line Clipping Algorithm¡·Liang-Barsky
    */
    static bool ClipLineInHomogenousClipSpace(
        const ScreenSpaceVertexTemplate*    pv1,
        const ScreenSpaceVertexTemplate*    pv2,
        ScreenSpaceVertexTemplate*          pOutV1,
        ScreenSpaceVertexTemplate*          pOutV2,
        const unsigned int                  realVertexSize);

    /*!
        \brief clipe line list in homogenous clip space, where the perspective divide haven't been done.
        \param indices input line list indices
        \param vertices input vertex data
        \param realVertexSize the vertes size of the input vertices in Bytes
        \param pClippedIndices the index data afther clipping
        \param pClippedVertices the vertex data afther clipping.
    */
    static void ClipLineList(
        const std::vector<unsigned int>&    indices, 
        std::unique_ptr<F32Buffer>          vertices,
        const unsigned int                  realVertexSize,
        std::vector<unsigned int> *         pClippedIndices, 
        std::unique_ptr<F32Buffer> *        pClippedVertices);

    /*!
        \brief clipe triangle list in homogenous clip space, where the perspective divide haven't been done.
        \param indices input triangle list indices
        \param vertices input vertex data
        \param realVertexSize the vertes size of the input vertices in Bytes
        \param pClippedIndices the index data afther clipping
        \param pClippedVertices the vertex data afther clipping.
        \param cutPlanes the planes used to perform cutting
    */
    static void ClipTriangleList(
        const std::vector<unsigned int>&                    indices,
        std::unique_ptr<F32Buffer>                          vertices,
        const unsigned int                                  realVertexSize,
        std::vector<unsigned int> *                         pClippedIndices,
        std::unique_ptr<F32Buffer> *                        pClippedVertices,
        const std::vector<std::unique_ptr<HPlaneEquation>>& cutPlanes);

    /*!
        \brief first perspective divided, and then do the viewport transformation for the vertex stream.
        for each vertex, we assum the first four component is the homogenous coordinates location, 
        and all the vertex have been clipped, and we don't concern about the point out of range.
    */
    std::unique_ptr<F32Buffer> ViewportTransformVertexStream(
        std::unique_ptr<F32Buffer>          verticesToBeTransformed, 
        const unsigned int                  realVertexSizeBytes);

    /*!
        \brief for each vertex data, process it with vertex shader which is defined in the pipline state object
        \param pVertexStream the input vertex buffer stream
        \param vsInputStride the size(byte) of one vertex which will be passed into the vertexShader.
        \param vsOutputStrid the size(byte) of one vertex which will be returned by the vertexShader.
        return a new block of memory for the result of the transform.
    */
    std::unique_ptr<F32Buffer> VertexShaderTransform(
        const F32Buffer*                    pVertexStream, 
        const unsigned int                  vsInputStride, 
        const unsigned int                  vsOutputStride);
};

} // namespace CommonClass
