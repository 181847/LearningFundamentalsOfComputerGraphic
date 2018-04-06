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
#include "../CommonClasses/RGBA.h"
#include "../CommonClasses/Image.h"
#include "../CommonClasses/OrthographicCamera.h"
#include "../CommonClasses/Ray.h"
#include "../CommonClasses/Sphere.h"
#include "../CommonClasses/PerspectiveCamera.h"
#include "../CommonClasses/Triangle.h"
#include "../CommonClasses/Scene.h"
#include "../CommonClasses/Polygon.h"
#include "../CommonClasses/ColorTemplate.h"
#pragma comment(lib, "CommonClasses.lib")




RandomTool::MTRandom globalMtr;
const unsigned int G_MAX_INT = 1000;

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

#pragma region Try the testUnit
	TEST_UNIT_START("a test always success")
	TEST_UNIT_END;
#pragma endregion

#pragma region test vector2
	TEST_UNIT_START("test vector2")
		CommonClass::vector2 v2;
		errorLogger.LogIfNotEq(v2.m_x, 0.0f);
		errorLogger.LogIfNotEq(v2.m_y, 0.0f);
	TEST_UNIT_END;
#pragma endregion

#pragma region test vector2 +/ -/ *
	TEST_UNIT_START("test vector2 +/ -/ *")

		RandomTool::MTRandom mtr;
		const unsigned int MAX_RAND_INT = 600;
		
		for (int i = 0; i < 200; ++i)
		{
			const Types::F32	comf1(1.0f * mtr.Random(MAX_RAND_INT) / (mtr.Random(MAX_RAND_INT) + 1)), 
								comf2(1.0f * mtr.Random(MAX_RAND_INT) / (mtr.Random(MAX_RAND_INT) + 1)),
								comf3(1.0f * mtr.Random(MAX_RAND_INT) / (mtr.Random(MAX_RAND_INT) + 1)),
								comf4(1.0f * mtr.Random(MAX_RAND_INT) / (mtr.Random(MAX_RAND_INT) + 1));
			CommonClass::vector2 cmp1(comf1, comf2);
			CommonClass::vector2 cmp2(comf3, comf4);

			// cmp1 + cmp2
			errorLogger.LogIfNotEq(
				cmp1 + cmp2,
				CommonClass::vector2(comf1 + comf3, comf2 + comf4));

			// cmp1 - cmp2
			errorLogger.LogIfNotEq(
				cmp1 - cmp2,
				CommonClass::vector2(comf1 - comf3, comf2 - comf4));

			// cmp1 * cmp2
			errorLogger.LogIfNotEq(
				cmp1 * cmp2,
				(comf1 * comf3 + comf2 * comf4));

			// cmp1 * cmp2
			errorLogger.LogIfNotEq(
				CommonClass::dotProd(cmp1, cmp2),
				(comf1 * comf3 + comf2 * comf4));
		}
	TEST_UNIT_END;
#pragma endregion

#pragma region test vector3 +/ -/ *
	TEST_UNIT_START("test vector3 +/ -/ */ X")
		using namespace CommonClass;
		RandomTool::MTRandom mtr;
		const unsigned int MAX_RAND_INT = 600;
		
		for (int i = 0; i < 200; ++i)
		{
			const Types::F32	comf1(1.0f * mtr.Random(MAX_RAND_INT) / (mtr.Random(MAX_RAND_INT) + 1)), 
								comf2(1.0f * mtr.Random(MAX_RAND_INT) / (mtr.Random(MAX_RAND_INT) + 1)),
								comf3(1.0f * mtr.Random(MAX_RAND_INT) / (mtr.Random(MAX_RAND_INT) + 1)),
								comf4(1.0f * mtr.Random(MAX_RAND_INT) / (mtr.Random(MAX_RAND_INT) + 1)),
								comf5(1.0f * mtr.Random(MAX_RAND_INT) / (mtr.Random(MAX_RAND_INT) + 1)),
								comf6(1.0f * mtr.Random(MAX_RAND_INT) / (mtr.Random(MAX_RAND_INT) + 1));
			CommonClass::vector3 cmp1(comf1, comf2, comf3);
			CommonClass::vector3 cmp2(comf4, comf5, comf6);

			// cmp1 + cmp2
			errorLogger.LogIfNotEq(
				cmp1 + cmp2,
				CommonClass::vector3(comf1 + comf4, comf2 + comf5, comf3 + comf6));

			// cmp1 - cmp2
			errorLogger.LogIfNotEq(
				cmp1 - cmp2,
				CommonClass::vector3(comf1 - comf4, comf2 - comf5, comf3 - comf6));
			
			// cmp1 * cmp2
			errorLogger.LogIfNotEq(
				cmp1 * cmp2,
				(comf1 * comf4 + comf2 * comf5 + comf3 * comf6));

			// cmp1 * cmp2
			errorLogger.LogIfNotEq(
				CommonClass::dotProd(cmp1, cmp2),
				(comf1 * comf4 + comf2 * comf5 + comf3 * comf6));

			// cmp1 X cmp2
			errorLogger.LogIfNotEq(
				CommonClass::crossProd(cmp1, cmp2),
				CommonClass::vector3(
					comf2 * comf6 - comf5 * comf3,
					comf4 * comf3 - comf1 * comf6,
					comf1 * comf5 - comf4 * comf2));

			// vector3 * float
			Types::F32 randomFloat = mtr.Random();
			errorLogger.LogIfNotEq(
				cmp1 * randomFloat,
				vector3(
					comf1 * randomFloat,
					comf2 * randomFloat,
					comf3 * randomFloat)); 
			errorLogger.LogIfNotEq(
				randomFloat * cmp2,
				vector3(
					comf4 * randomFloat,
					comf5 * randomFloat,
					comf6 * randomFloat));

			// - vector3
			errorLogger.LogIfNotEq(
				-cmp1, 
				vector3(-comf1, -comf2, -comf3));
		}
	TEST_UNIT_END;
#pragma endregion

