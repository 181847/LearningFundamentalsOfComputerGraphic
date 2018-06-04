#include "Pipline.h"
#include <array>
#include "DebugConfigs.h"
#include "EFloat.h"

namespace CommonClass
{

Pipline::Pipline()
{
}

Pipline::~Pipline()
{
}

void Pipline::SetBackBuffer(std::unique_ptr<RasterizeImage> backBuffer)
{
    m_backBuffer = std::move(backBuffer);
}

void Pipline::SetPSO(std::unique_ptr<PiplineStateObject> pso)
{
    m_pso = std::move(pso);
}

void Pipline::DrawInstance(const std::vector<unsigned int>& indices, const F32Buffer * vertices)
{
    if (nullptr == m_pso.get())
    {
        throw std::exception("lack of pipline state object");
    }

    if (m_pso->m_primitiveType != PrimitiveType::LINE_LIST)
    {
        throw std::exception("unsupported primitive type");
    }

    if (m_pso->m_pixelShader == nullptr)
    {
        throw std::exception("pipline state object lack of pixel shader.");
    }

    if (m_pso->m_vertexShader == nullptr)
    {
        throw std::exception("pipline state object lack of vertex shader.");
    }

    // the vertex size which will be passed to vertexShader
    const unsigned int vsInputStride = m_pso->m_vertexLayout.vertexShaderInputSize;
    // the vertex size which will be passed to pixelShader
    const unsigned int psInputStride = m_pso->m_vertexLayout.pixelShaderInputSize;
    
    // process each vertex with vertexShader
    std::unique_ptr<F32Buffer> vsOutputStream = VertexShaderTransform(vertices, vsInputStride, psInputStride);

    std::vector<unsigned int>  clippedIndices;   // the index data that has been clipped.
    std::unique_ptr<F32Buffer> clippedLineData;  // the vertex data that has been clipped.

    // clip all the line
    ClipLineList(indices, vsOutputStream.get(), psInputStride, &clippedIndices, &clippedLineData);

#ifdef _DEBUG
    // all line has been clipped. return
    if (clippedIndices.size() == 0)
    {
        printf("clipped all line, no line rests\n");
        return;
    }
#endif

    // indices after clipping
    const unsigned int numIndices = clippedIndices.size();
    // the byte size of vertex data after clipping
    const unsigned int numBytes = clippedLineData->GetSizeOfByte();

    // now the pipline is not complete, so for the simplification, we assume all the vertex for each shader is in the same size.
    assert(vsInputStride == psInputStride);
    assert(numBytes % vsInputStride == 0 && "vertics data error, cannot ensure every vertex data is complete.");

    auto viewportTransData = ViewportTransformVertexStream(std::move(clippedLineData), psInputStride);

    DrawLineList(clippedIndices, std::move(viewportTransData), psInputStride);
}

void Pipline::DrawLineList(
    const std::vector<unsigned int>& indices, 
    const std::unique_ptr<F32Buffer> lineEndPointList,
    const unsigned int vertexSizeInBytes)
{
    // Get index count, ensure they are even
    const unsigned int numIndices = indices.size();
    if (numIndices % 2 != 0)
    {
        throw std::exception("indices of line list is not even");
    }

    //const unsigned int vertexStride = m_pso->m_vertexLayout.pixelShaderInputSize;
    const unsigned int numVertices = lineEndPointList->GetSizeOfByte() / vertexSizeInBytes;
    unsigned char * pDataStart = lineEndPointList->GetBuffer();

    // for each two points, draw a segment
    if (numIndices <= 0)
    {
        throw std::exception("DrawLineList get no index.");
    }

    // loop throung all indices, each loop will pass two indices.
    for (unsigned int i = 0; i < numIndices - 1; i += 2)
    {
        const ScreenSpaceVertexTemplate* pv1 = reinterpret_cast<const ScreenSpaceVertexTemplate *>(pDataStart + indices[i    ] * vertexSizeInBytes);
        const ScreenSpaceVertexTemplate* pv2 = reinterpret_cast<const ScreenSpaceVertexTemplate *>(pDataStart + indices[i + 1] * vertexSizeInBytes);

        // handle the vertex data to single line drawing function,
        // in which will draw them with pixel shader.
        DrawBresenhamLine(pv1, pv2, vertexSizeInBytes);
    }
}

void Pipline::DrawBresenhamLine(const ScreenSpaceVertexTemplate* pv1, const ScreenSpaceVertexTemplate* pv2, const unsigned int realVertexSizeBytes)
{
    // get screen space location.
    Types::I32 x0, y0, x1, y1;
    x0 = static_cast<Types::I32>(pv1->m_posH.m_x);
    y0 = static_cast<Types::I32>(pv1->m_posH.m_y);
    x1 = static_cast<Types::I32>(pv2->m_posH.m_x);
    y1 = static_cast<Types::I32>(pv2->m_posH.m_y);

    
    bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);
    // ensure the absolute value of slope of the line is less than one.
    if (steep)
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    // ensure we draw line from left to right
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);

        // if we swap (x0, y0) with (x1, y1)
        // we should also swap two vertex to get the correct interplotion order.
        std::swap(pv1, pv2);
    }

    // bresenham algorithm coefficients.
    Types::I32 dx = x1 - x0;
    Types::I32 twoDy = std::abs(2 * (y1 - y0));
    Types::I32 yi = y1 > y0 ? 1 : -1;
    Types::I32 twoDx = 2 * dx;
    Types::I32 y = y0;
    Types::I32 error = twoDy - dx;
    Types::I32 twoDyMinusTwoDx = error - dx;

    // get pixel shader
    auto & pixelShader = m_pso->m_pixelShader;

    // create interpolated vertex buffer
    //const unsigned int PSVInputeSize = m_pso->m_vertexLayout.pixelShaderInputSize;
    F32Buffer pixelShaderInputBuffer(realVertexSizeBytes);
    // reinterpret it as ScreenSpaceVertexTemplate.
    ScreenSpaceVertexTemplate* pPSVInput = reinterpret_cast<ScreenSpaceVertexTemplate *>(pixelShaderInputBuffer.GetBuffer());

    // prepare interpolation coefficience, for pixel interpolation.
    Types::F32 t = 0.0f, dt = 1.0f / dx;    // "t" is used to interpolate between (x0, y0) and (x1, y1);
    for (auto x = x0; x <= x1; ++x)
    {
        Interpolate2(pv1, pv2, pPSVInput, t, realVertexSizeBytes);
        if (steep)
        {
            //m_backBuffer->SetPixel(y, x, RGB::BLACK);
            m_backBuffer->SetPixel(y, x, pixelShader(pPSVInput));
        }
        else
        {
            //m_backBuffer->SetPixel(x, y, RGB::BLACK);
            m_backBuffer->SetPixel(x, y, pixelShader(pPSVInput));
        }

        // update interpolation coefficience.
        t += dt;

        if (error > 0)
        {
            y += yi;
            error += twoDyMinusTwoDx;
        }
        else
        {
            error += twoDy;
        }
    }
}

