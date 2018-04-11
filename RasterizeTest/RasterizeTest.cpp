// For Unit Test
#include <MyTools\UnitTestModules.h>
#include <iostream>
#include <MyTools\RandomToolNeedLib\MTRandom.h>
#include <MyTools\MathTool.h>
#include <limits>
#include <array>
#include <ctime>
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

        defaultImg.DrawBresenhamLine(0, 0, UserConfig::COMMON_PIXEL_WIDTH - 1, UserConfig::COMMON_PIXEL_HEIGHT - 1);
        
        defaultImg.DrawBresenhamLine(100, 50, 400, 300);
        defaultImg.DrawBresenhamLine(100, 400, 50, 300);
        defaultImg.DrawBresenhamLine(500, 400, 200, 10);
        
        defaultImg.SaveTo(".\\OutputTestImage\\DrawBresenhamLine\\default_color.png");

    TEST_UNIT_END;
#pragma endregion

TEST_MODULE_END