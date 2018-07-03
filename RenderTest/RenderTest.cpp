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
#include "../CommonClasses/ImageWindow.h"
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
    \brief get random RGB color
    the random number is generated by the globalMtr (which is also used by other functions in this test module).
*/
RGB GetRandomRGB()
{
    return RGB(globalMtr.Random(), globalMtr.Random(), globalMtr.Random());
}

/*!
    \brief create a quad poly.
    \param p1-4 four point position
    remember the clockwise is front face.
    A-------->
    |
    |   front order
    |
    <--------
*/
auto CreatQuadPoly(const vector3 &p1, const vector3 &p2, const vector3 &p3, const vector3 &p4)
{

    auto poly = std::make_unique<Polygon>(
        p1,
        p2,
        p3
        );

    poly->AddPoint(p4);

    return poly;
}

/*!
    \brief help to get point position in an array whose size is 8.
*/
enum PointsOrder
{
    TOP_LEFT_FRONT = 0,
    TOP_LEFT_BACK,
    TOP_RIGHT_BACK,
    TOP_RIGHT_FRONT,
    BOTTOM_LEFT_FRONT,
    BOTTOM_LEFT_BACK,
    BOTTOM_RIGHT_BACK,
    BOTTOM_RIGHT_FRONT
};
/*!
    \brief build six poly for a box
    \param points eight points to consist a box,
    \return 6 polygons which is visible from the outside of the box, polygons are in the order of {top, bottom, left, right, front, back}
    first four points is on the top, ranged in clockwise
    last four points is on the botom, ranged ALSO in clockwise
    --------->
    A        |  \
    |  top   |   \
    |start   V    \
    *<--------     V
    \       ---------->
     \      A         |
      \     |   bottom|
       \    |start    V
        V   *<---------

*/
std::array<std::unique_ptr<Polygon>, 6> CreatBox(const std::array<vector3, 8>& points)
{
    std::array<std::unique_ptr<Polygon>, 6> boxPolys;

    // top
    boxPolys[0] = std::move(CreatQuadPoly(
        points[PointsOrder::TOP_LEFT_FRONT],
        points[PointsOrder::TOP_LEFT_BACK], 
        points[PointsOrder::TOP_RIGHT_BACK], 
        points[PointsOrder::TOP_RIGHT_FRONT]));

    // bottom
    boxPolys[1] = std::move(CreatQuadPoly(
        points[PointsOrder::BOTTOM_LEFT_BACK],
        points[PointsOrder::BOTTOM_LEFT_FRONT],
        points[PointsOrder::BOTTOM_RIGHT_FRONT],
        points[PointsOrder::BOTTOM_RIGHT_BACK]));

    // left
    boxPolys[2] = std::move(CreatQuadPoly(
        points[PointsOrder::TOP_LEFT_BACK],
        points[PointsOrder::TOP_LEFT_FRONT],
        points[PointsOrder::BOTTOM_LEFT_FRONT],
        points[PointsOrder::BOTTOM_LEFT_BACK]));

    // right
    boxPolys[3] = std::move(CreatQuadPoly(
        points[PointsOrder::TOP_RIGHT_FRONT],
        points[PointsOrder::TOP_RIGHT_BACK],
        points[PointsOrder::BOTTOM_RIGHT_BACK],
        points[PointsOrder::BOTTOM_RIGHT_FRONT]));

    // front
    boxPolys[4] = std::move(CreatQuadPoly(
        points[PointsOrder::TOP_LEFT_FRONT],
        points[PointsOrder::TOP_RIGHT_FRONT],
        points[PointsOrder::BOTTOM_RIGHT_FRONT],
        points[PointsOrder::BOTTOM_LEFT_FRONT]));

    // back
    boxPolys[5] = std::move(CreatQuadPoly(
        points[PointsOrder::TOP_RIGHT_BACK],
        points[PointsOrder::TOP_LEFT_BACK],
        points[PointsOrder::BOTTOM_LEFT_BACK],
        points[PointsOrder::BOTTOM_RIGHT_BACK]));

    return boxPolys;
}

