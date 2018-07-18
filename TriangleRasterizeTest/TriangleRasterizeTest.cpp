// For Unit Test
#include <MyTools\UnitTestModules.h>
#include <iostream>
#include <MyTools\RandomToolNeedLib\MTRandom.h>
#include <MyTools\MathTool.h>
#include <limits>
#include <array>
#include <ctime>
#include <MyTools\MathTool.h>
#pragma comment(lib, "MyTools\\RandomToolNeedLib\\LibForMTRandomAndPrimeSearch.lib")

#include "../CommonClasses/DebugHelpers.h"

// include test target code
#include "../CommonClasses/vector2.h"
#include "../CommonClasses/vector3.h"
#include "../CommonClasses/hvector.h"
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
#include "../CommonClasses/Helpers.h"
#include "../CommonClasses/HPlaneEquation.h"
#include "../CommonClasses/ImageWindow.h"
#pragma comment(lib, "CommonClasses.lib")

using namespace CommonClass;


/*!
    \brief some common configurations for the test.
*/
namespace UserConfig
{
    /*!
        \brief whether let user check some output image file is right,
        if false every check about the Image will be default to be correct.
    */
    const bool LET_USER_CHECK_IMG = false;

    /*!
        \brief common image resolution on width
    */
    const Types::U32 COMMON_PIXEL_WIDTH = 512;

    /*!
        \brief common image resolution on height
    */
    const Types::U32 COMMON_PIXEL_HEIGHT = 512;

    /*!
        \brief common render option, left bound location
    */
    const Types::F32 COMMON_RENDER_LEFT = -3.0f;


    /*!
        \brief common render option, right bound location
    */
    const Types::F32 COMMON_RENDER_RIGHT = +3.0f;


    /*!
        \brief common render option, bottom bound location
    */
    const Types::F32 COMMON_RENDER_BOTTOM = -3.0f;


    /*!
        \brief common render option, right bound location
    */
    const Types::F32 COMMON_RENDER_TOP = +3.0f;

}


TEST_MODULE_START

#pragma region a test always success
TEST_UNIT_START("a test always success")

TEST_UNIT_END;
#pragma endregion

#pragma region directly draw triangle in screen space
TEST_UNIT_START("directly draw triangle in screen space")

    // skip this test due to the bug of clipping line function.
    //return 0;

    // create and set a pipline.
    Pipline pipline;

    // set a backbuffer
    pipline.SetBackBuffer(std::make_unique<RasterizeImage>(
        UserConfig::COMMON_PIXEL_WIDTH, 
        UserConfig::COMMON_PIXEL_HEIGHT, 
        RGBA::GREEN));

    // create and config pipeline state object
    auto pso = std::make_unique<PiplineStateObject>();

    pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
        return RGBA::BLACK;
    };

    // set viewport, because in the triangle rasterization, we need viewport to limit the triangle boundary.
    Viewport viewport;
    viewport.left = 0;
    viewport.right = UserConfig::COMMON_PIXEL_WIDTH - 1;
    viewport.bottom = 0;
    viewport.top = UserConfig::COMMON_PIXEL_HEIGHT - 1;
    pso->SetViewport(viewport);
    
    pipline.SetPSO(std::move(pso));

    std::array<hvector, 3> triv = {
        hvector(  2.0f, 400.0f),
        hvector(200.0f,  10.0f),
        hvector(400.0f, 300.0f)
    };

    std::array<ScreenSpaceVertexTemplate*, 3> vInScreen;
    for (size_t i = 0; i < vInScreen.size(); ++i)
    {
        vInScreen[i] = reinterpret_cast<ScreenSpaceVertexTemplate*>(&triv[i]);
    }

    pipline.DrawTriangle(vInScreen[0], vInScreen[1], vInScreen[2], sizeof(hvector));

    std::wstring pictureIndex = L"001";
    pipline.m_backBuffer->SaveTo(L"..\\RasterizeTest\\OutputTestImage\\PiplineTest\\TriangleTest\\screenSpaceTriangle_" + pictureIndex + L".png");
        
TEST_UNIT_END;
#pragma endregion