bool Pipline::WClipLineInHomogenousClipSpace(const ScreenSpaceVertexTemplate * pv1, const ScreenSpaceVertexTemplate * pv2, ScreenSpaceVertexTemplate * pOutV1, ScreenSpaceVertexTemplate * pOutV2, const unsigned int realVertexSize)
{
    // is the w of two vertex position is greater or equal to zero?
    std::array<bool, 2> isWPositive = { pv1->m_posH.m_w >= 0.0f, pv2->m_posH.m_w >= 0.0f };

    // When cut w to zero, we will keep w little far from 0.0 to avoid
    // divided by zero, and you must notice that here we must use a positive small number,
    // to keep all the W to be positive.
    const Types::F32 EPSILON_TO_ZERO = 1e-20f;

    // if both w is positive, then just copy the vertex data and return true.
    if (isWPositive[0] && isWPositive[1])
    {
        memcpy(pOutV1, pv1, realVertexSize);
        memcpy(pOutV2, pv2, realVertexSize);
        return true;
    }
    // if both w is postive
    else if ( ! (isWPositive[0] || isWPositive[1]))
    {
        // reject the line
        return false;
    }
    else if (isWPositive[0])
    {
        // w1 is negative and w2 is positive
        const Types::F32 deltaW = pv1->m_posH.m_w - pv2->m_posH.m_w;
        const Types::F32 t = pv1->m_posH.m_w / deltaW;

        // copy start vertex data.
        memcpy(pOutV1, pv1, realVertexSize);

        Interpolate2(pv1, pv2, pOutV2, t, realVertexSize);

        // manually prevent w to be exactlly zero.
        pOutV2->m_posH.m_w = EPSILON_TO_ZERO;
    }
    else
    {
        // w1 is positive and w2 is negative
        const Types::F32 deltaW = pv2->m_posH.m_w - pv1->m_posH.m_w;
        const Types::F32 t = pv2->m_posH.m_w / deltaW;  // negative divide negative result in positive

        // copy end vertex data.
        memcpy(pOutV2, pv2, realVertexSize);

        Interpolate2(pv2, pv1, pOutV1, t, realVertexSize);

        // manually prevent w to be exactlly zero.
        pOutV1->m_posH.m_w = EPSILON_TO_ZERO;
    }

    return true;
}