/*!
	\brief let user to check the local image file, this function can be disabled by the constant UserConfig::LET_USER_CHECK_IMG.
	if the user input some number, then we think this test is passed, and return 0 for no error.
*/
unsigned int LetUserCheckJudge(const std::string& msg, bool force = UserConfig::LET_USER_CHECK_IMG)
{
	if (force)
	{
		std::cout << "Please check: " << msg << std::endl
			<< "Input '0' ~ '9' for test pass." << std::endl
			<< "Else hit enter for some error: ";

		char ch = std::cin.get();

		// if the character is not number, then the image is wrong.
		if (ch < '0' || ch > '9')
		{
			return 1;
		}

		std::cin.clear();
		std::cin.ignore(std::numeric_limits<int>::max(), '\n');
	}

	// no error happend 
	return 0;
}

TEST_MODULE_START

#pragma region render a scene use scene RayColor() function
	TEST_UNIT_START("render a scene use scene RayColor() function")
		using namespace CommonClass;

        const bool SKIP_THIS_TEST = true;

        if (SKIP_THIS_TEST)
        {
            std::cout << "test skipped";
            return errorLogger.conclusion();
        }

		/*!
		`	\brief set scene and light.
		*/
        Scene scene;
        RGB backgroundColor(RGB::BLACK);

        /*!
            \brief make up materials.
        */
        Material sphereMat;
        Material triMat;
        Material polyMat;

        sphereMat.m_kDiffuse        = RGB::RED;
        sphereMat.m_shinness        = 12.0f;
        sphereMat.SetRFresnel0(8);
        triMat.m_kDiffuse           = RGB::YELLOW;
        triMat.m_shinness           = 1.0f;
        triMat.SetRFresnel0(8);
        polyMat.m_kDiffuse          = RGB(0.3f, 0.5f, 0.9f);
        polyMat.m_shinness          = 0.5f;
        polyMat.SetRFresnel0(8);

        vector3 pointLightPosition(-3.0f, 4.0f, 3.0f);
        RGB pointLightColor = RGB::WHITE;
        auto pointLight = std::make_unique<Light>(pointLightPosition * 5.0f, pointLightColor);

        scene.Add(std::move(pointLight));

		/*!
			\brief set a sphere to render.
		*/
		auto tsph = std::make_unique<Sphere>(vector3(0.0f, 0.0f, 0.0f), 1.0f);
        tsph->m_material = sphereMat;
        auto tsph2 = std::make_unique<Sphere>(vector3(-1.0f, 2.0f, 0.5f), 1.0f);
        tsph2->m_material = sphereMat;
        auto tsph3 = std::make_unique<Sphere>(vector3(-1.0f, 0.0f, 2.0f), 1.0f);
        tsph3->m_material = sphereMat;

        /*!
            \brief render triangles
        */
		const Types::F32 borderLength = 20.0f;
		auto tri1 = std::make_unique<Triangle>(
			vector3(-borderLength, -2.0f, +borderLength),
			vector3(-borderLength, -2.0f, -borderLength),
			vector3(+borderLength, -2.0f, borderLength));
		auto tri2 = std::make_unique<Triangle>(
			vector3(+borderLength, -2.0f, +borderLength),
			vector3(-borderLength, -2.0f, -borderLength),
			vector3(+borderLength, -2.0f, -borderLength));
        tri1->m_material = triMat;
        tri2->m_material = triMat;

        /*!
            \brief render polygon
        */
        auto poly = std::make_unique<Polygon>(
            vector3(-borderLength, -borderLength, -2.0f),
            vector3(-borderLength, borderLength, -2.0f),
            vector3(+borderLength, borderLength, -2.0f)
            );
        poly->AddPoint(vector3(borderLength * 0.9f, -borderLength * 3.0f, -2.0f));
        poly->AddPoint(vector3(borderLength * 0.1f, -borderLength * 4.0f, -2.0f));
        poly->m_material = polyMat;


        scene.Add(std::move(tsph));
        scene.Add(std::move(tsph2));
        scene.Add(std::move(tsph3));
        scene.Add(std::move(tri1));
        scene.Add(std::move(tri2));
        scene.Add(std::move(poly));

		/*!
			\brief config a camera.
		*/
        vector3 camPosition = vector3(1.8f, 1.8f, 1.8f);
        //vector3 camPosition = 2.0f * vector3(1.8f, 1.8f, 1.8f);
        //vector3 camPosition = vector3( -3.8f, 1.0f, 1.8f);
		vector3 camTarget = vector3(0.0f, 0.0f, 0.0f);
		vector3 camLookUp = vector3(0.0f, 1.0f, 0.0f);
		Types::F32 focalLength = 0.5f;
		PerspectiveCamera camera(focalLength, camPosition, camTarget, camLookUp);

		camera.SetFilm(std::make_unique<Film>(
			UserConfig::COMMON_PIXEL_WIDTH, UserConfig::COMMON_PIXEL_HEIGHT,
			-0.5f, +0.5f,
			-0.5f, +0.5f));

		HitRecord hitRec, shadowHitRec;
		Ray viewRay;
		for (unsigned int i = 0; i < camera.m_film->m_width; ++i)
		{
			for (unsigned int j = 0; j < camera.m_film->m_height; ++j)
			{
				viewRay = camera.GetRay(i, j);

                camera.IncomeLight(i, j, scene.RayColor(viewRay, 0.0f, 1000.0f));
			}
		}

		camera.m_film->SaveTo("OutputTestImage\\RenderReflect\\ThisIsForSceneRayColor06.png");

		errorLogger += LetUserCheckJudge(
			"check \".\\OutputTestImage\\RenderReflect\\ThisIsForSceneRayColor01.png\"\n"
			"you should have seen a sphere with some color, the background is dark green.");

	TEST_UNIT_END;