#pragma region basic sphereRayTri test
TEST_UNIT_START("basic sphereRayTri test")
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

        static_assert(sizeof(SimplePoint) == 2 * sizeof(hvector), "SimplePoint size is wrong");
        // create and config pipline state object
        auto pso = std::make_unique<PiplineStateObject>();
        pso->m_primitiveType = PrimitiveType::TRIANGLE_LIST;
        pso->m_vertexLayout.vertexShaderInputSize = sizeof(SimplePoint);
        pso->m_vertexLayout.pixelShaderInputSize  = sizeof(SimplePoint);
        
        // the pixel shader will not work
        // due to the imcompletation of the triangle pipeline.
        pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
            const Types::F32   depth      = (pVertex->m_posH.m_z + 1.0f) * 0.5f;
            const SimplePoint* pPoint     = reinterpret_cast<const SimplePoint*>(pVertex);

            const Types::U32   roundIndex = static_cast<const Types::U32>(pPoint->m_rayIndex.m_x);
            const Types::F32   lineIndex  = pPoint->m_rayIndex.m_y;
            Types::F32   red;
            if (31.9 < lineIndex && lineIndex < 32.1)
            {
                red = 1.0f;
            }
            else
            {
                red = 0.0f;
            }

            RGBA               color     (red, 0.0f, 0.0f, 1.0f);
            return color;
        };

        const Types::F32 LEFT(-1.0f), RIGHT(1.0f), BOTTOM(-1.0f), TOP(1.0f), NEAR(-0.5f), FAR(-4.0f);

        // rotate the line a little.
        Transform rotateY = Transform::RotationY(Types::Constant::PI_F / 3.0f);
        Transform rotateZ = Transform::RotationZ(Types::Constant::PI_F / 3.0f);

        Transform moveIntoScree = Transform::Translation(0.0f, 0.0f, -2.0f);

        // perspective transformation
        Transform perTrans = Transform::PerspectiveOG(LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR);
        
        Transform pushInto = Transform::Translation(0.0f, 0.0f, -2.0f);

        Transform mat = perTrans * pushInto * rotateZ * rotateY;

        Transform toView = pushInto * rotateZ * rotateY;

        Transform perspect = perTrans;

        pso->m_vertexShader = [&mat, &toView, &perspect](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pDestV)->void {
            const SimplePoint* pSrcH = reinterpret_cast<const SimplePoint*>(pSrcVertex);
            SimplePoint* pDestH = reinterpret_cast<SimplePoint*>(pDestV);

            hvector inViewPos = toView * pSrcH->m_position;

            pDestH->m_position = perspect * inViewPos;
            //pDestH->m_position = pSrcH->m_position;
            pDestH->m_rayIndex = pSrcH->m_rayIndex;
        };

        Viewport viewport;
        viewport.left = 0;
        viewport.right = UserConfig::COMMON_PIXEL_WIDTH - 1;
        viewport.bottom = 0;
        viewport.top = UserConfig::COMMON_PIXEL_HEIGHT - 1;
        pso->SetViewport(viewport);

        // create and set a pipline.
        Pipline pipline;
        pipline.SetPSO(std::move(pso));

        // set a backbuffer
        pipline.SetBackBuffer(std::make_unique<RasterizeImage>(
            UserConfig::COMMON_PIXEL_WIDTH, 
            UserConfig::COMMON_PIXEL_HEIGHT, 
            RGBA::WHITE));

        std::vector<SimplePoint> points;
        std::vector<unsigned int> indices;
        unsigned int numIndices = 0;

        // create line segments in sphere ray.
        SphereRayTri([&numIndices, &points, &indices](HELP_SPHERE_RAY_TRI_LAMBDA_PARAMETERS)->void {

            //if (lineIndex == 32)
            {
                SimplePoint p0(hvector(x0, y0, 0.0f));
                p0.m_rayIndex.m_x = roundIndex;
                p0.m_rayIndex.m_y = lineIndex;
                p0.m_rayIndex.m_z = 0;
                points.push_back(p0);
                indices.push_back(numIndices++);

                SimplePoint p1(hvector(x1, y1, 0.0f));
                p1.m_rayIndex.m_x = roundIndex;
                p1.m_rayIndex.m_y = lineIndex;
                p1.m_rayIndex.m_z = 1;
                points.push_back(p1);
                indices.push_back(numIndices++);

                SimplePoint p2(hvector(x2, y2, 0.0f));
                p2.m_rayIndex.m_x = roundIndex;
                p2.m_rayIndex.m_y = lineIndex;
                p2.m_rayIndex.m_z = 1;
                points.push_back(p2);
                indices.push_back(numIndices++);
            } // end if lineIndex
        }, 
            0.0f, 0.0f,                     // center location
            0.8f,                           // segment length
            0.5f,                           // start radius
            12                               // num rounds
            ); // radio offset

        auto vertexBuffer = std::make_unique<F32Buffer>(points.size() * sizeof(SimplePoint));
        memcpy(vertexBuffer->GetBuffer(), points.data(), vertexBuffer->GetSizeOfByte());

        {
            TIME_GUARD;
            //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
            pipline.DrawInstance(indices, vertexBuffer.get());
        }

        std::wstring pictureIndex = L"007";
        pipline.m_backBuffer->SaveTo(L"..\\RasterizeTest\\OutputTestImage\\PiplineTest\\TriangleTest\\sphereTri_" + pictureIndex + L".png");