bool Pipline::ClipLineInHomogenousClipSpace(
    const ScreenSpaceVertexTemplate* pv1,
    const ScreenSpaceVertexTemplate* pv2,
    ScreenSpaceVertexTemplate* pOutV1,
    ScreenSpaceVertexTemplate* pOutV2, 
    const unsigned int realVertexSize)
{
    assert(pv1 != nullptr && pv2 != nullptr && pOutV1 != nullptr && pOutV2 != nullptr && "null pointer error");
    assert(pv1 != pOutV1 && pv2 != pOutV2 && pv1 != pOutV2 && pv2 != pOutV1 && "pointer address conflict error, the data will be wrong");

    // whether to cut the vector to be in the exact visible region
    // when some number error resist in the last cutting result.
//#define MANUALLY_CUT_HVECTOR

     // whether to reinforce the checks on xyz component in the last step of the cuttin
//#define MANUALLY_CHECK_XYZ_BOUNDg.


    std::array<Types::F32, 6> p;
    std::array<Types::F32, 6> q;

    Types::F32 t0 = 0.0f;       // start point, zero means the pv1
    Types::F32 t1 = 1.0f;       // end point, one means the pv2
    Types::F32 tempT = 0.0f;    // temp interpolate coefficience

    const Types::F32 deltaX = pv2->m_posH.m_x - pv1->m_posH.m_x;
    const Types::F32 deltaY = pv2->m_posH.m_y - pv1->m_posH.m_y;
    const Types::F32 deltaZ = pv2->m_posH.m_z - pv1->m_posH.m_z;
    const Types::F32 deltaW = pv2->m_posH.m_w - pv1->m_posH.m_w;

    p[0] = -(deltaX + deltaW);
    p[1] = deltaX - deltaW;
    p[2] = -(deltaY + deltaW);
    p[3] = deltaY - deltaW;
    p[4] = -(deltaZ + deltaW);
    p[5] = deltaZ - deltaW;

    q[0] = pv1->m_posH.m_x + pv1->m_posH.m_w;
    q[1] = pv1->m_posH.m_w - pv1->m_posH.m_x;
    q[2] = pv1->m_posH.m_y + pv1->m_posH.m_w;
    q[3] = pv1->m_posH.m_w - pv1->m_posH.m_y;
    q[4] = pv1->m_posH.m_z + pv1->m_posH.m_w;
    q[5] = pv1->m_posH.m_w - pv1->m_posH.m_z;

    for (unsigned int i = 0; i < 6; ++i)
    {
        if (std::abs(p[i] - 0.0f) < Types::Constant::EPSILON_F32)
        {
            if (q[i] * pv1->m_posH.m_w < 0.0f)
            {
                return false;
            }
        }
        else if (p[i] > 0.0f)
        {
            tempT = q[i] / p[i];
            if (tempT < t0)
            {
                // new end point is after start point.
                // early reject
                return false;
            }
            else if (tempT < t1)
            {
                // update t1, new end point
                t1 = tempT;
            }
        }
        else // p[i] < 0.0f
        {
            tempT = q[i] / p[i];
            if (tempT > t1)
            {
                // new start point is beyond end point.
                // early reject
                return false;
            }
            else if (tempT > t0)
            {
                // update t1, new start point
                t0 = tempT;
            }
        } // end else if p[i] > 0.0f
    }// end else for
    
#ifdef MANUALLY_CUT_HVECTOR
    // the function is a temporary solution for the clipped point will outside of the boundary
    // prefix the hvector for homogenous clip
    // for example, if w == -2.3, and x == -2.4,
    // after the perspective divide, the x will be 1.04... which is greater than one,
    // to prevent this overflowing error, just make x = equal to w.
    auto CutHvector = [](hvector & vec)->void {

        if (vec.m_w > 0.0f) 
        {
            if (vec.m_x < -vec.m_w)
            {
                vec.m_x = -vec.m_w;
            }
            else if (vec.m_x > vec.m_w)
            {
                vec.m_x = vec.m_w;
            }

            if (vec.m_y < -vec.m_w)
            {
                vec.m_y = -vec.m_w;
            }
            else if (vec.m_y > vec.m_w)
            {
                vec.m_y = vec.m_w;
            }

            if (vec.m_z < -vec.m_w)
            {
                vec.m_z = -vec.m_w;
            }
            else if (vec.m_z > vec.m_w)
            {
                vec.m_z = vec.m_w;
            }
        } // end if vec.m_w > 0.0f
        else
        {
            if (vec.m_x > -vec.m_w)     // positive greater -> -1 in NDC
            {
                vec.m_x = -vec.m_w;
            }
            else if (vec.m_x < vec.m_w) // negative less -> +1 in NDC
            {
                vec.m_x = vec.m_w;
            }

            if (vec.m_y > -vec.m_w)     // positive greater -> -1 in NDC
            {
                vec.m_y = -vec.m_w;
            }
            else if (vec.m_y < vec.m_w) // negative less -> +1 in NDC
            {
                vec.m_y = vec.m_w;
            }

            if (vec.m_z > -vec.m_w)     // positive greater -> -1 in NDC
            {
                vec.m_z = -vec.m_w;
            }
            else if (vec.m_z < vec.m_w) // negative less -> +1 in NDC
            {
                vec.m_z = vec.m_w;
            }
        } // end else if vec.m_w > 0.0f

    }; // lambda CutHvector

#endif // MANUALLY_CUT_HVECTOR

    if (t0 == 0.0f)
    {
        // just copy original start point data.
        memcpy(pOutV1, pv1, realVertexSize);
    }
    else
    {
        Interpolate2( pv1, pv2, 
            pOutV1,
            static_cast<Types::F32>(t0),
            realVertexSize);

#ifdef MANUALLY_CUT_HVECTOR
        CutHvector(pOutV1->m_posH);     // fix numeric issue when the clipped point will out of the frustum
#endif // MANUALLY_CUT_HVECTOR

    }
    
    if (t1 == 1.0f)
    {
        // just copy original end point data.
        memcpy(pOutV2, pv2, realVertexSize);
    }
    else
    {
        Interpolate2(
            pv1, pv2, 
            pOutV2,
            static_cast<Types::F32>(t1),
            realVertexSize);

#ifdef MANUALLY_CUT_HVECTOR
        CutHvector(pOutV2->m_posH);     // fix numeric issue when the clipped point will out of the frustum
#endif // MANUALLY_CUT_HVECTOR

    }

    // an ensurance that all the components should be limited in [-1, +1].

    
#ifdef MANUALLY_CHECK_XYZ_BOUND

    const Types::F32 
        x0(pOutV1->m_posH.m_x / pOutV1->m_posH.m_w), 
        y0(pOutV1->m_posH.m_y / pOutV1->m_posH.m_w),
        z0(pOutV1->m_posH.m_z / pOutV1->m_posH.m_w),
        x1(pOutV2->m_posH.m_x / pOutV2->m_posH.m_w),
        y1(pOutV2->m_posH.m_y / pOutV2->m_posH.m_w),
        z1(pOutV2->m_posH.m_z / pOutV2->m_posH.m_w);

#define OUT_RANGE_1(component) (component < (-1.0f - 0.01f) || (+1.0f + 0.01f) < component)

    BREAK_POINT_IF(
        OUT_RANGE_1(x0)
        || OUT_RANGE_1(y0)
        || OUT_RANGE_1(z0)
        || OUT_RANGE_1(x1)
        || OUT_RANGE_1(y1)
        || OUT_RANGE_1(z1));

#endif // MANUALLY_CHECK_XYZ_BOUND

    return true;
}