#pragma endregion
 
#pragma region render inside boxes and sphere
	TEST_UNIT_START("render inside boxes and sphere")
		using namespace CommonClass;

        const bool SKIP_THIS_TEST = false;

        if (SKIP_THIS_TEST)
        {
            std::cout << "test skipped";
            return errorLogger.conclusion();
        }

		/*!
		`	\brief set scene and light.
		*/
        Scene scene;

        /*!
            \brief make up materials.
        */
        Material sphereMat      (RGB(0.5f, 0.2f, 1.0f),              8,  16.0f);
        Material triMat         (RGB::YELLOW,           8,  1.0f);
        Material polyMat        (RGB(0.3f, 0.5f, 0.9f), 8,  1.0f);
        Material polyMat_left   (RGB::RED,              8,  1.0f);
        Material polyMat_right  (RGB::GREEN,            8,  1.0f);
        Material polyMat_top    (RGB::WHITE * 0.6f,            8,  3.0f);
        Material polyMat_bottom (RGB::WHITE * 0.6f,            8,  3.0f);
        Material polyMat_back   (RGB::WHITE * 0.6f,            8,  3.0f);
        Material box_1_Mat      (RGB::WHITE * 0.8f,            8,  2.0f);
        Material box_2_Mat      (RGB::WHITE * 0.8f,            8,  2.0f);


        vector3 pointLightPosition(0.0f, 5.0f, 0.0f);
        RGB pointLightColor = RGB::WHITE;
        auto pointLight1 = std::make_unique<Light>(pointLightPosition, pointLightColor * 0.5f);
        auto pointLight2 = std::make_unique<Light>(vector3(2.2f, 2.7f, 0.0f), pointLightColor * 0.2f);
        auto pointLight3 = std::make_unique<Light>(vector3(-2.2f, 2.7f, 0.0f), pointLightColor * 0.2f);

        scene.Add(std::move(pointLight1));
        scene.Add(std::move(pointLight2));
        scene.Add(std::move(pointLight3));

		/*!
			\brief set a sphere to render.
		*/
		auto tsph = std::make_unique<Sphere>(vector3(-1.0617f, 0.8190f, 1.2368f), 0.8f);
        tsph->m_material = sphereMat;