#pragma region test RGBA +/ -/ rgbMulti/ *(scalar)/ /(scalar)
	TEST_UNIT_START("test RGBA +/ -/ */ *(scalar)/ /(scalar)")
		RandomTool::MTRandom mtr;
		const unsigned int MAX_RAND_INT = 64;

		/*!
			\brief clamp the channel to [0.0f, 1.0f]
		*/
		auto clampChannel = [](Types::F32 ch)-> Types::F32
		{
			if (ch <= 0.0f)
			{
				return 0.0f;
			}
			else if (ch >= 1.0f)
			{
				return 1.0f;
			}
			else
			{
				return ch;
			}
		};

		for (int i = 0; i < 200; ++i)
		{
			/*!
				make up the RGBA channel
			*/
			const Types::F32	comu1(mtr.Random()),
								comu2(mtr.Random()),
								comu3(mtr.Random()),
								comu4(mtr.Random()),
								comu5(mtr.Random()),
								comu6(mtr.Random()),
								comu7(mtr.Random()),
								comu8(mtr.Random());

			/*!
				scale the RGB with float.
			*/
			const Types::F32	comf1(mtr.Random() * mtr.Random(MAX_RAND_INT)),
								comf2(mtr.Random() * mtr.Random(MAX_RAND_INT));

			CommonClass::RGBA cmp1(comu1, comu2, comu3, comu4);
			CommonClass::RGBA cmp2(comu5, comu6, comu7, comu8);

			/*!
				\brief recover the cmp1's value
			*/
			auto recoverCMP1 = [&cmp1, comu1, comu2, comu3, comu4]()
			{
				cmp1 = CommonClass::RGBA(comu1, comu2, comu3, comu4);
			};

			// default constructor can ignore alpha channel, the alpha of the pixel will be set to max(opaque).
			errorLogger.LogIfNotEq(
				CommonClass::RGBA(comu1, comu2, comu3),
				CommonClass::RGBA(comu1, comu2, comu3, CommonClass::RGBA::ALPHA_CHANNEL_OPAQUE));

			// assign rgb only
			cmp1.AssignRGB(cmp2);
			errorLogger.LogIfNotEq(
				cmp1,
				CommonClass::RGBA(comu5, comu6, comu7, comu4));
			recoverCMP1();

			// cmp1 + cmp2 rgb only
			cmp1.AddRGB(cmp2);
			errorLogger.LogIfNotEq( 
				cmp1,
				CommonClass::RGBA(clampChannel(comu1 + comu5), clampChannel(comu2 + comu6), clampChannel(comu3 + comu7), comu4));
			recoverCMP1();

			// cmp1 - cmp2 rgb only
			cmp1.SubRGB(cmp2),
			errorLogger.LogIfNotEq(
				cmp1,
				CommonClass::RGBA(clampChannel(comu1 - comu5), clampChannel(comu2 - comu6), clampChannel(comu3 - comu7), comu4));
			recoverCMP1();
			
			// cmp1 * cmp2 rgb only
			cmp1.MulRGB(cmp2),
			errorLogger.LogIfNotEq(
				cmp1,
				CommonClass::RGBA(clampChannel(comu1 * comu5), clampChannel(comu2 * comu6), clampChannel(comu3 * comu7), comu4));
			recoverCMP1();

			// cmp1 * scalar rgb only
			cmp1.MulRGB(comf1),
			errorLogger.LogIfNotEq(
				cmp1,
				CommonClass::RGBA(clampChannel(comu1 * comf1), clampChannel(comu2 * comf1), clampChannel(comu3 * comf1), comu4));
			recoverCMP1();

			// cmp1 / scalar rgb only
			Types::F32 reciprocalComf2 = 1.0f / comf2;
			cmp1.DivRGB(comf2),
			errorLogger.LogIfNotEq(
				cmp1,
				CommonClass::RGBA(clampChannel(comu1 * reciprocalComf2), clampChannel(comu2 * reciprocalComf2), clampChannel(comu3 * reciprocalComf2), comu4));
			recoverCMP1();

			// alpha add
			cmp1.AddAlpha(comu8);
			errorLogger.LogIfNotEq(
				cmp1.m_a,
				clampChannel(comu4 + comu8));
			recoverCMP1();

			// alpha sub
			cmp1.SubAlpha(comu8);
			errorLogger.LogIfNotEq(
				cmp1.m_a,
				clampChannel(comu4 - comu8));
			recoverCMP1();

			// alpha mul float
			cmp1.MulAlpha(comf1);
			errorLogger.LogIfNotEq(
				cmp1.m_a,
				clampChannel((comu4 * comf1)));
			recoverCMP1();

			// alpha div float
			cmp1.DivAlpha(comf2);
			errorLogger.LogIfNotEq(
				cmp1.m_a,
				clampChannel((comu4 / comf2)));
			recoverCMP1();


		}
	TEST_UNIT_END;
#pragma endregion

#pragma region test Image
	TEST_UNIT_START("test Image")
		const Types::U32 WIDTH(512), HEIGHT(512);
		CommonClass::Image testImage(WIDTH, HEIGHT);

		CommonClass::RGBA pixelSetter;
        TRGBA pixelSetterT;

		pixelSetter.SetChannel<CommonClass::RGBA::B>(0.5f);
        pixelSetterT.SetChannel<TRGBA::B>(0.5f);

		for (int x = 0; x < WIDTH; ++x)
		{
			for (int y = 0; y < HEIGHT; ++y)
			{
				//pixelSetter.SetChannel<CommonClass::RGBA::R>  (         x / 512.0f);
				//pixelSetter.SetChannel<CommonClass::RGBA::G>(         y / 512.0f);
				//pixelSetter.SetChannel<CommonClass::RGBA::A>( ( x + y ) / 1024.0f);

                pixelSetterT.SetChannel<TRGBA::R>(      x / 512.0f);
                pixelSetterT.SetChannel<TRGBA::G>(      y / 512.0f);
                pixelSetterT.SetChannel<TRGBA::A>((x + y) / 1024.0f);

				testImage.SetPixel(x, y, pixelSetterT);
			}
		}

		testImage.SaveTo("OutputTestImage\\ThisImageIsForTest.png");

		errorLogger += LetUserCheckJudge(
				"check \".\\OutputTestImage\\ThisImageIsForTest.png\"\n"
				"with the file in the same folder \"CheckWithMe.png\"\n"
				"if they are the SAME, means RIGHT.");

	TEST_UNIT_END;
#pragma endregion

