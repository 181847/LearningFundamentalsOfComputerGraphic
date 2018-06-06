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
#pragma comment(lib, "CommonClasses.lib")

RandomTool::MTRandom globalMtr;
const unsigned int G_MAX_INT = 1000;

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
	const Types::U32 COMMON_PIXEL_WIDTH     = 512;
	
	/*!
		\brief common image resolution on height
	*/
	const Types::U32 COMMON_PIXEL_HEIGHT    = 512;
	
	/*!
		\brief common render option, left bound location
	*/
	const Types::F32 COMMON_RENDER_LEFT     = -3.0f;

	
	/*!
		\brief common render option, right bound location
	*/
	const Types::F32 COMMON_RENDER_RIGHT    = +3.0f;

	
	/*!
		\brief common render option, bottom bound location
	*/
	const Types::F32 COMMON_RENDER_BOTTOM   = -3.0f;

	
	/*!
		\brief common render option, right bound location
	*/
	const Types::F32 COMMON_RENDER_TOP      = +3.0f;

    /*!
        \brief for some draw line tests, the loop time
    */
    const unsigned int LOOP_DRAW_LINE_COUNT = 1;

    /*!
        \brief for some loop test, between several(the value) test output one message.
    */
    const unsigned int PRINT_MSG_DIST = 1;
}


/*!
	\brief get random vector3.
	if you want to set the seed, try globalMtr.SetRandomSeed(...);
*/
CommonClass::vector3 GetRandomVector3(bool allowZeroVector = true)
{
	CommonClass::vector3 randVec;
	do
	{
		randVec = CommonClass::vector3((globalMtr.Random() - 0.5f) * (globalMtr.Random(G_MAX_INT) + 1),
			(globalMtr.Random() - 0.5f) * (globalMtr.Random(G_MAX_INT) + 1),
			(globalMtr.Random() - 0.5f) * (globalMtr.Random(G_MAX_INT) + 1));

		// if allowZeroVector is false, loop until a none zero vector
	} while (! allowZeroVector && randVec.m_x == 0.0f && randVec.m_y== 0.0f && randVec.m_z == 0.0f);
	

	return randVec;
}

#define HELP_SPHERE_RAY_LAMBDA_PARAMETERS const Types::F32 x0, const Types::F32 y0, const Types::F32 x1, const Types::F32 y1, const unsigned int roundIndex, const unsigned int lineIndex

TEST_MODULE_START

#pragma region check module work
    TEST_UNIT_START("check module work")
        // empty
        // always success
    TEST_UNIT_END;
#pragma endregion

#pragma region save rasterize image
    TEST_UNIT_START("save rasterize image")
        RasterizeImage defaultImg   (UserConfig::COMMON_PIXEL_WIDTH, UserConfig::COMMON_PIXEL_HEIGHT);
        RasterizeImage redImg       (UserConfig::COMMON_PIXEL_WIDTH, UserConfig::COMMON_PIXEL_HEIGHT, RGBA::RED);
        RasterizeImage greenImg     (UserConfig::COMMON_PIXEL_WIDTH, UserConfig::COMMON_PIXEL_HEIGHT, RGBA::GREEN);
        RasterizeImage blueImg      (UserConfig::COMMON_PIXEL_WIDTH, UserConfig::COMMON_PIXEL_HEIGHT, RGBA::BLUE);
        defaultImg. SaveTo(".\\OutputTestImage\\BaseRasterizeImage\\default_color.png");
        redImg.     SaveTo(".\\OutputTestImage\\BaseRasterizeImage\\red_color.png");
        greenImg.   SaveTo(".\\OutputTestImage\\BaseRasterizeImage\\green_color.png");
        blueImg.    SaveTo(".\\OutputTestImage\\BaseRasterizeImage\\blue_color.png");
    TEST_UNIT_END;
#pragma endregion

#pragma region draw WuXiaolin lines
    TEST_UNIT_START("draw WuXiaolin lines")
        RasterizeImage defaultImg(UserConfig::COMMON_PIXEL_WIDTH, UserConfig::COMMON_PIXEL_HEIGHT, RGBA::WHITE);

        const Types::F32 CENTER_X = defaultImg.m_width / 2.0f,
                         CENTER_Y = defaultImg.m_height / 2.0f;

        {
            TIME_GUARD;

            SphereRay([&defaultImg](HELP_SPHERE_RAY_LAMBDA_PARAMETERS) -> void {
                defaultImg.DrawWuXiaolinLine(x0, y0, x1, y1);
            }, CENTER_X, CENTER_Y);
        }// end time counter
        
        defaultImg.SaveTo(".\\OutputTestImage\\DrawWuXiaolinLine\\sphere_ray.png");

    TEST_UNIT_END;
