#include <MyTools/TestTool/Suit.h>  // include test framework

#include "stdafx.h"
#include "../CommonClasses/vector2.h"
#include "../CommonClasses/vector3.h"
#include "../CommonClasses/Image.h"
#include "../CommonClasses/OrthographicCamera.h"
#include "../CommonClasses/Ray.h"
#include "../CommonClasses/Sphere.h"
#include "../CommonClasses/PerspectiveCamera.h"
#include "../CommonClasses/Triangle.h"
#include "../CommonClasses/Scene.h"
#include "../CommonClasses/Polygon.h"
#include "../CommonClasses/ColorTemplate.h"
#include "../CommonClasses/Light.h"
#include "../CommonClasses/Material.h"
#include "../CommonClasses/RasterizeImage.h"
#include "../CommonClasses/Pipline.h"
#include "../CommonClasses/CoordinateFrame.h"
#include "../CommonClasses/DebugConfigs.h"
#include "../CommonClasses/ImageWindow.h"
#include "../CommonClasses/Helpers.h"
#pragma comment(lib, "CommonClasses.lib")

namespace UserConfig
{
    
}



using namespace CommonClass;

/*!
    \brief the common environment for pipline, for example: some useful pixel shader, pipline stateObject.
*/
class CommonEnvironment
{
public:
    // temp struct for line drawing.
    struct SimplePoint
    {
    public:
        hvector m_position;
        /*!
        \brief used to store the sphere ray location information
        x - round index
        y - line index in the round
        z = 0 means it's start point
        z = 1 means it's end point
        */
        hvector m_rayIndex;
        explicit SimplePoint(const hvector& pos)
            :m_position(pos)
        {
            // empty
        }
    };

public:
    // membbers
    int COMMON_PIXEL_WIDTH = 512;
    int COMMON_PIXEL_HEIGHT = 512;

public:
    CommonEnvironment()
    {
        // empty
    }

    /*!
        \brief Build common PiplineStateObject for using.
    */
    std::unique_ptr<PiplineStateObject> GetCommonPSO()
    {
        auto pso = std::make_unique<PiplineStateObject>();
        pso->m_primitiveType = PrimitiveType::LINE_LIST;
        pso->m_vertexLayout.vertexShaderInputSize = sizeof(SimplePoint);
        pso->m_vertexLayout.pixelShaderInputSize = sizeof(SimplePoint);
        pso->SetViewport(GetCommonViewport());

        return std::move(pso);
    }

    /*!
        \brief Build common viewport by the default view size defined in CommonEnvironment.
    */
    Viewport GetCommonViewport()
    {
        Viewport viewport;
        viewport.left = 0;
        viewport.right = COMMON_PIXEL_WIDTH - 1;
        viewport.bottom = 0;
        viewport.top = COMMON_PIXEL_HEIGHT - 1;
        return viewport;
    }

    /*!
        \brief Build common Pipline for drawing, 
        all you need do is to set PixelShader/VertexShader, 
        and call drawing function with mesh data.
    */
    std::unique_ptr<Pipline> GetCommonPipline()
    {
        // create and set a pipline.
        auto pipline = std::make_unique<Pipline>();
        pipline->SetPSO(std::move(GetCommonPSO()));

        // set a backbuffer
        pipline->SetBackBuffer(std::make_unique<RasterizeImage>(
            COMMON_PIXEL_WIDTH,
            COMMON_PIXEL_HEIGHT,
            RGBA::WHITE));

        return std::move(pipline);
    }
};

class TriangleTransformCase : public TestSuit::Case
{
public:
    using SimplePoint = CommonEnvironment::SimplePoint;
    static_assert(sizeof(SimplePoint) == 2 * sizeof(hvector), "The size of SimplePoint is not matched for this case.");

private:
    CommonEnvironment * pEnvironment;

public:
    TriangleTransformCase() : Case("triangle transform case") {}

    virtual void SetEnvironment(void * pEnvironment) override
    {
        this->pEnvironment = reinterpret_cast<CommonEnvironment*>(pEnvironment);
    }

    /*!
        \brief fill in the mesh data with sphere ray line list.
        \param outVetexData return vertex data
        \param outIndices return indices in line list.
    */
    void BuildSphereRayLineData(std::vector<SimplePoint>& outVertexData, std::vector<unsigned int>& outIndices)
    {
        std::vector<SimplePoint> points;
        std::vector<unsigned int> indices;
        unsigned int numIndices = 0;

        const float CENTER_X = 0.0f;
        const float CENTER_Y = 0.0f;
        const float SEGMENT_LENGTH = 0.3f;
        const float START_RADIUS = 0.3;
        const float NUM_ROUNDS = 12;
        const float RADIO_OFFSET = 0.0f; // Add this offset to every angle inside the sphereRay generating process.

                                         // create line segments in sphere ray.
        SphereRay([&numIndices, &outVertexData, &outIndices](HELP_SPHERE_RAY_LAMBDA_PARAMETERS)->void {

            const unsigned int theIndexOfOneLine = 64;
            // only get one line for convenience of debugging
            //if (lineIndex == theIndexOfOneLine)
            //{
            // add start vertex and its index
            SimplePoint start(hvector(x0, y0, 0.0f));
            start.m_rayIndex.m_x = roundIndex;
            start.m_rayIndex.m_y = lineIndex;
            start.m_rayIndex.m_z = 0;
            outVertexData.push_back(start);
            outIndices.push_back(numIndices++);

            // add end vertex and its index
            SimplePoint end(hvector(x1, y1, 0.0f));
            end.m_rayIndex.m_x = roundIndex;
            end.m_rayIndex.m_y = lineIndex;
            end.m_rayIndex.m_z = 1;
            outVertexData.push_back(end);
            outIndices.push_back(numIndices++);
            //}
        },
            CENTER_X, CENTER_Y, // center location
            SEGMENT_LENGTH,     // segment length
            START_RADIUS,       // start radius
            NUM_ROUNDS,         // num rounds
            RADIO_OFFSET        // radio offset
            );// end calling SphereRay()
    }// end BuildSphereRayTriangleMeshData()