#pragma region vector3 copy constructor
	TEST_UNIT_START("vector3 copy constructor")
		using namespace CommonClass;
		vector3 m3(1.0f, 2.0f, 3.0f);
		vector3 m_toBeError(1.1f, 2.2f, 3.3f);
		vector3 m_copy(m3);

		// construct success
		errorLogger.LogIfFalse(AlmostEqual(m3, m_copy));

		// ensure function AlmostEqual is right
		errorLogger.LogIfTrue(AlmostEqual(m3, m_toBeError));
	TEST_UNIT_END;
#pragma endregion

#pragma region vector3 normalize
	TEST_UNIT_START("vector3 normalize")
		using namespace CommonClass;
		using namespace MathTool;
		
		globalMtr.SetRandomSeed(5);

		

		for (int i = 0; i < 20; ++i)
		{
			vector3 v1(GetRandomVector3(false));

			v1 = Normalize(v1);

			errorLogger.LogIfFalse(almost_equal(Length(v1), 1.0f, 8));

			v1.m_x *= 2.0f;
			v1.m_y *= 2.0f;
			v1.m_z *= 3.0f;

			// ensurance, make vector's length not equal to unit.
			errorLogger.LogIfTrue(almost_equal(Length(v1), 1.0f, 8));
		}
	TEST_UNIT_END;
#pragma endregion

#pragma region film capture light test
	TEST_UNIT_START("film capture light test")
		using namespace CommonClass;
		const Types::U32 WIDTH(512), HEIGHT(512);
		
		Film tfilm(WIDTH, HEIGHT, -1.0f, 1.0f, -1.0f, 1.0f);

		RGBA pixelSetter;
        TRGBA pixelSetterT;
		pixelSetter.SetChannel<RGBA::B>(0.5f);
        pixelSetterT.SetChannel<TRGBA::B>(0.5f);

		for (int x = 0; x < WIDTH; ++x)
		{
			for (int y = 0; y < HEIGHT; ++y)
			{
				/*pixelSetter.SetChannel<CommonClass::RGBA::R>(x / 512.0f);
				pixelSetter.SetChannel<CommonClass::RGBA::G>(y / 512.0f);
				pixelSetter.SetChannel<CommonClass::RGBA::A>((x + y) / 1024.0f);*/

                
                pixelSetterT.SetChannel<TRGBA::R>(      x / 512.0f);
                pixelSetterT.SetChannel<TRGBA::G>(      y / 512.0f);
                pixelSetterT.SetChannel<TRGBA::A>((x + y) / 1024.0f);

				tfilm.SetPixel(x, y, pixelSetterT);
			}
		}


		tfilm.SaveTo("OutputTestImage\\ThisImageIsForFilm.png");


		errorLogger += LetUserCheckJudge(
			"check \".\\OutputTestImage\\ThisImageIsForFilm.png\"\n"
			"with the file in the same folder \"CheckWithMe.png\"\n"
			"if they are the SAME, means RIGHT.");
		
	TEST_UNIT_END;
#pragma endregion

#pragma region check cameras basis is constructed correctly
	TEST_UNIT_START("check camera's basis is constructed correctly")
		using namespace CommonClass;
		using namespace MathTool;
		
		globalMtr.SetRandomSeed(5);

		/*!
			\brief help to check the vector's length is almost 1 unit.
		*/
		auto lengthAlmostEqual_1_Unit = [](const vector3& a)->bool
		{
			return almost_equal(Length(a), 1.0f, 8);
		};

		/*!
			\brief using cross product check two vectors is almost perpendicular to each other.
			Warning: the result of cross product by have a huge differ from 0.0f,
					so the almost_equal will not work properly,
					here we just specify a range (-1e-7f, 1e-7f).
		*/
		auto almostPerpendicular = [](const vector3& a, const vector3& b)->bool
		{
			Types::F32 dpValue = dotProd(a, b);
			//std::printf("## check perpendicular vector3 %f.\n", dpValue);

			return (-1e-7f) < dpValue && dpValue < (1e-7f);
		};

		for (int i = 0; i < 20; ++i)
		{
			vector3 origin = GetRandomVector3(true);
			vector3 target = GetRandomVector3(false);
			vector3 dummyLookUp = Normalize(GetRandomVector3(false));

			// if the position and target is overlaped, skip this turn.
			if (origin == target)
			{
				continue;
			}

			OrthographicCamera orthoCamera(origin, target, dummyLookUp);
			
			// check basis is all unit vector
			errorLogger.LogIfFalse(lengthAlmostEqual_1_Unit(orthoCamera.m_w));
			errorLogger.LogIfFalse(lengthAlmostEqual_1_Unit(orthoCamera.m_u));
			errorLogger.LogIfFalse(lengthAlmostEqual_1_Unit(orthoCamera.m_v));

			// check three basises is perpendicular to each other
			errorLogger.LogIfFalse(almostPerpendicular(orthoCamera.m_w, orthoCamera.m_u));
			errorLogger.LogIfFalse(almostPerpendicular(orthoCamera.m_w, orthoCamera.m_v));
			errorLogger.LogIfFalse(almostPerpendicular(orthoCamera.m_u, orthoCamera.m_v));
		}
		
	TEST_UNIT_END;
#pragma endregion

#pragma region test capmera capture light
	TEST_UNIT_START("test capmera capture light")
		using namespace CommonClass;
		using namespace MathTool;
		
		globalMtr.SetRandomSeed(5);

		/*!
			\brief help to check the vector's length is almost 1 unit.
		*/
		auto lengthAlmostEqual_1_Unit = [](const vector3& a)->bool
		{
			return almost_equal(Length(a), 1.0f, 8);
		};

		/*!
			\brief using cross product check two vectors is almost perpendicular to each other.
			Warning: the result of cross product by have a huge differ from 0.0f,
					so the almost_equal will not work properly,
					here we just specify a range (-1e-7f, 1e-7f).
		*/
		auto almostPerpendicular = [](const vector3& a, const vector3& b)->bool
		{
			Types::F32 dpValue = dotProd(a, b);
			//std::printf("## check perpendicular vector3 %f.\n", dpValue);

			return (-1e-7f) < dpValue && dpValue < (1e-7f);
		};

		vector3 origin(0.0f, 0.0f, 0.0f);
		vector3 target(1.0f, 0.0f, 0.0f);
		vector3 dummyLookUp(0.0f, 1.0f, 0.0f);

		const Types::U32 WIDTH(512), HEIGHT(512);
		OrthographicCamera orthoCamera(origin, target, dummyLookUp);
		orthoCamera.SetFilm(std::make_unique<Film>(WIDTH, HEIGHT, -1.0f, 1.0f, -1.0f, 1.0f));

		RGBA pixelSetter;
		pixelSetter.SetChannel<RGBA::B>(0.5f);

		for (int x = 0; x < WIDTH; ++x)
		{
			for (int y = 0; y < HEIGHT; ++y)
			{
				pixelSetter.SetChannel<CommonClass::RGBA::R>(x / 512.0f);
				pixelSetter.SetChannel<CommonClass::RGBA::G>(y / 512.0f);
				pixelSetter.SetChannel<CommonClass::RGBA::A>((x + y) / 1024.0f);

				orthoCamera.IncomeLight(x, y, pixelSetter);
			}
		}

		orthoCamera.m_film->SaveTo("OutputTestImage\\ThisImageIsForOrthoCamera.png");

		errorLogger += LetUserCheckJudge(
			"check \".\\OutputTestImage\\ThisImageIsForOrthoCamera.png\"\n"
			"with the file in the same folder \"CheckWithMe.png\"\n"
			"if they are the SAME, means RIGHT.");
		
	TEST_UNIT_END;