#pragma endregion

#pragma region draw bresenham lines
    TEST_UNIT_START("draw bresenham lines")
        testConfig.m_loopTime = UserConfig::LOOP_DRAW_LINE_COUNT;

        if (testParameter.m_runningIndex % UserConfig::PRINT_MSG_DIST == 0)
        {
            testConfig.m_hideThisOutput = false;
        }
        else
        {
            testConfig.m_hideThisOutput = true;
            ShowProgress(testParameter.m_runningIndex * 1.0f / testConfig.m_loopTime);
        }

        RasterizeImage defaultImg(UserConfig::COMMON_PIXEL_WIDTH, UserConfig::COMMON_PIXEL_HEIGHT, RGBA::WHITE);
        testConfig.m_testName += ("--index: " + std::to_string(testParameter.m_runningIndex));

        const Types::F32 CENTER_X = defaultImg.m_width / 2.0f,
                         CENTER_Y = defaultImg.m_height / 2.0f;

        {
            TIME_GUARD;

            SphereRay([&defaultImg](HELP_SPHERE_RAY_LAMBDA_PARAMETERS) -> void{
                defaultImg.DrawBresenhamLine(

                    static_cast<Types::U32>(x0),
                    static_cast<Types::U32>(y0),
                    static_cast<Types::U32>(x1),
                    static_cast<Types::U32>(y1));
            }, CENTER_X, CENTER_Y);
        }// end time counter
        
        
        defaultImg.SaveTo(".\\OutputTestImage\\DrawBresenhamLine\\sphere_ray.png");

    TEST_UNIT_END;
#pragma endregion

#pragma region draw bresenham lines
    TEST_UNIT_START("draw bresenham lines in one call")
        testConfig.m_loopTime = UserConfig::LOOP_DRAW_LINE_COUNT;

        if (testParameter.m_runningIndex % UserConfig::PRINT_MSG_DIST == 0)
        {
            testConfig.m_hideThisOutput = false;
        }
        else
        {
            testConfig.m_hideThisOutput = true;
            ShowProgress(testParameter.m_runningIndex * 1.0f / testConfig.m_loopTime);
        }

        RasterizeImage defaultImg(UserConfig::COMMON_PIXEL_WIDTH, UserConfig::COMMON_PIXEL_HEIGHT, RGBA::WHITE);
        testConfig.m_testName += ("--index: " + std::to_string(testParameter.m_runningIndex));

        const Types::F32 CENTER_X = defaultImg.m_width / 2.0f,
                         CENTER_Y = defaultImg.m_height / 2.0f;

        {
            TIME_GUARD;

            SphereRay([&defaultImg](HELP_SPHERE_RAY_LAMBDA_PARAMETERS) -> void {
                defaultImg.DrawBresenhamLine_inOneCall(
                    static_cast<Types::U32>(x0),
                    static_cast<Types::U32>(y0),
                    static_cast<Types::U32>(x1),
                    static_cast<Types::U32>(y1));
            }, CENTER_X, CENTER_Y);
        }// end time counter
        
        defaultImg.SaveTo(".\\OutputTestImage\\DrawBresenhamLine\\default_color_in_one_call_sphere_ray.png");

    TEST_UNIT_END;
#pragma endregion

#pragma region draw bresenham lines retry
    TEST_UNIT_START("draw bresenham lines retry")
        testConfig.m_loopTime = UserConfig::LOOP_DRAW_LINE_COUNT;

        if (testParameter.m_runningIndex % UserConfig::PRINT_MSG_DIST == 0)
        {
            testConfig.m_hideThisOutput = false;
        }
        else
        {
            testConfig.m_hideThisOutput = true;
            ShowProgress(testParameter.m_runningIndex * 1.0f / testConfig.m_loopTime);
        }

        RasterizeImage defaultImg(UserConfig::COMMON_PIXEL_WIDTH, UserConfig::COMMON_PIXEL_HEIGHT, RGBA::WHITE);
        testConfig.m_testName += ("--index: " + std::to_string(testParameter.m_runningIndex));

        const Types::F32 CENTER_X = defaultImg.m_width / 2.0f,
                         CENTER_Y = defaultImg.m_height / 2.0f;

        {
            TIME_GUARD;

            SphereRay([&defaultImg](HELP_SPHERE_RAY_LAMBDA_PARAMETERS) -> void{
                defaultImg.DrawBresenhamLine(

                    static_cast<Types::U32>(x0),
                    static_cast<Types::U32>(y0),
                    static_cast<Types::U32>(x1),
                    static_cast<Types::U32>(y1));
            }, CENTER_X, CENTER_Y);
        }// end time counter
        
        
        defaultImg.SaveTo(".\\OutputTestImage\\DrawBresenhamLine\\sphere_ray.png");

    TEST_UNIT_END;
