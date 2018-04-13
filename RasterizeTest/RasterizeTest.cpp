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
    \param centerLocationX the center x location of the sphere.
    \param centerLocationY the center y location of the sphere.
    \param segmentLength the length of each segment
    \param startInnerRadius the minimum radius of the spheres
    \param numRounds how many rounds to draw.
    \param drawLine the function to recieve the line start and end points.
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
        Types::F32 segmentLen = 35.0f;
        Types::F32 rIn = 30.0f;
        Types::F32 rOut = rIn + segmentLen;

        Types::F32 x0, y0, x1, y1;
        for (unsigned int i = 0; i < numRounds; ++i)
        {
            for (unsigned int j = 0; j < 64; ++j)
            {
                theta = deltaTheta * j + (i % 2) * deltaTheta * 0.5f;
                x0 = centerLocationX + rIn  * std::cos(theta);
                y0 = centerLocationY + rIn  * std::sin(theta);
                x1 = centerLocationX + rOut * std::cos(theta);
                y1 = centerLocationY + rOut * std::sin(theta);

                drawLine(x0, y0, x1, y1);
            }
            rIn = rOut;
            rOut += segmentLen;
        } // end outer for loop
}

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

#pragma region draw bresenham lines
    TEST_UNIT_START("draw bresenham lines")
        RasterizeImage defaultImg(UserConfig::COMMON_PIXEL_WIDTH, UserConfig::COMMON_PIXEL_HEIGHT, RGBA::WHITE);

        const Types::F32 CENTER_X = defaultImg.m_width / 2.0f,
                         CENTER_Y = defaultImg.m_height / 2.0f;

        {
            TimeGuard countDrawLines(timeCounter);

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
    TEST_UNIT_START("draw WuXiaolin lines")
        RasterizeImage defaultImg(UserConfig::COMMON_PIXEL_WIDTH, UserConfig::COMMON_PIXEL_HEIGHT, RGBA::WHITE);

        const Types::F32 CENTER_X = defaultImg.m_width / 2.0f,
                         CENTER_Y = defaultImg.m_height / 2.0f;

        {
            TimeGuard countDrawLines(timeCounter);

            SphereRay([&defaultImg](const Types::F32 x0, const Types::F32 y0, const Types::F32 x1, const Types::F32 y1) -> void {
                defaultImg.DrawWuXiaolinLine(x0, y0, x1, y1);
            }, CENTER_X, CENTER_Y);
        }// end time counter
        
        defaultImg.SaveTo(".\\OutputTestImage\\DrawWuXiaolinLine\\sphere_ray.png");

    TEST_UNIT_END;
#pragma endregion

#pragma region draw bresenham lines
    TEST_UNIT_START("draw bresenham lines in one call")
        RasterizeImage defaultImg(UserConfig::COMMON_PIXEL_WIDTH, UserConfig::COMMON_PIXEL_HEIGHT, RGBA::WHITE);

        const Types::F32 CENTER_X = defaultImg.m_width / 2.0f,
                         CENTER_Y = defaultImg.m_height / 2.0f;

        {
            TimeGuard countDrawLines(timeCounter);

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

TEST_MODULE_END