#pragma endregion

#pragma region test ray construct
	TEST_UNIT_START("test ray construct")
		using namespace CommonClass;
		
		for (int i = 0; i < 20; ++i)
		{
			Ray tray(GetRandomVector3(), GetRandomVector3(false));

			errorLogger.LogIfFalse(MathTool::almost_equal(Length(tray.m_direction), 1.0f, 8));
		}
	TEST_UNIT_END;
#pragma endregion

#pragma region get ray from orthographic camera
	TEST_UNIT_START("get ray from orthographic camera")
		using namespace CommonClass;
		using namespace MathTool;

		vector3 camOrigin = GetRandomVector3();
		vector3 camLookAt = GetRandomVector3();
		vector3 camLookUp = Normalize(GetRandomVector3(false));
		OrthographicCamera orthoCamera(camOrigin, camLookAt, camLookUp);
		orthoCamera.SetFilm(std::make_unique<Film>(
			UserConfig::COMMON_PIXEL_WIDTH, UserConfig::COMMON_PIXEL_HEIGHT,
			UserConfig::COMMON_RENDER_LEFT, UserConfig::COMMON_RENDER_RIGHT, UserConfig::COMMON_RENDER_BOTTOM, UserConfig::COMMON_RENDER_TOP));

		for (unsigned int i = 0; i < orthoCamera.m_film->m_width; ++i)
		{

			for (unsigned int j = 0; j < orthoCamera.m_film->m_height; ++j)
			{
				Ray ray = orthoCamera.GetRay(i, j);
				
				errorLogger.LogIfFalse(AlmostPerpendicular(ray.m_origin - orthoCamera.m_origin, ray.m_direction, -1e-4f, +1e-4f));
				errorLogger.LogIfFalse(AlmostEqual(ray.m_direction, -orthoCamera.m_w));
			}
		}


	TEST_UNIT_END;
#pragma endregion

#pragma region test camere pixel location
	TEST_UNIT_START("test camere pixel location")
		using namespace CommonClass;
		vector3 origin(0.0f, 0.0f, 0.0f);
		vector3 target(1.0f, 0.0f, 0.0f);
		vector3 dummyLookUp(0.0f, 1.0f, 0.0f);

		const Types::U32 WIDTH(512), HEIGHT(512);
		OrthographicCamera orthoCamera(origin, target, dummyLookUp);
		orthoCamera.SetFilm(std::make_unique<Film>(WIDTH, HEIGHT, -1.0f, 1.0f, -1.0f, 1.0f));

		RGBA pixelSetter;
		RGBA black(0.0f, 0.0f, 0.0f);
		RGBA white(1.0f, 1.0f, 1.0f);
		RGBA red(1.0f, 0.0f, 0.0f);
		RGBA green(0.0f, 1.0f, 0.0f);
		RGBA blue(0.0f, 0.0f, 1.0f);


		// background as black
		for (int x = 0; x < WIDTH; ++x)
		{
			for (int y = 0; y < HEIGHT; ++y)
			{
				orthoCamera.IncomeLight(x, y, black);
			}
		}
		// counterclockwise from left top corner: 
		// white ------------------------> red 
		//  A                               |
		//  |                               |
		//  |           black               |
		//  |                               |
		//  |                               |
		//  |                               V
		//  blue <------------------------ green
		orthoCamera.IncomeLight(0,			HEIGHT - 1, white);
		orthoCamera.IncomeLight(WIDTH - 1,	HEIGHT - 1, red);
		orthoCamera.IncomeLight(WIDTH - 1,	0,			green);
		orthoCamera.IncomeLight(0,			0,			blue);

		orthoCamera.m_film->SaveTo("OutputTestImage\\ThisImageIsForOrthoCameraPixelLoc.png");



		errorLogger += LetUserCheckJudge(
			"check \".\\OutputTestImage\\ThisImageIsForOrthoCamera.png\"\n"
			"four different pixel in the corner (very small), it should look like:\n"
			"white ------------------------> red \n"
			" A                               | \n"
			" |                               | \n"
			" |           black               | \n"
			" |                               | \n"
			" |                               | \n"
			" |                               V \n"
			" blue <------------------------ green \n"
			"if they are the SAME, means RIGHT.");
	TEST_UNIT_END;
#pragma endregion