#pragma endregion

#pragma region draw bresenham lines retry
    TEST_UNIT_START("draw bresenham lines in one call retry")
        testConfig.m_loopTime = UserConfig::LOOP_DRAW_LINE_COUNT;

        if (testParameter.m_runningIndex % UserConfig::PRINT_MSG_DIST == 0)
        {
            testConfig.m_hideThisOutput = false;
        }
        else
        {
            testConfig.m_hideThisOutput = true;
            ShowProgress(testParameter.m_runningIndex * 1.0f / testConfig.m_loopTime);
        }

        RasterizeImage defaultImg(UserConfig::COMMON_PIXEL_WIDTH, UserConfig::COMMON_PIXEL_HEIGHT, RGBA::WHITE);
        testConfig.m_testName += ("--index: " + std::to_string(testParameter.m_runningIndex));

        const Types::F32 CENTER_X = defaultImg.m_width / 2.0f,
                         CENTER_Y = defaultImg.m_height / 2.0f;

        {
            TIME_GUARD;

            SphereRay([&defaultImg](HELP_SPHERE_RAY_LAMBDA_PARAMETERS) -> void {
                defaultImg.DrawBresenhamLine_inOneCall(
                    static_cast<Types::U32>(x0),
                    static_cast<Types::U32>(y0),
                    static_cast<Types::U32>(x1),
                    static_cast<Types::U32>(y1));
            }, CENTER_X, CENTER_Y);
        }// end time counter
        
        defaultImg.SaveTo(".\\OutputTestImage\\DrawBresenhamLine\\default_color_in_one_call_sphere_ray.png");

    TEST_UNIT_END;
#pragma endregion

#pragma region 2d line clipping
    TEST_UNIT_START(" 2d line clipping")
        testConfig.m_loopTime = UserConfig::LOOP_DRAW_LINE_COUNT;

        RasterizeImage defaultImg(UserConfig::COMMON_PIXEL_WIDTH, UserConfig::COMMON_PIXEL_HEIGHT, RGBA::WHITE);

        const Types::F32 CENTER_X = 0.5,
                         CENTER_Y = 0.5;

        {
            TIME_GUARD;

            defaultImg.SetScissor(ScissorRect(  0, 300, 300, 511));

            SphereRay([&defaultImg](HELP_SPHERE_RAY_LAMBDA_PARAMETERS) -> void {
                defaultImg.DrawLine(x0, y0, x1, y1, RGB::GREEN);
            }, CENTER_X, CENTER_Y, 0.1f, 0.1f, 6);


            defaultImg.SetScissor(ScissorRect(300, 511,   0, 511));

            SphereRay([&defaultImg](HELP_SPHERE_RAY_LAMBDA_PARAMETERS) -> void {
                defaultImg.DrawLine(x0, y0, x1, y1, RGB::RED);
            }, CENTER_X, CENTER_Y, 0.3f, 0.2f, 6);


            defaultImg.SetScissor(ScissorRect(  0, 300,   0, 300));

            SphereRay([&defaultImg](HELP_SPHERE_RAY_LAMBDA_PARAMETERS) -> void {
                defaultImg.DrawLine(x0, y0, x1, y1, RGB::BLUE);
            }, CENTER_X, CENTER_Y, 0.05f, 0.1f, 8);
        }// end time counter
        
        defaultImg.SaveTo(".\\OutputTestImage\\DrawBresenhamLine\\2d_line_clipping.png");

    TEST_UNIT_END;
#pragma endregion

#pragma region draw triangle flat bottom
    TEST_UNIT_START("draw triangle flat bottom")
        RasterizeImage defaultImg(UserConfig::COMMON_PIXEL_WIDTH, UserConfig::COMMON_PIXEL_HEIGHT, RGBA::WHITE);
        
        const Types::F32 CENTER_X = defaultImg.m_width / 2.0f,
                         CENTER_Y = defaultImg.m_height / 2.0f;

        {
            TIME_GUARD;

            defaultImg.DrawTri_flatBottom(
                256.0f, 500.0f,
                100.0f, 300.0f,
                300.0f, 300.0f,
                RGB::BLUE);

            defaultImg.DrawTri_flatTop(
                256.0f, 100.0f,
                100.0f, 300.0f,
                300.0f, 300.0f,
                RGB::RED);

        }// end time counter
        
        defaultImg.SaveTo(".\\OutputTestImage\\DrawBresenhamLine\\triangleFlatBottom.png");

    TEST_UNIT_END;