#pragma region build the big open box

        Types::F32 outBoxLeft = -2.5f;
        Types::F32 outBoxRight = +2.5f;
        Types::F32 outBoxTop = 5.470f;
        Types::F32 outBoxBack = -2.813f;
        Types::F32 outBoxFront = 2.813f;
        Types::F32 outBoxBottom = 0.100f;

        std::array<vector3, 8> outBoxPoints = {
            vector3(outBoxLeft, outBoxTop, outBoxFront),
            vector3(outBoxLeft, outBoxTop, outBoxBack),
            vector3(outBoxRight, outBoxTop, outBoxBack),
            vector3(outBoxRight, outBoxTop, outBoxFront),
            vector3(outBoxLeft, outBoxBottom, outBoxFront),
            vector3(outBoxLeft, outBoxBottom, outBoxBack),
            vector3(outBoxRight, outBoxBottom, outBoxBack),
            vector3(outBoxRight, outBoxBottom, outBoxFront)
        };
        
        auto leftPoly = CreatQuadPoly(
            outBoxPoints[PointsOrder::TOP_LEFT_FRONT],
            outBoxPoints[PointsOrder::TOP_LEFT_BACK],
            outBoxPoints[PointsOrder::BOTTOM_LEFT_BACK],
            outBoxPoints[PointsOrder::BOTTOM_LEFT_FRONT]
        );
        auto rightPoly = CreatQuadPoly(
            outBoxPoints[PointsOrder::TOP_RIGHT_BACK],
            outBoxPoints[PointsOrder::TOP_RIGHT_FRONT],
            outBoxPoints[PointsOrder::BOTTOM_RIGHT_FRONT],
            outBoxPoints[PointsOrder::BOTTOM_RIGHT_BACK]
        );
        auto bottomPoly = CreatQuadPoly(
            outBoxPoints[PointsOrder::BOTTOM_LEFT_FRONT],
            outBoxPoints[PointsOrder::BOTTOM_LEFT_BACK],
            outBoxPoints[PointsOrder::BOTTOM_RIGHT_BACK],
            outBoxPoints[PointsOrder::BOTTOM_RIGHT_FRONT]
        );
        auto topPoly = CreatQuadPoly(
            outBoxPoints[PointsOrder::TOP_LEFT_FRONT],
            outBoxPoints[PointsOrder::TOP_RIGHT_FRONT],
            outBoxPoints[PointsOrder::TOP_RIGHT_BACK],
            outBoxPoints[PointsOrder::TOP_LEFT_BACK]
        );
        auto backPoly = CreatQuadPoly(
            outBoxPoints[PointsOrder::TOP_LEFT_BACK],
            outBoxPoints[PointsOrder::TOP_RIGHT_BACK],
            outBoxPoints[PointsOrder::BOTTOM_RIGHT_BACK],
            outBoxPoints[PointsOrder::BOTTOM_LEFT_BACK]
        );

        leftPoly    ->m_material = polyMat_left;
        rightPoly   ->m_material = polyMat_right;
        bottomPoly  ->m_material = polyMat_bottom;
        topPoly     ->m_material = polyMat_top;
        backPoly    ->m_material = polyMat_back;

        scene.Add(std::move(leftPoly));
        scene.Add(std::move(rightPoly));
        scene.Add(std::move(bottomPoly));
        scene.Add(std::move(topPoly));
        scene.Add(std::move(backPoly));

#pragma endregion

#pragma region build inner box 1
        std::array<vector3, 8> innerBox_1_Points = {
            vector3(-1.689f, +3.286f, -0.560f),
            vector3(-2.187f, +3.286f, -2.007f),
            vector3(-0.503f, +3.286f, -2.587f),
            vector3(-0.005f, +3.286f, -1.140f),
            vector3(-1.689f, +0.112f, -0.560f),
            vector3(-2.187f, +0.112f, -2.007f),
            vector3(-0.503f, +0.112f, -2.587f),
            vector3(-0.005f, +0.112f, -1.140f)};

        auto innerBox1 = CreatBox(innerBox_1_Points);

        for (auto & polyFace : innerBox1)
        {
            polyFace->m_material = box_1_Mat;
        }

        /*!
            \brief add each poly seperately for debuging.
        */
        // TOP
        scene.Add(std::move(innerBox1[0]));
        
        // BOTTOM
        scene.Add(std::move(innerBox1[1]));

        // LEFT
        scene.Add(std::move(innerBox1[2]));

        // RIGHT
        scene.Add(std::move(innerBox1[3]));

        // FRONT
        scene.Add(std::move(innerBox1[4]));

        // BACK
        scene.Add(std::move(innerBox1[5]));