#pragma region check orhographic camera and sphere-ray collision
	TEST_UNIT_START("check orhographic camera and sphere-ray collision")
		using namespace CommonClass;

		Sphere tsph(vector3(-1.0f, 2.0f, 2.0f), 0.9f);

		RGBA hitPixel(1.0f, 1.0f, 1.0f);
		RGBA missSphPixel(0.0f, 0.0f, 0.0f);
		RGBA missAABBPixel(0.0f, 0.5f, 0.0f);

		vector3 camPosition = vector3(2.0f, 1.0f, 3.0f);
		vector3 camTarget = vector3(0.0f, 0.0f, 0.0f);
		vector3 camLookUp = vector3(0.0f, 1.0f, 0.0f);

		OrthographicCamera orthoCamera(camPosition, camTarget, camLookUp);
		orthoCamera.SetFilm(std::make_unique<Film>(
			UserConfig::COMMON_PIXEL_WIDTH, UserConfig::COMMON_PIXEL_HEIGHT,
			UserConfig::COMMON_RENDER_LEFT, UserConfig::COMMON_RENDER_RIGHT, 
			UserConfig::COMMON_RENDER_BOTTOM, UserConfig::COMMON_RENDER_TOP));

		HitRecord hitRec;

		for (unsigned int i = 0; i < orthoCamera.m_film->m_width; ++i)
		{
			for (unsigned int j = 0; j < orthoCamera.m_film->m_height; ++j)
			{
				Ray ray = orthoCamera.GetRay(i, j);
				
				//BREAK_POINT_IF(i == 400 && j == 256);

				// try bounding box first
				if (tsph.BoundingBox().Hit(ray, 0.0f, 1000.0f, &hitRec))
				{
					
					// try sphere
					if (tsph.Hit(ray, 0.0f, 1000.0f, &hitRec))
					{
						hitPixel.SetChannel<RGBA::R>((hitRec.m_hitT / 3.8f));
						hitPixel.SetChannel<RGBA::G>((hitRec.m_hitT / 3.8f));
						hitPixel.SetChannel<RGBA::B>((hitRec.m_hitT / 3.8f));
						
						orthoCamera.IncomeLight(i, j, hitPixel);
					}
					else
					{
						orthoCamera.IncomeLight(i, j, missSphPixel);
					}
				}
				else
				{
					orthoCamera.IncomeLight(i, j, missAABBPixel);
				}
			}
		}

		orthoCamera.m_film->SaveTo("OutputTestImage\\ThisImageIsForOrthoCameraRenderSphere.png");

		errorLogger += LetUserCheckJudge(
			"check \".\\OutputTestImage\\ThisImageIsForOrthoCamera.png\"\n"
			"you should have seen sphere is wrapped in a black box area(3D), back ground is green.");

	TEST_UNIT_END;
#pragma endregion

#pragma region try rgba constant colors
	TEST_UNIT_START("try rgba constant colors")
		using namespace CommonClass;
		using namespace Types;

		const U32 WIDTH(512), HEIGHT(512);
		Image img(WIDTH, HEIGHT);

		std::array<RGBA, 8> colors = {
			RGBA::RED,
			RGBA::GREEN,
			RGBA::BLUE,
			RGBA::YELLOW,
			RGBA::CYAN,
			RGBA::MAGENTA,
			RGBA::WHITE,
			RGBA::BLACK
		};

		int i = 0;

		for (RGBA& oneColor : colors)
		{
			for (int count = 0; count < 64; ++count)
			{
				for (int row = 0; row < HEIGHT; ++row)
				{
					img.SetPixel(i + count, row, oneColor);
				}
			}
			i += 64;
		}

		img.SaveTo("OutputTestImage\\ConstantColorCheck.png");

		errorLogger += LetUserCheckJudge(
			"check OutputTestImage\\ConstantColorCheck.png\n"
			"it should look like 8 color strip:\n"
			"red -> green -> blue -> black -> yellow -> cyan -> manenta -> white -> black");

	TEST_UNIT_END;
#pragma endregion

#pragma region check perspective camera and sphere-ray collision
	TEST_UNIT_START("check perspective camera and sphere-ray collision")
		using namespace CommonClass;

		Sphere tsph(vector3(0.0f, 0.0f, 0.0f), 1.0f);

		RGBA hitPixel(1.0f, 1.0f, 1.0f);
		RGBA missSphPixel(0.0f, 0.0f, 0.0f);
		RGBA missAABBPixel(0.0f, 0.5f, 0.0f);

		vector3 camPosition = vector3(1.8f, 1.8f, 1.8f);
		vector3 camTarget = vector3(0.0f, 0.0f, 0.0f);
		vector3 camLookUp = vector3(0.0f, 1.0f, 0.0f);

		Types::F32 focalLength = 0.5f;

		PerspectiveCamera perspectCamera(focalLength, camPosition, camTarget, camLookUp);
		perspectCamera.SetFilm(std::make_unique<Film>(
			UserConfig::COMMON_PIXEL_WIDTH, UserConfig::COMMON_PIXEL_HEIGHT,
			-0.5f, +0.5f,
			-0.5f, +0.5f));

		HitRecord hitRec;

		for (unsigned int i = 0; i < perspectCamera.m_film->m_width; ++i)
		{
			for (unsigned int j = 0; j < perspectCamera.m_film->m_height; ++j)
			{
				Ray ray = perspectCamera.GetRay(i, j);
				
				//BREAK_POINT_IF(i == 256 && j == 256);

				// try bounding box first
				if (tsph.BoundingBox().Hit(ray, 0.0f, 1000.0f, &hitRec))
				{
					
					// try sphere
					if (tsph.Hit(ray, 0.0f, 1000.0f, &hitRec))
					{
						hitPixel.SetChannel<RGBA::R>((hitRec.m_hitT / 3.8f));
						hitPixel.SetChannel<RGBA::G>((hitRec.m_hitT / 3.8f));
						hitPixel.SetChannel<RGBA::B>((hitRec.m_hitT / 3.8f));
						
						perspectCamera.IncomeLight(i, j, hitPixel);
					}
					else
					{
						perspectCamera.IncomeLight(i, j, missSphPixel);
					}
				}
				else
				{
					perspectCamera.IncomeLight(i, j, missAABBPixel);
				}
			}
		}

		perspectCamera.m_film->SaveTo("OutputTestImage\\ThisImageIsForPerspectiveCameraRenderSphere.png");

		errorLogger += LetUserCheckJudge(
			"check \".\\OutputTestImage\\ThisImageIsForPerspectiveCameraRenderSphere.png\"\n"
			"you should have seen a sphere is wrapped in a black box area(3D perspective), back ground is green.");

	TEST_UNIT_END;
#pragma endregion

#pragma region test std::chrono
	TEST_UNIT_START("test std::chrono")
		using namespace std::chrono;
		
		system_clock::time_point tp_epoch;
		time_point<system_clock, duration<int>> tp_seconds(duration<int>(1));

		system_clock::time_point tp(tp_seconds);

		auto start = system_clock::now();

		//std::cout	<< "1 second since system_clock epoch = "
		//			<< tp.time_since_epoch().count()
		//			<< " system_clock periods." << std::endl;

		auto end = system_clock::now();

		auto durat = duration_cast<milliseconds>(end - start);

		//std::cout << "time for previous printing is milliseconds: "
		//	<< durat.count() << std::endl;

		//std::time_t tt = system_clock::to_time_t(tp);
		//std::cout << "time_point tp is: " << std::ctime(&tt);

	TEST_UNIT_END;
