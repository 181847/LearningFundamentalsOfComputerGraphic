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

    std::string pictureIndex = "000";
    pipline.m_backBuffer->SaveTo("..\\RasterizeTest\\OutputTestImage\\PiplineTest\\TriangleTest\\screenSpaceTriangle_" + pictureIndex + ".png");
        
TEST_UNIT_END;
#pragma endregion


TEST_MODULE_END