TEST_UNIT_END;
#pragma endregion

#pragma region triangle cut test
TEST_UNIT_START("triangle cut test")

    // skip this test due to the bug of clipping line function.
    //return 0;

    // create and set a pipline.
    Pipline pipline;

    // set a backbuffer
    pipline.SetBackBuffer(std::make_unique<RasterizeImage>(
        UserConfig::COMMON_PIXEL_WIDTH, 
        UserConfig::COMMON_PIXEL_HEIGHT, 
        RGBA::GREEN));

    // create and config pipeline state object
    auto pso = std::make_unique<PiplineStateObject>();

    pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
        return RGBA::BLACK;
    };

    // set viewport, because in the triangle rasterization, we need viewport to limit the triangle boundary.
    Viewport viewport;
    viewport.left = 0;
    viewport.right = UserConfig::COMMON_PIXEL_WIDTH - 1;
    viewport.bottom = 0;
    viewport.top = UserConfig::COMMON_PIXEL_HEIGHT - 1;
    pso->SetViewport(viewport);
    
    pipline.SetPSO(std::move(pso));

    std::array<hvector, 3> triv = {
        hvector(  2.0f, 400.0f, 1.0f),
        hvector(200.0f,  10.0f, 2.0f),
        hvector(400.0f, 300.0f, -1.0f)
    };
    const size_t VERTEX_SIZE = sizeof(hvector);

    std::array<ScreenSpaceVertexTemplate*, 3> vInScreen;
    for (size_t i = 0; i < vInScreen.size(); ++i)
    {
        vInScreen[i] = reinterpret_cast<ScreenSpaceVertexTemplate*>(&triv[i]);
    }
    
    // define the plane equation to cut triangle
    class PositiveZPlane : public HPlaneEquation
    {
    public:
        Types::F32 eval(const hvector& pointH) override
        {
            return pointH.m_z;
        }

        Types::F32 cutCoefficient(const hvector& point1, const hvector& point2) override
        {
            return (0.0f - point1.m_z) / (point2.m_z - point1.m_z);
        }
    };

    PositiveZPlane posZPlane;
    TrianglePair cutResult(TrianglePair::ZERO, VERTEX_SIZE);
    {
        TIME_GUARD;
        TrianglePair cutResutlInTimer(posZPlane.CutTriangle(vInScreen[0], vInScreen[1], vInScreen[2], VERTEX_SIZE));
        cutResult = std::move(cutResutlInTimer);
    }

    switch (cutResult.m_count)
    {
    case 0:
        std::cout << "triangle reject" << std::endl;
        break;

    case 1:
    case 3:
        pipline.DrawTriangle(
            cutResult.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::ONE_TRI_1),
            cutResult.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::ONE_TRI_2),
            cutResult.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::ONE_TRI_3),
            VERTEX_SIZE);
        break;

    case 2:
        /*pipline.DrawTriangle(
            cutResult.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_1_1),
            cutResult.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_1_2),
            cutResult.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_1_3),
            VERTEX_SIZE);*/
        pipline.DrawTriangle(
            cutResult.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_2_1),
            cutResult.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_2_2),
            cutResult.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_2_3),
            VERTEX_SIZE);
        break;
    }

    std::wstring pictureIndex = L"001";
    pipline.m_backBuffer->SaveTo(L"..\\RasterizeTest\\OutputTestImage\\PiplineTest\\TriangleTest\\triangleCut_" + pictureIndex + L".png");
        
TEST_UNIT_END;
#pragma endregion