#pragma endregion

#pragma region try ray-triangle hit
	TEST_UNIT_START("try ray-triangle hit")
		using namespace CommonClass;

		Triangle tri(
			vector3(1.0f, 0.0f, 0.0f), 
			vector3(-1.0f, 0.0f, 0.0f), 
			vector3(0.0f, 2.0f, 1.0f));

		RGBA hitPixel(1.0f, 1.0f, 1.0f);
		RGBA missSphPixel(0.0f, 0.0f, 0.0f);
		RGBA missAABBPixel(0.0f, 0.5f, 0.0f);
		RGBA positivePixel(RGBA::RED);
		RGBA negativePixel(RGBA::GREEN);

		vector3 camPosition = vector3(3.0f, 3.0f, 3.0f);
		vector3 camTarget = vector3(0.0f, 0.0f, 0.0f);
		vector3 camLookUp = vector3(0.0f, 1.0f, 0.0f);

		Types::F32 focalLength = 0.5f;
		PerspectiveCamera camera(focalLength, camPosition, camTarget, camLookUp);
		camera.SetFilm(std::make_unique<Film>(
			UserConfig::COMMON_PIXEL_WIDTH, UserConfig::COMMON_PIXEL_HEIGHT,
			-0.5f, +0.5f,
			-0.5f, +0.5f));

		HitRecord hitRec;

		for (unsigned int i = 0; i < camera.m_film->m_width; ++i)
		{
			for (unsigned int j = 0; j < camera.m_film->m_height; ++j)
			{
				Ray ray = camera.GetRay(i, j);
				
				//BREAK_POINT_IF(i == 256 && j == 256);

				// try bounding box first
				if (tri.BoundingBox().Hit(ray, 0.0f, 1000.0f, &hitRec))
				{
					
					// try triangle
					if (tri.Hit(ray, 0.0f, 1000.0f, &hitRec))
					{
						//hitPixel.SetChannel<RGBA::R>((hitRec.m_hitT / 3.8f));
						//hitPixel.SetChannel<RGBA::G>((hitRec.m_hitT / 3.8f));
						//hitPixel.SetChannel<RGBA::B>((hitRec.m_hitT / 3.8f));
						
						//orthoCamera.IncomeLight(i, j, hitPixel);

						if ( ! hitRec.m_isBackFace)
						{
							camera.IncomeLight(i, j, positivePixel);
						}
						else
						{
							//PUT_BREAK_POINT;
							camera.IncomeLight(i, j, negativePixel);
						}
					}
					else
					{
						camera.IncomeLight(i, j, missSphPixel);
					}
				}
				else
				{
					camera.IncomeLight(i, j, missAABBPixel);
				}
			}
		}

		camera.m_film->SaveTo("OutputTestImage\\ThisImageIsForOrthoCameraRenderTriangle.png");

		errorLogger += LetUserCheckJudge(
			"check \".\\OutputTestImage\\ThisImageIsForOrthoCameraRenderTriangle.png\"\n"
			"you should have seen a triangle in the center.");
	TEST_UNIT_END;
#pragma endregion

#pragma region hit with scene
	TEST_UNIT_START("hit with scene")
		using namespace CommonClass;
		
		Scene testScene;

		auto sph1 = std::make_unique<Sphere>(vector3(0.0f, 0.0f, 0.0f), 1.0f);
		auto sph2 = std::make_unique<Sphere>(vector3(3.0f, 2.0f, 1.0f), 0.6f);

		const Types::F32 borderLength = 20.0f;
		auto tri1 = std::make_unique<Triangle>(
			vector3(-borderLength, -2.0f, -borderLength),
			vector3(-borderLength, -2.0f, borderLength),
			vector3(+borderLength, -2.0f, borderLength));
		auto tri2 = std::make_unique<Triangle>(
			vector3(-borderLength, -2.0f, -borderLength),
			vector3(+borderLength, -2.0f, +borderLength),
			vector3(+borderLength, -2.0f, -borderLength));

		auto poly = std::make_unique<Polygon>(
			vector3(-borderLength, -borderLength, -2.0f),
			vector3(-borderLength, borderLength, -2.0f),
			vector3(+borderLength, borderLength, -2.0f)
			);

		testScene.Add(std::move(sph1));
		testScene.Add(std::move(sph2));
		testScene.Add(std::move(tri1));
		testScene.Add(std::move(tri2));
		testScene.Add(std::move(poly));

		RGBA backgroundColor(RGBA::BLACK);
		RGBA hitColor(RGBA::RED);

		vector3 camPosition = vector3(3.0f, 4.0f, 2.0f);
		vector3 camTarget = vector3(0.0f, 0.0f, 0.0f);
		vector3 camLookUp = vector3(0.0f, 1.0f, 0.0f);

		Types::F32 focalLength = 0.5f;
		PerspectiveCamera camera(focalLength, camPosition, camTarget, camLookUp);
		camera.SetFilm(std::make_unique<Film>(
			UserConfig::COMMON_PIXEL_WIDTH, UserConfig::COMMON_PIXEL_HEIGHT,
			-0.5f, +0.5f,
			-0.5f, +0.5f));

		HitRecord hitRec;

		for (unsigned int i = 0; i < camera.m_film->m_width; ++i)
		{
			for (unsigned int j = 0; j < camera.m_film->m_height; ++j)
			{
				Ray ray = camera.GetRay(i, j);

				//BREAK_POINT_IF(i == 256 && j == 256);

				// try triangle
				if (testScene.Hit(ray, 0.0f, 1000.0f, &hitRec))
				{
					//hitPixel.SetChannel<RGBA::R>((hitRec.m_hitT / 3.8f));
					//hitPixel.SetChannel<RGBA::G>((hitRec.m_hitT / 3.8f));
					//hitPixel.SetChannel<RGBA::B>((hitRec.m_hitT / 3.8f));
					hitColor.SetChannel<RGBA::A>(hitRec.m_hitT / 8.0f);

					camera.IncomeLight(i, j, hitColor);
				}
				else
				{
					camera.IncomeLight(i, j, backgroundColor);
				}
			}
		}

		camera.m_film->SaveTo("OutputTestImage\\ThisImageIsForSceneHit.png");

		errorLogger += LetUserCheckJudge(
			"check \".\\OutputTestImage\\ThisImageIsForSceneHit.png\"\n"
			"you should have seen multiple object in the scene, the background color is cyan.");
	TEST_UNIT_END;
