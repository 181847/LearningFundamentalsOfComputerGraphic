#pragma once
#include <memory>

#include "RasterizeImage.h"
#include "PiplineStateObject.h"
#include "ScreenSpaceVertexTemplate.h"
#include "F32Buffer.h"

namespace CommonClass
{

/*!
    \brief store the pipline light data, such as the VSVertexStrid,
    the count of index, the count of vertex.
*/
struct InstanceParameter
{

};

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

    /*!
        \brief draw line lists with out shader processing.
        \param indices indices of the line segments, length of it should be even
        \param vertices endpoints of line segments. they should have been transfered to screen space.
        \param vertexSizeInBytes the vertex size in bytes
        we assum the four float in front of the vertex is the screen space vertex location.
        -0.5 <= x <= pixelWidth - 0.5
        -0.5 <= y <= pixelHeight - 0.5
        -1 <= z <= 1
        w == 1
    */
    void DrawLineList(
        const std::vector<unsigned int>& indices, 
        const std::unique_ptr<F32Buffer> lineEndPointList,
        const unsigned int vertexSizeInBytes);

#ifdef _DEBUG
public:
#else
private:
#endif
    /*!
        \brief draw bresenhamLine.
        \param (x0, y0) start point location in screen space
        \param (x1, y1) ens point location in screen space
        \param realVertexSizeBytes the real vertex size in bytes.
    */
    void DrawBresenhamLine(
        const ScreenSpaceVertexTemplate*    pv1, 
        const ScreenSpaceVertexTemplate*    pv2, 
        const unsigned int                  realVertexSizeBytes);

    /*!
        \brief clip line in homogenous clipping space by the plane w = 0,
        \param pv1 input start vertex
        \param pv2 input end vertex
        \param pOutV1 output start vertex data
        \param pOutV2 output end vertex data
        \return whether this line is not visible which means that both w <= 0.
    */
    static bool WClipLineInHomogenousClipSpace(
        const ScreenSpaceVertexTemplate*    pv1,
        const ScreenSpaceVertexTemplate*    pv2,
        ScreenSpaceVertexTemplate*          pOutV1,
        ScreenSpaceVertexTemplate*          pOutV2,
        const unsigned int                  realVertexSize);

    /*!
        \brief clipping the line in homogenous clip space
        \param pv1 start point of the line
        \param pv2 end point of the line
        \param pOutV1 where to write clipped start point ( or not clipped ), address should't be the same as pv1
        \param pOutV2 where to write clipped end point ( or not clipped ), address should't be the same as pv2
        \return if the line can be drawn.(if the line is rejected, return false)
        warning! the 'pOutV' address shouldn't equal the address of 'pv'
        we assum the NDC space is in [-1, +1]^3,
        yes, z is mapped to -1 (near plane), +1 (far plane).
        reference from: ��Some Improvements to a Parametric Line Clipping Algorithm��Liang-Barsky
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
        const F32Buffer*                    vertices,
        const unsigned int                  realVertexSize,
        std::vector<unsigned int> *         pClippedIndices, 
        std::unique_ptr<F32Buffer> *        pClippedVertices);

private:

    /*!
        \brief first perspective divided, and then do the viewport transformation for the vertex stream.
        for each vertex, we assum the first four component is the homogenous coordinates location, 
        and all the vertex have been clipped, and we don't concern about the point out of range.
    */
    std::unique_ptr<F32Buffer> ViewportTransformVertexStream(
        std::unique_ptr<F32Buffer> verticesToBeTransformed, const unsigned int realVertexSizeBytes);

    /*!
        \brief for each vertex data, process it with vertex shader which is defined in the pipline state object
        \param pVertexStream the input vertex buffer stream
        \param vsInputStride the size(byte) of one vertex which will be passed into the vertexShader.
        \param vsOutputStrid the size(byte) of one vertex which will be returned by the vertexShader.
    */
    std::unique_ptr<F32Buffer> VertexShaderTransform(
        const F32Buffer* pVertexStream, const unsigned int vsInputStride, const unsigned int vsOutputStride);
};

} // namespace CommonClass