#pragma region multiple planes cut triangle
TEST_UNIT_START("multiple planes cut triangle")

    // skip this test due to the bug of clipping line function.
    //return 0;

    // create and set a pipline.
    Pipline pipline;

    // set a backbuffer
    pipline.SetBackBuffer(std::make_unique<RasterizeImage>(
        UserConfig::COMMON_PIXEL_WIDTH, 
        UserConfig::COMMON_PIXEL_HEIGHT, 
        RGBA::GREEN));

    // create and config pipeline state object
    auto pso = std::make_unique<PiplineStateObject>();

    pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
        return RGBA::BLACK;
    };

    // set viewport, because in the triangle rasterization, we need viewport to limit the triangle boundary.
    Viewport viewport;
    viewport.left = 0;
    viewport.right = UserConfig::COMMON_PIXEL_WIDTH - 1;
    viewport.bottom = 0;
    viewport.top = UserConfig::COMMON_PIXEL_HEIGHT - 1;
    pso->SetViewport(viewport);
    
    pipline.SetPSO(std::move(pso));

    std::array<hvector, 3> triv = {
        hvector(  2.0f, 400.0f, 1.0f),
        hvector(200.0f,  10.0f, 2.0f),
        hvector(400.0f, 300.0f, -1.0f)
    };
    const size_t VERTEX_SIZE = sizeof(hvector);

    std::array<ScreenSpaceVertexTemplate*, 3> vInScreen;
    for (size_t i = 0; i < vInScreen.size(); ++i)
    {
        vInScreen[i] = reinterpret_cast<ScreenSpaceVertexTemplate*>(&triv[i]);
    }
    
    // the cut plane only cut xy plane,
    // in the other words, the plane is always perpendicular to xy plane
    class XYCutPlane : public HPlaneEquation
    {
    public:
        const float m_A, m_B, m_C;
    public:
        XYCutPlane(const float A, const float B, const float C)
            :m_A(A), m_B(B), m_C(C)
        {
            // empty
        }

        Types::F32 eval(const hvector& pointH) override
        {
            return m_A * pointH.m_x + m_B * pointH.m_y + m_C;
        }

        Types::F32 cutCoefficient(const hvector& point1, const hvector& point2) override
        {
            float x1(point1.m_x), y1(point1.m_y), x2(point2.m_x), y2(point2.m_y);
            float numerator = - (m_A * x1 + m_B * y1 + m_C);
            float denominator = m_A * (x2 - x1) + m_B * (y2 - y1);
            return numerator / denominator;
        }
    };

    std::vector<std::unique_ptr<HPlaneEquation>> cutPlanes;
    cutPlanes.push_back(std::make_unique<XYCutPlane>( 248.0f,  -57.0f,   -3893.0f));
    cutPlanes.push_back(std::make_unique<XYCutPlane>( 247.0f,  248.0f,  -67192.0f));
    cutPlanes.push_back(std::make_unique<XYCutPlane>(-185.0f,  293.0f,   14290.0f));
    cutPlanes.push_back(std::make_unique<XYCutPlane>(-282.0f, -353.0f,  151912.0f));

    std::vector<TrianglePair> cutResults;
    {
        TIME_GUARD;
        pipline.FrustumCutTriangle(vInScreen[0], vInScreen[1], vInScreen[2], VERTEX_SIZE,
            &cutResults, cutPlanes);
    }

    // lambda function to draw one TrianglePair.
    auto drawCutResult = [&pipline, &VERTEX_SIZE](TrianglePair& tp)->void {
        switch (tp.m_count)
        {
        case 0:
            std::cout << "triangle reject" << std::endl;
            break;// case 0

        case 1:
        case 3:
            pipline.DrawTriangle(
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::ONE_TRI_1),
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::ONE_TRI_2),
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::ONE_TRI_3),
                VERTEX_SIZE);
            break;// case 1 and 3

        case 2:
            pipline.DrawTriangle(
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_1_1),
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_1_2),
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_1_3),
                VERTEX_SIZE);
            pipline.DrawTriangle(
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_2_1),
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_2_2),
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_2_3),
                VERTEX_SIZE);
            break;// case 2
        }// end switch tp.m_count
    };// end lambda drawCutResult

    //drawCutResult(cutResults[5]);

    for (auto & result : cutResults)
    {
        drawCutResult(result);
    }

    std::wstring pictureIndex = L"005";
    pipline.m_backBuffer->SaveTo(L"..\\RasterizeTest\\OutputTestImage\\PiplineTest\\TriangleTest\\triangleMultipleCut_" + pictureIndex + L".png");
        
TEST_UNIT_END;
#pragma endregion