#pragma endregion

#pragma region draw triangle general tri
    TEST_UNIT_START("draw triangle general tri")
        RasterizeImage defaultImg(UserConfig::COMMON_PIXEL_WIDTH, UserConfig::COMMON_PIXEL_HEIGHT, RGBA::WHITE);
        
        const Types::F32 CENTER_X = defaultImg.m_width / 2.0f,
                         CENTER_Y = defaultImg.m_height / 2.0f;

        {
            TIME_GUARD;

            defaultImg.DrawTriangle(
                256.0f, 500.0f,
                100.0f, 300.0f,
                300.0f, 100.0f,
                RGB::BLUE);

        }// end time counter
        
        defaultImg.SaveTo(".\\OutputTestImage\\DrawBresenhamLine\\generalTriangle.png");

    TEST_UNIT_END;
#pragma endregion

#pragma region draw line with pipline
    TEST_UNIT_START("draw line with pipline")
        
        // skip this test due to the bug of clipping line function.
        //return 0;
        // temp struct for line drawing.
        struct SimplePoint
        {
        public:
            hvector m_position;
            SimplePoint(const hvector& pos)
                :m_position(pos)
            {
                // empty
            }
        };
        static_assert(sizeof(SimplePoint) == 16, "size of SimplePoint is wrong");

        // create and config pipline state object
        auto pso = std::make_unique<PiplineStateObject>();
        pso->m_primitiveType = PrimitiveType::LINE_LIST;
        pso->m_vertexLayout.vertexShaderInputSize = sizeof(hvector);
        pso->m_vertexLayout.pixelShaderInputSize  = sizeof(hvector);
        
        pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {

            RGBA color;
            color.m_chas.m_r = pVertex->m_posH.m_x * 1.0f / 512;
            color.m_chas.m_g = pVertex->m_posH.m_y * 1.0f / 512;

            return color;
        };

        pso->m_vertexShader = [](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pdestV)->void {
            memcpy(pdestV, pSrcVertex, sizeof(SimplePoint));
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
        SphereRay([&numIndices, &points, &indices](HELP_SPHERE_RAY_LAMBDA_PARAMETERS)->void {

            // add start vertex and its index
            points.push_back(SimplePoint(hvector(x0, y0)));
            indices.push_back(numIndices++);

            // add end vertex and its index
            points.push_back(SimplePoint(hvector(x1, y1)));
            indices.push_back(numIndices++);
        }, 
            0.0f, 0.0f, // center location
            0.2f,       // segment length
            0.1f,       // start radius
            4);         // num rounds

        auto vertexBuffer = std::make_unique<F32Buffer>(points.size() * 4 * sizeof(Types::F32));
        memcpy(vertexBuffer->GetBuffer(), points.data(), vertexBuffer->GetSizeOfByte());

        {
            TIME_GUARD;
            pipline.DrawInstance(indices, vertexBuffer.get());
        }

        pipline.m_backBuffer->SaveTo(".\\OutputTestImage\\PiplineTest\\drawLines.png");
        
    TEST_UNIT_END;
#pragma endregion

#pragma region line clipping in pipline
    TEST_UNIT_START("line clipping in pipline")

        // skip this test due to the bug of clipping line function.
        //std::cout << "next test will be skipped." << std::endl;
        //return 0;
        

        // temp struct for line drawing.
        struct SimplePoint
        {
        public:
            hvector m_position;
            SimplePoint(const hvector& pos)
                :m_position(pos)
            {
                // empty
            }
        };
        // create and config pipline state object
        auto pso = std::make_unique<PiplineStateObject>();
        pso->m_primitiveType = PrimitiveType::LINE_LIST;
        pso->m_vertexLayout.vertexShaderInputSize = sizeof(hvector);
        pso->m_vertexLayout.pixelShaderInputSize  = sizeof(hvector);
        
        pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {

            RGBA color;
            color.m_chas.m_r = pVertex->m_posH.m_x * 1.0f / 512;
            color.m_chas.m_g = pVertex->m_posH.m_y * 1.0f / 512;

            return color;
        };

        pso->m_vertexShader = [](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pdestV)->void {
            memcpy(pdestV, pSrcVertex, sizeof(SimplePoint));
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
        SphereRay([&numIndices, &points, &indices](HELP_SPHERE_RAY_LAMBDA_PARAMETERS)->void {

            // add start vertex and its index
            points.push_back(SimplePoint(hvector(x0, y0)));
            indices.push_back(numIndices++);

            // add end vertex and its index
            points.push_back(SimplePoint(hvector(x1, y1)));
            indices.push_back(numIndices++);
        }, 
            0.8f, 0.0f, // center location
            0.3f,       // segment length
            0.1f,       // start radius
            12);         // num rounds

        auto vertexBuffer = std::make_unique<F32Buffer>(points.size() * 4 * sizeof(Types::F32));
        memcpy(vertexBuffer->GetBuffer(), points.data(), vertexBuffer->GetSizeOfByte());

        {
            TIME_GUARD;
            pipline.DrawInstance(indices, vertexBuffer.get());
        }

        pipline.m_backBuffer->SaveTo(".\\OutputTestImage\\PiplineTest\\lineClippingTest.png");
        
    TEST_UNIT_END;
#pragma endregion

#pragma region line vertex transformation
    TEST_UNIT_START("line vertex transformation")

        // skip this test due to the bug of clipping line function.
        //std::cout << "next test will be skipped." << std::endl;
        //return 0;

        // temp struct for line drawing.
        struct SimplePoint
        {
        public:
            hvector m_position;
            SimplePoint(const hvector& pos)
                :m_position(pos)
            {
                // empty
            }
        };
        // create and config pipline state object
        auto pso = std::make_unique<PiplineStateObject>();
        pso->m_primitiveType = PrimitiveType::LINE_LIST;
        pso->m_vertexLayout.vertexShaderInputSize = sizeof(hvector);
        pso->m_vertexLayout.pixelShaderInputSize  = sizeof(hvector);
        
        pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {

            RGBA color(RGBA::WHITE);
            //color.SetChannel<RGBA::R>(pVertex->m_posH.m_x * 1.0f / 512);
            //color.SetChannel<RGBA::G>(pVertex->m_posH.m_y * 1.0f / 512);

            Types::F32 chanV = (pVertex->m_posH.m_z + 1.0f) / 2.0f;
            const Types::F32 gamma = 4.0f;
            chanV = std::powf(chanV, gamma);
            chanV = 1.0f - chanV;
            color.SetChannel<RGBA::B>(chanV);
            color.SetChannel<RGBA::R>(chanV);
            color.SetChannel<RGBA::G>(chanV);

            return color;
        };

        vector3 localU(1.0f, 0.0f, 1.0f);
        vector3 localV(0.0f, 1.0f, 0.0f);
        vector3 coordPos(0.0f, 0.0f, 0.0f);
        CoordinateFrame coordFrm(localU, localV, coordPos);
        Transform toWorld = coordFrm.GetTransformLocalToWorld();

        pso->m_vertexShader = [&toWorld](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pDestV)->void {
            const hvector* pSrcH = reinterpret_cast<const hvector*>(pSrcVertex);
            hvector* pDestH = reinterpret_cast<hvector*>(pDestV);
            
            *pDestH = toWorld * (*pSrcH);
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
        SphereRay([&numIndices, &points, &indices](HELP_SPHERE_RAY_LAMBDA_PARAMETERS)->void {

            // add start vertex and its index
            points.push_back(SimplePoint(hvector(x0, y0)));
            indices.push_back(numIndices++);

            // add end vertex and its index
            points.push_back(SimplePoint(hvector(x1, y1)));
            indices.push_back(numIndices++);
        }, 
            0.8f, 0.0f, // center location
            0.3f,       // segment length
            0.1f,       // start radius
            12);         // num rounds

        auto vertexBuffer = std::make_unique<F32Buffer>(points.size() * 4 * sizeof(Types::F32));
        memcpy(vertexBuffer->GetBuffer(), points.data(), vertexBuffer->GetSizeOfByte());

        {
            TIME_GUARD;
            pipline.DrawInstance(indices, vertexBuffer.get());
        }

        pipline.m_backBuffer->SaveTo(".\\OutputTestImage\\PiplineTest\\lineVertexShaderProcess06.png");
        
    TEST_UNIT_END;
#pragma endregion

#pragma region orthographic view transformation
    TEST_UNIT_START("orthographic view transformation")

        // skip this test due to the bug of clipping line function.
        //std::cout << "next test will be skipped." << std::endl;
        //return 0;

        // temp struct for line drawing.
        struct SimplePoint
        {
        public:
            hvector m_position;
            SimplePoint(const hvector& pos)
                :m_position(pos)
            {
                // empty
            }
        };
        // create and config pipline state object
        auto pso = std::make_unique<PiplineStateObject>();
        pso->m_primitiveType = PrimitiveType::LINE_LIST;
        pso->m_vertexLayout.vertexShaderInputSize = sizeof(hvector);
        pso->m_vertexLayout.pixelShaderInputSize  = sizeof(hvector);
        
        pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {

            return RGBA::BLACK;
        };

        // using MARCO BEFORE TO select two output of images,
        // BEFORE represent the regular sphere ray
        // else represent the distortion sphere ray which will be stretched.
//#define BEFORE

#ifdef BEFORE
        const Types::F32 LEFT(-1.0f), RIGHT(1.0f), BOTTOM(-1.0f), TOP(1.0f), NEAR(+1.0f), FAR(-1.0f);
#else
        const Types::F32 LEFT(-1.5f), RIGHT(-0.5f), BOTTOM(-1.0f), TOP(1.0f), NEAR(+1.0f), FAR(-1.0f);
#endif// BEFORE

        Transform orthTrans = Transform::OrthographicTransOG(LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR);

        pso->m_vertexShader = [& orthTrans](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pDestV)->void {
            const hvector* pSrcH = reinterpret_cast<const hvector*>(pSrcVertex);
            hvector* pDestH = reinterpret_cast<hvector*>(pDestV);
            
            *pDestH = orthTrans * (*pSrcH);
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
        SphereRay([&numIndices, &points, &indices](HELP_SPHERE_RAY_LAMBDA_PARAMETERS)->void {

            // add start vertex and its index
            points.push_back(SimplePoint(hvector(x0, y0)));
            indices.push_back(numIndices++);

            // add end vertex and its index
            points.push_back(SimplePoint(hvector(x1, y1)));
            indices.push_back(numIndices++);
        }, 
            0.0f, 0.0f, // center location
            0.3f,       // segment length
            0.1f,       // start radius
            3);         // num rounds

        auto vertexBuffer = std::make_unique<F32Buffer>(points.size() * 4 * sizeof(Types::F32));
        memcpy(vertexBuffer->GetBuffer(), points.data(), vertexBuffer->GetSizeOfByte());

        {
            TIME_GUARD;
            pipline.DrawInstance(indices, vertexBuffer.get());
        }

#ifdef BEFORE
        pipline.m_backBuffer->SaveTo(".\\OutputTestImage\\PiplineTest\\orthographicViewTransformationBefore.png");
#else
        pipline.m_backBuffer->SaveTo(".\\OutputTestImage\\PiplineTest\\orthographicViewTransformationAfter.png");
#endif
        
    TEST_UNIT_END;
#pragma endregion

#pragma region perspective view transformation
    TEST_UNIT_START("perspective view transformation")

        // skip this test due to the bug of clipping line function.
        //std::cout << "next test will be skipped." << std::endl;
        //return 0;

        // temp struct for line drawing.
        struct SimplePoint
        {
        public:
            hvector m_position;
            SimplePoint(const hvector& pos)
                :m_position(pos)
            {
                // empty
            }
        };
        // create and config pipline state object
        auto pso = std::make_unique<PiplineStateObject>();
        pso->m_primitiveType = PrimitiveType::LINE_LIST;
        pso->m_vertexLayout.vertexShaderInputSize = sizeof(hvector);
        pso->m_vertexLayout.pixelShaderInputSize  = sizeof(hvector);
        
        pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
            const Types::F32 depth = (pVertex->m_posH.m_z + 1.0f) * 0.5f;
            RGBA color(depth, depth, depth, 1.0f);
            return color;
        };

        const Types::F32 LEFT(-1.0f), RIGHT(1.0f), BOTTOM(-1.0f), TOP(1.0f), NEAR(-1.0f), FAR(-4.0f);

        // rotate the line a little.
        Transform rotateY = Transform::RotationY(Types::Constant::PI_F / 3.0f);
        Transform rotateZ = Transform::RotationZ(Types::Constant::PI_F / 3.0f);

        Transform moveIntoScree = Transform::Translation(0.0f, 0.0f, -2.0f);

        // perspective tranformation
        Transform perTrans = Transform::PerspectiveOG(LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR);
        
        Transform pushInto = Transform::Translation(0.0f, 0.0f, -2.0f);

        Transform mat = perTrans * pushInto * rotateZ * rotateY;

        pso->m_vertexShader = [&mat](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pDestV)->void {
            const hvector* pSrcH = reinterpret_cast<const hvector*>(pSrcVertex);
            hvector* pDestH = reinterpret_cast<hvector*>(pDestV);
            
            *pDestH = mat * (*pSrcH);
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
        SphereRay([&numIndices, &points, &indices](HELP_SPHERE_RAY_LAMBDA_PARAMETERS)->void {

            // add start vertex and its index
            points.push_back(SimplePoint(hvector(x0, y0, 0.0f)));
            indices.push_back(numIndices++);

            // add end vertex and its index
            points.push_back(SimplePoint(hvector(x1, y1, 0.0f)));
            indices.push_back(numIndices++);
        }, 
            0.0f, 0.0f, // center location
            0.3f,       // segment length
            0.1f,       // start radius
            12);         // num rounds

        auto vertexBuffer = std::make_unique<F32Buffer>(points.size() * 4 * sizeof(Types::F32));
        memcpy(vertexBuffer->GetBuffer(), points.data(), vertexBuffer->GetSizeOfByte());


        {
            TIME_GUARD;

            DebugGuard<DEBUG_CLIENT_CONF_LINE_CLIP_ERROR_ANALYSIS> guard;
            pipline.DrawInstance(indices, vertexBuffer.get());
        }

        pipline.m_backBuffer->SaveTo(".\\OutputTestImage\\PiplineTest\\perspectiveViewTransformation.png");
        
    TEST_UNIT_END;
#pragma endregion

#pragma region perspective single line
    TEST_UNIT_START("perspective single line")

        // skip this test due to the bug of clipping line function.
        //return 0;

        // temp struct for line drawing.
        struct SimplePoint
        {
        public:
            hvector m_position;
            SimplePoint(const hvector& pos)
                :m_position(pos)
            {
                // empty
            }
        };
        // create and config pipline state object
        auto pso = std::make_unique<PiplineStateObject>();
        pso->m_primitiveType = PrimitiveType::LINE_LIST;
        pso->m_vertexLayout.vertexShaderInputSize = sizeof(hvector);
        pso->m_vertexLayout.pixelShaderInputSize  = sizeof(hvector);
        
        pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
            
            return RGBA::BLACK;
        };

        const Types::F32 LEFT(-1.0f), RIGHT(1.0f), BOTTOM(-1.0f), TOP(1.0f), NEAR(-1.0f), FAR(-4.0f);

        // rotate the line a little.
        Transform rotateY = Transform::RotationY(Types::Constant::PI_F / 3.0f);

        Transform moveIntoScree = Transform::Translation(0.0f, 0.0f, -2.0f);

        // perspective tranformation
        Transform perTrans = Transform::PerspectiveOG(LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR);

        Transform mat = perTrans;// * rotateY;

        pso->m_vertexShader = [&mat](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pDestV)->void {
            const hvector* pSrcH = reinterpret_cast<const hvector*>(pSrcVertex);
            hvector* pDestH = reinterpret_cast<hvector*>(pDestV);
            
            *pDestH = mat * (*pSrcH);
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

        indices.push_back(0);
        indices.push_back(1);

        SimplePoint p1(hvector(0.5f, 0.5f, -0.5f, 1.0f));
        SimplePoint p2(hvector(-0.5f, -0.5f, -6.0f, 1.0f));
        points.push_back(p1);
        points.push_back(p2);

        auto vertexBuffer = std::make_unique<F32Buffer>(points.size() * 4 * sizeof(Types::F32));
        memcpy(vertexBuffer->GetBuffer(), points.data(), vertexBuffer->GetSizeOfByte());

        {
            TIME_GUARD;
            //DebugGuard<DEBUG_CLIENT_CONF_LINE_CLIP_ERROR_ANALYSIS> guard;
            //PUT_BREAK_POINT;
            pipline.DrawInstance(indices, vertexBuffer.get());
        }

        pipline.m_backBuffer->SaveTo(".\\OutputTestImage\\PiplineTest\\perspectiveSingleLineShowInvisible.png");
        
    TEST_UNIT_END;
#pragma endregion

#pragma region line clipping error analysis
    TEST_UNIT_START("line clipping error analysis")

        // skip this test due to the bug of clipping line function.
        //return 0;

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
        pso->m_primitiveType = PrimitiveType::LINE_LIST;
        pso->m_vertexLayout.vertexShaderInputSize = sizeof(SimplePoint);
        pso->m_vertexLayout.pixelShaderInputSize  = sizeof(SimplePoint);
        
        pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
            const Types::F32   depth      = (pVertex->m_posH.m_z + 1.0f) * 0.5f;
            const SimplePoint* pPoint     = reinterpret_cast<const SimplePoint*>(pVertex);
            const Types::F32   red        = pPoint->m_rayIndex.m_z;

            const Types::U32   roundIndex = static_cast<const Types::U32>(pPoint->m_rayIndex.m_x);
            const Types::U32   lineIndex  = static_cast<const Types::U32>(pPoint->m_rayIndex.m_y);
            //const Types::F32   isTheOne   = 22 < lineIndex && lineIndex < 34 ? 1.0f : 0.0f;
            const Types::F32   isTheOne = roundIndex == 7 && 34 <= lineIndex && lineIndex <= 34  ? 1.0f : 0.0f;

            RGBA               color     (isTheOne, 0.0f, 0.0f, 1.0f);
            return color;
        };

        const Types::F32 LEFT(-1.0f), RIGHT(1.0f), BOTTOM(-1.0f), TOP(1.0f), NEAR(-0.5f), FAR(-4.0f);

        // rotate the line a little.
        Transform rotateY = Transform::RotationY(Types::Constant::PI_F / 3.0f);
        Transform rotateZ = Transform::RotationZ(Types::Constant::PI_F / 3.0f);

        Transform moveIntoScree = Transform::Translation(0.0f, 0.0f, -2.0f);

        // perspective tranformation
        Transform perTrans = Transform::PerspectiveOG(LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR);
        
        Transform pushInto = Transform::Translation(0.0f, 0.0f, -2.0f);

        Transform mat = perTrans * pushInto * rotateZ * rotateY;

        Transform toView = pushInto * rotateZ * rotateY;

        Transform perspect = perTrans;

        pso->m_vertexShader = [&mat, &toView, &perspect](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pDestV)->void {
            const SimplePoint* pSrcH = reinterpret_cast<const SimplePoint*>(pSrcVertex);
            SimplePoint* pDestH = reinterpret_cast<SimplePoint*>(pDestV);

            //DebugClient<DEBUG_CLIENT_CONF_LINE_CLIP_ERROR_ANALYSIS>(pSrcH->m_rayIndex.m_x == 7.0f && pSrcH->m_rayIndex.m_y == 34.0f);
            
            hvector inViewPos = toView * pSrcH->m_position;

            pDestH->m_position = perspect * inViewPos;
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
        SphereRay([&numIndices, &points, &indices](HELP_SPHERE_RAY_LAMBDA_PARAMETERS)->void {

            const unsigned int theIndexOfOneLine = 64;
            // only get one line for convenience of debugging
            //if (lineIndex == theIndexOfOneLine)
            //{
                // add start vertex and its index
                SimplePoint start(hvector(x0, y0, 0.0f));
                start.m_rayIndex.m_x = roundIndex;
                start.m_rayIndex.m_y = lineIndex;
                start.m_rayIndex.m_z = 0;
                points.push_back(start);
                indices.push_back(numIndices++);

                // add end vertex and its index
                SimplePoint end(hvector(x1, y1, 0.0f));
                end.m_rayIndex.m_x = roundIndex;
                end.m_rayIndex.m_y = lineIndex;
                end.m_rayIndex.m_z = 1;
                points.push_back(end);
                indices.push_back(numIndices++);
            //}
        }, 
            0.0f, 0.0f,                     // center location
            0.3f,                           // segment length
            0.1f,                           // start radius
            12                              // num rounds
            ); // radio offset

        auto vertexBuffer = std::make_unique<F32Buffer>(points.size() * sizeof(SimplePoint));
        memcpy(vertexBuffer->GetBuffer(), points.data(), vertexBuffer->GetSizeOfByte());

        {
            TIME_GUARD;
            DebugGuard<DEBUG_CLIENT_CONF_LINE_CLIP_ERROR_ANALYSIS> guard;
            pipline.DrawInstance(indices, vertexBuffer.get());
        }

        std::string pictureIndex = "final";
        pipline.m_backBuffer->SaveTo(".\\OutputTestImage\\PiplineTest\\lineClippingErrAnalysis_fixed_" + pictureIndex + ".png");
        
    TEST_UNIT_END;
#pragma endregion


TEST_MODULE_END