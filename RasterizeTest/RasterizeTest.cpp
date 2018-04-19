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

/*!
    \brief help to generate sphere ray lines to be drawn.
    \param drawLine the function to recieve the line start and end points.
    \param centerLocationX the center x location of the sphere.
    \param centerLocationY the center y location of the sphere.
    \param segmentLength the length of each segment
    \param startInnerRadius the minimum radius of the spheres
    \param numRounds how many rounds to draw.
*/
void SphereRay(
    std::function<void(const Types::F32 x0, const Types::F32 y0, const Types::F32 x1, const Types::F32 y1)>
    drawLine,
    const Types::F32 centerLocationX,
    const Types::F32 centerLocationY,
    const Types::F32 segmentLength = 35.0f,
    const Types::F32 startInnerRadius = 30.0f,
    const Types::U32 numRounds = 5)
{
        Types::F32 deltaTheta = 2.0f * MathTool::PI_F / 64.0f;
        Types::F32 theta = 0.0f;
        Types::F32 rIn = startInnerRadius;
        Types::F32 rOut = rIn + segmentLength;

        Types::F32 x0, y0, x1, y1;
        for (unsigned int i = 0; i < numRounds; ++i)
        {
            for (unsigned int j = 0; j < 64; ++j)
            {
                //BREAK_POINT_IF(i == 3 && j == 0 && centerLocationX == 0.5f);
                theta = deltaTheta * j + (i % 2) * deltaTheta * 0.5f;
                x0 = centerLocationX + rIn  * std::cos(theta);
                y0 = centerLocationY + rIn  * std::sin(theta);
                x1 = centerLocationX + rOut * std::cos(theta);
                y1 = centerLocationY + rOut * std::sin(theta);

                drawLine(x0, y0, x1, y1);
            }
            rIn = rOut;
            rOut += segmentLength;
        } // end outer for loop
}

#define HELP_SPHERE_RAY_LAMBDA_PARAMETERS const Types::F32 x0, const Types::F32 y0, const Types::F32 x1, const Types::F32 y1

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

            SphereRay([&defaultImg](const Types::F32 x0, const Types::F32 y0, const Types::F32 x1, const Types::F32 y1) -> void {
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

            SphereRay([&defaultImg](const Types::F32 x0, const Types::F32 y0, const Types::F32 x1, const Types::F32 y1) -> void{
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

            SphereRay([&defaultImg](const Types::F32 x0, const Types::F32 y0, const Types::F32 x1, const Types::F32 y1) -> void {
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

            SphereRay([&defaultImg](const Types::F32 x0, const Types::F32 y0, const Types::F32 x1, const Types::F32 y1) -> void{
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

            SphereRay([&defaultImg](const Types::F32 x0, const Types::F32 y0, const Types::F32 x1, const Types::F32 y1) -> void {
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

            SphereRay([&defaultImg](const Types::F32 x0, const Types::F32 y0, const Types::F32 x1, const Types::F32 y1) -> void {
                defaultImg.DrawLine(x0, y0, x1, y1, RGB::GREEN);
            }, CENTER_X, CENTER_Y, 0.1f, 0.1f, 6);


            defaultImg.SetScissor(ScissorRect(300, 511,   0, 511));

            SphereRay([&defaultImg](const Types::F32 x0, const Types::F32 y0, const Types::F32 x1, const Types::F32 y1) -> void {
                defaultImg.DrawLine(x0, y0, x1, y1, RGB::RED);
            }, CENTER_X, CENTER_Y, 0.3f, 0.2f, 6);


            defaultImg.SetScissor(ScissorRect(  0, 300,   0, 300));

            SphereRay([&defaultImg](const Types::F32 x0, const Types::F32 y0, const Types::F32 x1, const Types::F32 y1) -> void {
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

        // create and config pipline state object
        auto pso = std::make_unique<PiplineStateObject>();
        pso->m_primitiveType = PrimitiveType::LINE_LIST;
        pso->m_vertexLayout.vertexShaderInputSize = 4 * sizeof(Types::F32);
        pso->m_vertexLayout.pixelShaderInputSize  = 4 * sizeof(Types::F32);

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

TEST_MODULE_END