#pragma region abstract frustrum plane test
TEST_UNIT_START("abstract frustrum plane test")

    // skip this test due to the bug of clipping line function.
    //return 0;

    // create and set a pipline.
    Pipline pipline;

    // set a backbuffer
    pipline.SetBackBuffer(std::make_unique<RasterizeImage>(
        UserConfig::COMMON_PIXEL_WIDTH, 
        UserConfig::COMMON_PIXEL_HEIGHT, 
        RGBA::GREEN));

    // create and config pipeline state object
    auto pso = std::make_unique<PiplineStateObject>();

    pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
        return RGBA::BLACK;
    };

    // set viewport, because in the triangle rasterization, we need viewport to limit the triangle boundary.
    Viewport viewport;
    viewport.left = 0;
    viewport.right = UserConfig::COMMON_PIXEL_WIDTH - 1;
    viewport.bottom = 0;
    viewport.top = UserConfig::COMMON_PIXEL_HEIGHT - 1;
    pso->SetViewport(viewport);
    
    pipline.SetPSO(std::move(pso));

    std::array<hvector, 3> triv = {
        hvector(  2.0f, 400.0f,  2.0f, 1.0f),
        hvector(200.0f,  10.0f,  1.0f, 1.0f),
        hvector(400.0f, 300.0f, -3.0f, -1.0f)
    };
    const size_t VERTEX_SIZE = sizeof(hvector);

    std::array<ScreenSpaceVertexTemplate*, 3> vInScreen;
    for (size_t i = 0; i < vInScreen.size(); ++i)
    {
        vInScreen[i] = reinterpret_cast<ScreenSpaceVertexTemplate*>(&triv[i]);
    }

    //using PlaneType = FrustumHPlaneEquation<2, false>;

    using PlaneType = WZeroHPlaneEquation;

    PlaneType cutPlane;
    
    TrianglePair cutResult(TrianglePair::ZERO, VERTEX_SIZE);

    std::vector<TrianglePair> cutResults;
    {
        TIME_GUARD;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        cutResult = cutPlane.CutTriangle(vInScreen[0], vInScreen[1], vInScreen[2], VERTEX_SIZE);
    }

    // lambda function to draw one TrianglePair.
    auto drawCutResult = [&pipline, &VERTEX_SIZE](TrianglePair& tp)->void {
        switch (tp.m_count)
        {
        case 0:
            std::cout << "triangle reject" << std::endl;
            break;// case 0

        case 1:
        case 3:
            pipline.DrawTriangle(
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::ONE_TRI_1),
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::ONE_TRI_2),
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::ONE_TRI_3),
                VERTEX_SIZE);
            break;// case 1 and 3

        case 2:
            pipline.DrawTriangle(
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_1_1),
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_1_2),
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_1_3),
                VERTEX_SIZE);
            pipline.DrawTriangle(
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_2_1),
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_2_2),
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_2_3),
                VERTEX_SIZE);
            break;// case 2
        }// end switch tp.m_count
    };// end lambda drawCutResult

    //drawCutResult(cutResults[5]);

    drawCutResult(cutResult);

    std::wstring pictureIndex = L"001";
    pipline.m_backBuffer->SaveTo(L"..\\RasterizeTest\\OutputTestImage\\PiplineTest\\TriangleTest\\frustumPlaneCut_" + pictureIndex + L".png");
        
TEST_UNIT_END;
#pragma endregion