void Pipline::ClipLineList(
    const std::vector<unsigned int>&    indices, 
    const F32Buffer *                   vertices,
    const unsigned int                  realVertexSize, 
    std::vector<unsigned int> *         pClippedIndices, 
    std::unique_ptr<F32Buffer> *        pClippedVertices)
{
    // For each line segment, the clip test will be executed, and return a new line segment,
    // now we don't have optimization on reusing vertex by same index.
    
    assert(realVertexSize % 4 == 0                                      && "vertex size is not the times of four");
    assert(pClippedIndices != nullptr && pClippedVertices != nullptr    && "argument nullptr error");
    assert(indices.size() % 2 == 0                                      && "line indices is not pairs");

    // empty the output indice buffer.
    pClippedIndices->clear();
    unsigned int numLineSegment = indices.size() / 2;                           // number of all line segments

    // create a buffer stream which is big enough to hold every individual vertex for each line segment.
    // each end point will have its own vertex.
    auto clippedStream = std::make_unique<F32Buffer>(realVertexSize * 2 * numLineSegment); // allocate buffer for clipped data

    const unsigned int twoRealVertexSizeBytes = 2 * realVertexSize;

    unsigned char * pSrcVerticesAddr    = vertices->GetBuffer();                // source vertex start address

    unsigned char * pStartSrcVertex     = nullptr;                              // src start vertex location
    unsigned char * pEndSrcVertex       = nullptr;                              // src end vertex location
    unsigned char * pStartClippedVertex = clippedStream->GetBuffer();           // output clipped start vertex
    unsigned char * pEndClippedVertex   = pStartClippedVertex + realVertexSize; // output clipped end vertex

    // before clipping in homogenous space, let's first clip the vertex data into w > 0
    // create a temp buffer for that result, which will contain only two vertex.
    auto            wClipBuffer         = std::make_unique<F32Buffer>(2 * realVertexSize);
    unsigned char * pWClipStartVertex   = wClipBuffer->GetBuffer();               // temp buffer for start vertex of the w clipping
    unsigned char * pWClipEndVertex     = pWClipStartVertex + realVertexSize;     // temp buffer for end vertex of the w clipping

    unsigned int    startClippedVertexIndex = 0;
    unsigned int    endClippedVertexIndex   = 1;
    unsigned int    numClippedVertex        = 0;                                  // how many vertex is added to clippedStream

    bool canDrawThisSegment = false;                                              // for each clipping test, is this line can be draw,(or the line is rejected).
    for (unsigned int i = 0; i < numLineSegment; ++i)
    {
        // find source vertices of line segment
        pStartSrcVertex = GetVertexPtrAt(pSrcVerticesAddr, indices[ i * 2 ],     realVertexSize);
        pEndSrcVertex   = GetVertexPtrAt(pSrcVerticesAddr, indices[ i * 2 + 1 ], realVertexSize);

        // proceed w=0 plane clipping.
        canDrawThisSegment = WClipLineInHomogenousClipSpace(
            reinterpret_cast<ScreenSpaceVertexTemplate *>(pStartSrcVertex),
            reinterpret_cast<ScreenSpaceVertexTemplate *>(pEndSrcVertex),
            reinterpret_cast<ScreenSpaceVertexTemplate *>(pWClipStartVertex),
            reinterpret_cast<ScreenSpaceVertexTemplate *>(pWClipEndVertex),
            realVertexSize);

        if ( ! canDrawThisSegment)
        {
            // the line's two end point all have negative w, reject them.
            continue;
        }

        // homogenous space clipping test
        canDrawThisSegment = ClipLineInHomogenousClipSpace(
            reinterpret_cast<ScreenSpaceVertexTemplate *>(pWClipStartVertex),
            reinterpret_cast<ScreenSpaceVertexTemplate *>(pWClipEndVertex),
            reinterpret_cast<ScreenSpaceVertexTemplate *>(pStartClippedVertex), 
            reinterpret_cast<ScreenSpaceVertexTemplate *>(pEndClippedVertex), 
            realVertexSize);

        if (!canDrawThisSegment)
        {
            // line is rejected, skip it.
            continue;
        }

        // output vertex address move on
        pStartClippedVertex += twoRealVertexSizeBytes;
        pEndClippedVertex   += twoRealVertexSizeBytes;

        // push vertex index
        pClippedIndices->push_back(startClippedVertexIndex);
        pClippedIndices->push_back(endClippedVertexIndex);

        // increase vertexIndex to next
        startClippedVertexIndex += 2;
        endClippedVertexIndex   += 2;

        numClippedVertex += 2;

    }// end for line segments

    // after clipping, is the clipped vertices cost same space as we allocated?
    const unsigned int realClippedStreamSize = numClippedVertex * realVertexSize;
    if (realClippedStreamSize < clippedStream->GetSizeOfByte())
    {
        // if the vertices of visible line segment use less space than clippedStream stream
        // creat another buffer stream which will only contain the useable data.
        auto shrinkClippedStream = std::make_unique<F32Buffer>(realClippedStreamSize);

        memcpy(shrinkClippedStream->GetBuffer(), clippedStream->GetBuffer(), shrinkClippedStream->GetSizeOfByte());
        
        // return vertex data
        *pClippedVertices = std::move(shrinkClippedStream);
    }
    else if (realClippedStreamSize == clippedStream->GetSizeOfByte())
    {
        *pClippedVertices = std::move(clippedStream);
    }
    else
    {
        assert(false && "Warning! After clipping, we get more vertices than expected, there must be some error.");
    }
}