#pragma endregion

#pragma region hit with polygon in scene
	TEST_UNIT_START("hit with polygon in scene")
		using namespace CommonClass;

		const Types::F32 borderLength = 3.0f;

		auto poly = std::make_unique<Polygon>(
			vector3(-borderLength, -borderLength, +4.0f),
			vector3(-borderLength, borderLength, -4.0f),
			vector3(+borderLength, borderLength, -4.0f)
			);

		poly->AddPoint(vector3(borderLength * 0.9f, -borderLength * 3.0f, -2.0f));
		poly->AddPoint(vector3(borderLength * 0.1f, -borderLength * 4.0f, -2.0f));

		RGBA backgroundColor(RGBA::GREEN);
		backgroundColor.MulRGB(0.5f); // make color darker
		RGBA hitBBoxColor(RGBA::BLACK);
		RGBA hitColor(RGBA::RED);

		vector3 camPosition = vector3(-4.0f, 8.0f, 8.0f);
		vector3 camTarget = vector3(0.0f, 0.0f, 0.0f);
		vector3 camLookUp = vector3(0.0f, 1.0f, 0.0f);

		Types::F32 focalLength = 0.5f;
		PerspectiveCamera camera(focalLength, camPosition, camTarget, camLookUp);
		camera.SetFilm(std::make_unique<Film>(
			UserConfig::COMMON_PIXEL_WIDTH, UserConfig::COMMON_PIXEL_HEIGHT,
			-0.5f, +0.5f,
			-0.5f, +0.5f));

		HitRecord hitRec;

		for (unsigned int i = 0; i < camera.m_film->m_width; ++i)
		{
			for (unsigned int j = 0; j < camera.m_film->m_height; ++j)
			{
				Ray ray = camera.GetRay(i, j);

				//BREAK_POINT_IF(i == 278 && j == 192);
				//BREAK_POINT_IF(i == 185 && j == 195);

				// try triangle
				if (poly->BoundingBox().Hit(ray, 0.0f, 1000.f, &hitRec))
				{
					if (poly->Hit(ray, 0.0f, 1000.0f, &hitRec))
					{
						//hitPixel.SetChannel<RGBA::R>((hitRec.m_hitT / 3.8f));
						//hitPixel.SetChannel<RGBA::G>((hitRec.m_hitT / 3.8f));
						//hitPixel.SetChannel<RGBA::B>((hitRec.m_hitT / 3.8f));
						hitColor.SetChannel<RGBA::A>(hitRec.m_hitT / 4.0f);

						camera.IncomeLight(i, j, hitColor);
					}
					else
					{
						camera.IncomeLight(i, j, hitBBoxColor);
					}
				}
				else
				{
					camera.IncomeLight(i, j, backgroundColor);
				}
				
			}
		}

		camera.m_film->SaveTo("OutputTestImage\\ThisImageIsForHitPolygon.png");

		errorLogger += LetUserCheckJudge(
			"check \".\\OutputTestImage\\ThisImageIsForHitPolygon.png\"\n"
			"you should have seen a polygon(red) in the scene, surrounded by a black box area(3D), the background color is dark green.");
	TEST_UNIT_END;
#pragma endregion

#pragma region render a sphere with a point light
	TEST_UNIT_START("render a sphere with a point light")
		using namespace CommonClass;

		/*!
		`	\brief set scene and light.
		*/
		Scene scene;
		scene.m_pointLight = Light(vector3(0.0f, 2.0f, 3.0f), RGBA::RED);

		/*!
			\brief set a sphere to render.
		*/
		auto tsph = std::make_unique<Sphere>(vector3(0.0f, 0.0f, 0.0f), 1.0f);
		tsph->m_kDiffuse = RGBA::WHITE;
		tsph->m_kAmbient = RGBA::WHITE;
		tsph->m_kSpecular = RGBA::WHITE;
		scene.Add(std::move(tsph));

		RGBA hitPixel(1.0f, 1.0f, 1.0f);
		RGBA backgroundColor(RGBA::BLACK);

		/*!
			\brief config a camera.
		*/
		vector3 camPosition = vector3(1.8f, 1.8f, 1.8f);
		vector3 camTarget = vector3(0.0f, 0.0f, 0.0f);
		vector3 camLookUp = vector3(0.0f, 1.0f, 0.0f);
		Types::F32 focalLength = 0.5f;
		PerspectiveCamera camera(focalLength, camPosition, camTarget, camLookUp);

		camera.SetFilm(std::make_unique<Film>(
			UserConfig::COMMON_PIXEL_WIDTH, UserConfig::COMMON_PIXEL_HEIGHT,
			-0.5f, +0.5f,
			-0.5f, +0.5f));

		HitRecord hitRec;
		Ray viewRay;
		Ray toLightRay;
		for (unsigned int i = 0; i < camera.m_film->m_width; ++i)
		{
			for (unsigned int j = 0; j < camera.m_film->m_height; ++j)
			{
				viewRay = camera.GetRay(i, j);
				
				//BREAK_POINT_IF(i == 256 && j == 256);

				if (scene.Hit(viewRay, 0.0f, 1000.0f, &hitRec))
				{
					toLightRay = scene.m_pointLight.ToLight(hitRec.m_hitPoint);

					camera.IncomeLight(i, j, hitPixel);
				}
				else
				{
					camera.IncomeLight(i, j, backgroundColor);
				}
			}
		}

		camera.m_film->SaveTo("OutputTestImage\\ThisImageIsForPointLight.png");

		errorLogger += LetUserCheckJudge(
			"check \".\\OutputTestImage\\ThisImageIsForPointLight.png\"\n"
			"you should have seen a sphere rendered under a point light");

	TEST_UNIT_END;
#pragma endregion