#pragma region basic sphereRayTri test
TEST_UNIT_START("basic sphereRayTri test")
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

    static_assert(sizeof(SimplePoint) == 2 * sizeof(hvector), "SimplePoint size is wrong");
    // create and config pipline state object
    auto pso = std::make_shared<PiplineStateObject>();
    pso->m_primitiveType = PrimitiveType::TRIANGLE_LIST;
    pso->m_vertexLayout.vertexShaderInputSize = sizeof(SimplePoint);
    pso->m_vertexLayout.pixelShaderInputSize  = sizeof(SimplePoint);
        
    // the pixel shader will not work
    // due to the imcompletation of the triangle pipeline.
    pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
        const Types::F32   depth      = (pVertex->m_posH.m_z + 1.0f) * 0.5f;
        const SimplePoint* pPoint     = reinterpret_cast<const SimplePoint*>(pVertex);

        const Types::U32   roundIndex = static_cast<const Types::U32>(pPoint->m_rayIndex.m_x);
        const Types::F32   lineIndex  = pPoint->m_rayIndex.m_y;
        Types::F32   red;
        if (31.9 < lineIndex && lineIndex < 32.1)
        {
            red = 1.0f;
        }
        else
        {
            red = 0.0f;
        }

        RGBA               color     (red, 0.0f, 0.0f, 1.0f);
        return color;
    };

    const Types::F32 LEFT(-1.0f), RIGHT(1.0f), BOTTOM(-1.0f), TOP(1.0f), NEAR(-0.5f), FAR(-4.0f);

    // rotate the line a little.
    Transform rotateY = Transform::RotationY(Types::Constant::PI_F / 3.0f);
    Transform rotateZ = Transform::RotationZ(Types::Constant::PI_F / 3.0f);

    Transform moveIntoScree = Transform::Translation(0.0f, 0.0f, -2.0f);

    // perspective transformation
    Transform perTrans = Transform::PerspectiveOG(LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR);
        
    Transform pushInto = Transform::Translation(0.0f, 0.0f, -2.0f);

    Transform mat = perTrans * pushInto * rotateZ * rotateY;

    Transform toView = pushInto * rotateZ * rotateY;

    Transform perspect = perTrans;

    pso->m_vertexShader = [&mat, &toView, &perspect](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pDestV)->void {
        const SimplePoint* pSrcH = reinterpret_cast<const SimplePoint*>(pSrcVertex);
        SimplePoint* pDestH = reinterpret_cast<SimplePoint*>(pDestV);

        hvector inViewPos = toView * pSrcH->m_position;

        pDestH->m_position = perspect * inViewPos;
        //pDestH->m_position = pSrcH->m_position;
        pDestH->m_rayIndex = pSrcH->m_rayIndex;
    };

    Viewport viewport;
    viewport.left = 0;
    viewport.right = UserConfig::COMMON_PIXEL_WIDTH - 1;
    viewport.bottom = 0;
    viewport.top = UserConfig::COMMON_PIXEL_HEIGHT - 1;
    pso->SetViewport(viewport);

    // create and set a pipline.
    Pipline pipline;
    pipline.SetPSO(pso);

    // set a backbuffer
    pipline.SetBackBuffer(std::make_unique<RasterizeImage>(
        UserConfig::COMMON_PIXEL_WIDTH, 
        UserConfig::COMMON_PIXEL_HEIGHT, 
        RGBA::WHITE));

    std::vector<SimplePoint> points;
    std::vector<unsigned int> indices;
    unsigned int numIndices = 0;

    // create line segments in sphere ray.
    SphereRayTri([&numIndices, &points, &indices](HELP_SPHERE_RAY_TRI_LAMBDA_PARAMETERS)->void {

        //if (lineIndex == 32)
        {
            SimplePoint p0(hvector(x0, y0, 0.0f));
            p0.m_rayIndex.m_x = roundIndex;
            p0.m_rayIndex.m_y = lineIndex;
            p0.m_rayIndex.m_z = 0;
            points.push_back(p0);
            indices.push_back(numIndices++);

            SimplePoint p1(hvector(x1, y1, 0.0f));
            p1.m_rayIndex.m_x = roundIndex;
            p1.m_rayIndex.m_y = lineIndex;
            p1.m_rayIndex.m_z = 1;
            points.push_back(p1);
            indices.push_back(numIndices++);

            SimplePoint p2(hvector(x2, y2, 0.0f));
            p2.m_rayIndex.m_x = roundIndex;
            p2.m_rayIndex.m_y = lineIndex;
            p2.m_rayIndex.m_z = 1;
            points.push_back(p2);
            indices.push_back(numIndices++);
        } // end if lineIndex
    }, 
        0.0f, 0.0f,                     // center location
        0.8f,                           // segment length
        0.5f,                           // start radius
        12                               // num rounds
        ); // radio offset

    auto vertexBuffer = std::make_unique<F32Buffer>(points.size() * sizeof(SimplePoint));
    memcpy(vertexBuffer->GetBuffer(), points.data(), vertexBuffer->GetSizeOfByte());

    {
        TIME_GUARD;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline.DrawInstance(indices, vertexBuffer.get());
    }


    std::wstring pictureIndex = L"001";
    std::wstring pictureName = L"z - Buffer_" + pictureIndex + L".png";

    ImageWindow imgWnd(pipline.m_backBuffer.get(), pictureName);
    imgWnd.BlockShow();

    pipline.m_backBuffer->SaveTo(L"..\\RasterizeTest\\OutputTestImage\\PiplineTest\\z-Buffer\\" + pictureName);

TEST_UNIT_END;
#pragma endregion

TEST_MODULE_END