std::unique_ptr<F32Buffer> Pipline::ViewportTransformVertexStream(std::unique_ptr<F32Buffer> verticesToBeTransformed, const unsigned int realVertexSizeBytes)
{
    
    const unsigned int  numVertices             = verticesToBeTransformed->GetSizeOfByte() / realVertexSizeBytes;   // compute the number of vertex.
    auto                viewportTransData       = std::make_unique<F32Buffer>(numVertices * realVertexSizeBytes);   // create transfered data buffer.

    unsigned char *     pSrcFloat               = verticesToBeTransformed->GetBuffer();                             // point to the vertex data before viewport transformation, a address in clippedLineData.
    unsigned char *     pDestFloat              = viewportTransData->GetBuffer();                                   // point to the vertex data after viewport transformation, address in viewportTransData.

    Transform&          viewportTransformMat    = m_pso->m_viewportTransform;

    // loop through all vertices.
    for (unsigned int i = 0; i < numVertices; ++i)
    {
        ScreenSpaceVertexTemplate* pSrcVertex  = reinterpret_cast<ScreenSpaceVertexTemplate * >(pSrcFloat);     // source data
        ScreenSpaceVertexTemplate* pDestVertex = reinterpret_cast<ScreenSpaceVertexTemplate * >(pDestFloat);    // transformed to

        // copy the memory of the vertex, ensure the data (except the location) is same.
        memcpy(pDestVertex, pSrcVertex, realVertexSizeBytes);

        if (pDestVertex->m_posH.m_w != 1.0f)
        {
            // perspective divided
            const Types::F32 RECIPOCAL_W = 1.0f / pDestVertex->m_posH.m_w;
            pDestVertex->m_posH.m_x *= RECIPOCAL_W;
            pDestVertex->m_posH.m_y *= RECIPOCAL_W;
            pDestVertex->m_posH.m_z *= RECIPOCAL_W;
            pDestVertex->m_posH.m_w = 1.0f;
        }

        // transfrom
        pDestVertex->m_posH = viewportTransformMat * pDestVertex->m_posH;

        // move to next data.
        pSrcFloat += realVertexSizeBytes;
        pDestFloat += realVertexSizeBytes;
    } // end for vertices.

    return viewportTransData;
}

std::unique_ptr<F32Buffer> Pipline::VertexShaderTransform(const F32Buffer * pVertexStream, const unsigned int vsInputStride, const unsigned int vsOutputStride)
{
    assert(pVertexStream != nullptr);

    const unsigned int  sizeOfInputStream   = pVertexStream->GetSizeOfByte();
    assert(sizeOfInputStream % vsInputStride == 0 && "vertexShader stream input error, the size is not complete.");

    const unsigned int  numVertex           = sizeOfInputStream / vsInputStride;

    auto                vertexOutputStream  = std::make_unique<F32Buffer>(numVertex * vsOutputStride);

    unsigned char *     pVSInput            = pVertexStream->GetBuffer();
    unsigned char *     pVSOutput           = vertexOutputStream->GetBuffer();

    auto                vertexShader = m_pso->m_vertexShader;

    for (unsigned int i = 0; i < numVertex; ++i)
    {
        vertexShader(pVSInput, reinterpret_cast<ScreenSpaceVertexTemplate*>(pVSOutput));

        pVSInput  += vsInputStride;
        pVSOutput += vsOutputStride;
    }

    return vertexOutputStream;
}

} // namespace CommonClass