#pragma region a temp test for ColorTemplate
	TEST_UNIT_START("a temp test for ColorTemplate")

		const unsigned int size_cc4 = sizeof(TRGBA);
		const unsigned int size_cc3 = sizeof(TRGB);
		TRGBA color4(0.0f, 1.0f, 0.0f, 2.0f);
		TRGB color3(0.8f, 1.0f);
		color4.m_chas.m_a;
		color3.m_chas.m_r;
		const Types::F32 conmin = TRGBA::MIN_CHANNEL_VALUE;

        color4.SetChannel<3>(2.0f);

		color4.SetChannel<TRGBA::R>(2.0f);
		errorLogger.LogIfNotEq  (1.0f, color4.m_chas.m_r);
        errorLogger.LogIfEq     (3.0f, color4.m_chas.m_r);

        color4.SetChannel<TRGBA::G>(-1.0f);
        errorLogger.LogIfNotEq  (0.0f, color4.m_chas.m_g);
        errorLogger.LogIfEq     (0.4f, color4.m_chas.m_g);

        color4.SetChannel<TRGBA::B>(0.3f);
        errorLogger.LogIfNotEq  (0.3f, color4.m_chas.m_b);
        errorLogger.LogIfEq     (0.2f, color4.m_chas.m_b);

        color4.SetChannel<TRGBA::A>(0.6f);
        errorLogger.LogIfNotEq  (0.6f, color4.m_chas.m_a);
        errorLogger.LogIfEq     (0.7f, color4.m_chas.m_a);



        //color3.SetChannel<45>(2.0f);
        //color3.SetChannel<TRGBA::A>(2.0f);
        color3.SetChannel<TRGBA::R>(2.0f);
        errorLogger.LogIfNotEq  (1.0f, color3.m_chas.m_r);
        errorLogger.LogIfEq     (3.0f, color4.m_chas.m_r);

        color3.SetChannel<TRGBA::G>(-1.0f);
        errorLogger.LogIfNotEq  (0.0f, color3.m_chas.m_g);
        errorLogger.LogIfEq     (0.4f, color4.m_chas.m_g);

        color3.SetChannel<TRGBA::B>(0.3f);
        errorLogger.LogIfNotEq  (0.3f, color3.m_chas.m_b);
        errorLogger.LogIfEq     (0.2f, color4.m_chas.m_b);

        TRGBA try324 = Cast(color3);

	TEST_UNIT_END;
#pragma endregion

#pragma region test RGBA +/ -/ rgbMulti/ *(scalar)/ /(scalar)
	TEST_UNIT_START("test RGBA +/ -/ */ *(scalar)/ /(scalar)")
		RandomTool::MTRandom mtr;
		const unsigned int MAX_RAND_INT = 64;

		/*!
			\brief clamp the channel to [0.0f, 1.0f]
		*/
		auto clampChannel = [](Types::F32 ch)-> Types::F32
		{
			if (ch <= 0.0f)
			{
				return 0.0f;
			}
			else if (ch >= 1.0f)
			{
				return 1.0f;
			}
			else
			{
				return ch;
			}
		};

		for (int i = 0; i < 200; ++i)
		{
			/*!
				make up the RGBA channel
			*/
			const Types::F32	comu1(mtr.Random()),
								comu2(mtr.Random()),
								comu3(mtr.Random()),
								comu4(mtr.Random()),
								comu5(mtr.Random()),
								comu6(mtr.Random()),
								comu7(mtr.Random()),
								comu8(mtr.Random());

			/*!
				scale the RGB with float.
			*/
			const Types::F32	comf1(mtr.Random() * mtr.Random(MAX_RAND_INT)),
								comf2(mtr.Random() * mtr.Random(MAX_RAND_INT));

			TRGBA cmp1(comu1, comu2, comu3, comu4);
			TRGBA cmp2(comu5, comu6, comu7, comu8);

			/*!
				\brief recover the cmp1's value
			*/
			auto recoverCMP1 = [&cmp1, comu1, comu2, comu3, comu4]()
			{
				cmp1 = TRGBA(comu1, comu2, comu3, comu4);
			};

			// default constructor can ignore alpha channel, the alpha of the pixel will be set to max(opaque).
			errorLogger.LogIfNotEq(
                TRGBA(comu1, comu2, comu3),
                TRGBA(comu1, comu2, comu3, TRGBA::ALPHA_CHANNEL_OPAQUE));

			// assign
			cmp1 = cmp2;
			errorLogger.LogIfNotEq(
				cmp1,
                TRGBA(comu5, comu6, comu7, comu8));
			recoverCMP1();

			// cmp1 + cmp2
			//cmp1 = cmp1 + cmp2;
			errorLogger.LogIfNotEq( 
				cmp1 + cmp2,
                TRGBA(clampChannel(comu1 + comu5), clampChannel(comu2 + comu6), clampChannel(comu3 + comu7), clampChannel(comu4 + comu8)));
			//recoverCMP1();

			// cmp1 - cmp2
            //cmp1 = cmp1 - cmp2;
			errorLogger.LogIfNotEq(
				cmp1 - cmp2,
                TRGBA(clampChannel(comu1 - comu5), clampChannel(comu2 - comu6), clampChannel(comu3 - comu7), clampChannel(comu4 - comu8)));
			//recoverCMP1();
			
			// cmp1 * cmp2
			//cmp1 = cmp1 * cmp2,
			errorLogger.LogIfNotEq(
				cmp1 * cmp2,
				TRGBA(clampChannel(comu1 * comu5), clampChannel(comu2 * comu6), clampChannel(comu3 * comu7), clampChannel(comu4 * comu8)));
			//recoverCMP1();

			// cmp1 * scalar
			//cmp1.MulRGB(comf1),
			errorLogger.LogIfNotEq(
				cmp1 * comf1,
                TRGBA(clampChannel(comu1 * comf1), clampChannel(comu2 * comf1), clampChannel(comu3 * comf1), clampChannel(comu4 * comf1)));
			errorLogger.LogIfNotEq(
			    comf1 * cmp1,       // switch RGBA and scalar
				TRGBA(clampChannel(comu1 * comf1), clampChannel(comu2 * comf1), clampChannel(comu3 * comf1), clampChannel(comu4 * comf1)));
			//recoverCMP1();

			// cmp1 / scalar
			//Types::F32 reciprocalComf2 = 1.0f / comf2;
			//cmp1.DivRGB(comf2),
			errorLogger.LogIfNotEq(
				cmp1 / comf2,
				TRGBA(clampChannel(comu1 / comf2), clampChannel(comu2 / comf2), clampChannel(comu3 / comf2), clampChannel(comu4 / comf2)));
			//recoverCMP1();
		}
	TEST_UNIT_END;
#pragma endregion

TEST_MODULE_END