    virtual void Run()
    {
        using namespace CommonClass;

        assert(pEnvironment);
        auto pipline = pEnvironment->   GetCommonPipline();
        auto pso     = pipline->        GetPSO();

        const Types::F32 LEFT(-1.0f), RIGHT(1.0f), BOTTOM(-1.0f), TOP(1.0f), NEAR(-0.5f), FAR(-4.0f);

        // world to camera space
        Transform pushInto = Transform::Translation(0.0f, 0.0f, -2.0f);
        Transform rotateY = Transform::RotationY(Types::Constant::PI_F / 3.0f);
        Transform rotateZ = Transform::RotationZ(Types::Constant::PI_F / 3.0f);
        Transform toView = pushInto * rotateZ * rotateY;

        // perspective tranformation
        Transform perTrans = Transform::PerspectiveOG(LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR);
        Transform perspect = perTrans;

        // single matrix from world space to NDC space. 
        // This matrix may not be used currently for debuging.
        Transform toNDC = perTrans * toView;

        pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
            const Types::F32   depth = (pVertex->m_posH.m_z + 1.0f) * 0.5f;
            const SimplePoint* pPoint = reinterpret_cast<const SimplePoint*>(pVertex);
            const Types::F32   red = pPoint->m_rayIndex.m_z;

            const Types::U32   roundIndex = static_cast<const Types::U32>(pPoint->m_rayIndex.m_x);
            const Types::U32   lineIndex = static_cast<const Types::U32>(pPoint->m_rayIndex.m_y);
            //const Types::F32   isTheOne   = 22 < lineIndex && lineIndex < 34 ? 1.0f : 0.0f;
            const Types::F32   isTheOne = roundIndex == 7 && 34 <= lineIndex && lineIndex <= 34 ? 1.0f : 0.0f;

            RGBA               color(isTheOne, 0.0f, 0.0f, 1.0f);
            return color;
        };

        pso->m_vertexShader = [&toView, &perspect](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pDestV)->void {
            const SimplePoint* pSrcH = reinterpret_cast<const SimplePoint*>(pSrcVertex);
            SimplePoint* pDestH = reinterpret_cast<SimplePoint*>(pDestV);

            //DebugClient<DEBUG_CLIENT_CONF_LINE_CLIP_ERROR_ANALYSIS>(pSrcH->m_rayIndex.m_x == 7.0f && pSrcH->m_rayIndex.m_y == 34.0f);

            // to camera space
            hvector inViewPos = toView * pSrcH->m_position;

            // perspective transform
            pDestH->m_position = perspect * inViewPos;

            // additional information about the ray index for Debuging.
            pDestH->m_rayIndex = pSrcH->m_rayIndex;
        };

        std::vector<SimplePoint> points;
        std::vector<unsigned int> indices;
        BuildSphereRayLineData(points, indices);

        auto vertexBuffer = std::make_unique<F32Buffer>(points.size() * sizeof(SimplePoint));
        memcpy(vertexBuffer->GetBuffer(), points.data(), vertexBuffer->GetSizeOfByte());

        {
            COUNT_DETAIL_TIME;
            DebugGuard<DEBUG_CLIENT_CONF_LINE_CLIP_ERROR_ANALYSIS> guard;
            pipline->DrawInstance(indices, vertexBuffer.get());
        }

        std::wstring pictureIndex = L"final";
        std::wstring pictureName = L"lineClippingErrAnalysis_fixed_" + pictureIndex + L".png";
        ImageWindow imgWnd(pipline->m_backBuffer.get(), pictureName);
        imgWnd.BlockShow();

        pipline->m_backBuffer->SaveTo(L"..\\OutputTestImage\\PiplineTest\\" + pictureName);
    }
};

class RasterizeSuit : public TestSuit::Suit<TriangleTransformCase>
{
public:
    virtual void * PrepareBeforeEachCase(TestSuit::Case * pTheCase) override
    {
        CommonEnvironment * pNewEnv = new CommonEnvironment;
        return pNewEnv;
    }

    virtual void FinishEachCase(TestSuit::Case * pTheCase, void * pEnvironment) override
    {
        delete reinterpret_cast<CommonEnvironment*>(pEnvironment);
    }

};

int main()
{
    RasterizeSuit firstSuit;

    firstSuit.Start();

    getchar();
    return 0;
}