#pragma endregion

#pragma region build inner box 2
        std::array<vector3, 8> innerBox_2_Points = {
            vector3(+0.612f, +2.001f, -0.716f),
            vector3(+1.077f, +2.001f, -1.671f),
            vector3(+2.184f, +2.001f, -1.131f),
            vector3(+1.718f, +2.001f, -0.177f),
            vector3(+0.612f, +0.112f, -0.716f),
            vector3(+1.077f, +0.112f, -1.671f),
            vector3(+2.184f, +0.112f, -1.131f),
            vector3(+1.718f, +0.112f, -0.177f)};

        auto innerBox2 = CreatBox(innerBox_2_Points);

        for (auto & polyFace : innerBox2)
        {
            polyFace->m_material = box_2_Mat;
        }

        /*!
            \brief add each poly seperately for debuging.
        */
        // TOP
        scene.Add(std::move(innerBox2[0]));
        
        // BOTTOM
        scene.Add(std::move(innerBox2[1]));

        // LEFT
        scene.Add(std::move(innerBox2[2]));

        // RIGHT
        scene.Add(std::move(innerBox2[3]));

        // FRONT
        scene.Add(std::move(innerBox2[4]));

        // BACK
        scene.Add(std::move(innerBox2[5]));

#pragma endregion

        scene.Add(std::move(tsph));

		/*!
			\brief config a camera.
		*/
        //vector3 camPosition = vector3(0.0f, 2.618f, 9.564f);        // original camera position
        //vector3 camPosition = vector3(-0.2f, 5.5f, 3.0f);
        //vector3 camPosition = vector3( -3.8f, 1.0f, 1.8f);
        vector3 camPosition = vector3(0.0f, 3.6f, 8.0f);
		vector3 camTarget = vector3(0.0f, 2.145f, 0.0f);
		vector3 camLookUp = vector3(0.0f, 1.0f, 0.0f);
		Types::F32 focalLength = 1.0f;
		PerspectiveCamera camera(focalLength, camPosition, camTarget, camLookUp);

		camera.SetFilm(std::make_unique<Film>(
			UserConfig::COMMON_PIXEL_WIDTH * 2, UserConfig::COMMON_PIXEL_HEIGHT * 2,
			-0.5f, +0.5f,
			-0.5f, +0.5f));

        /*!
            \brief help to trace the rendering progress.
        */
        const unsigned int PIXEL_WIDTH = camera.m_film->m_width;
        const unsigned int PIXEL_HEIGHT = camera.m_film->m_height;
        const unsigned int NUM_ALL_PIXEL = PIXEL_WIDTH * PIXEL_HEIGHT;

		HitRecord hitRec, shadowHitRec;
		Ray viewRay;
		for (unsigned int i = 0; i < PIXEL_WIDTH; ++i)
		{
			for (unsigned int j = 0; j < PIXEL_HEIGHT; ++j)
			{
                // print the progress
                const unsigned int CURR_COUNT_PIXE = j + i * PIXEL_HEIGHT + 1;
                if (CURR_COUNT_PIXE % 2500 == 0)
                {
                    ShowProgress(CURR_COUNT_PIXE * 1.0f / NUM_ALL_PIXEL);
                }

                //BREAK_POINT_IF(i == 90 && j == 511 - 298);

				viewRay = camera.GetRay(i, j);

                //BREAK_POINT_IF(i == 93 && j == 511 - 76);
                camera.IncomeLight(i, j, scene.RayColor(viewRay, 0.0f, 1000.0f));
			}
		}

		camera.m_film->SaveTo("OutputTestImage\\RenderInsideBox\\InsideBox29.png");

        ImageWindow imgWnd(camera.m_film.get(), L"InsideBox29.png");
        imgWnd.BlockShow();

		errorLogger += LetUserCheckJudge(
			"check \".\\OutputTestImage\\RenderInsideBox\\....png\"\n"
			"you should have seen a sphere with some color, the background is dark green.");

	TEST_UNIT_END;
#pragma endregion

TEST_MODULE_END
