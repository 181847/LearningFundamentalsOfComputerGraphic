// For Unit Test
#include <MyTools\UnitTestModules.h>
#include <iostream>
#include <MyTools\RandomToolNeedLib\MTRandom.h>
#include <MyTools\MathTool.h>
#pragma comment(lib, "MyTools\\RandomToolNeedLib\\LibForMTRandomAndPrimeSearch.lib")

// include test target code
#include "../CommonClasses/vector2.h"
#include "../CommonClasses/vector3.h"
#include "../CommonClasses/RGBA.h"
#include "../CommonClasses/Image.h"
#include "../CommonClasses/OrthographicCamera.h"
#pragma comment(lib, "CommonClasses.lib")

RandomTool::MTRandom globalMtr;
const unsigned int G_MAX_INT = 1000;

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

DECLARE_TEST_UNITS;

namespace TestUnit
{
void GetReady() {}
void AfterTest() {}

void AddTestUnit()
{
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
		const Types::U32 width(512), height(512);
		CommonClass::Image testImage(width, height);

		CommonClass::RGBA pixelSetter;
		pixelSetter.SetChannel<CommonClass::RGBA::BLUE>(0.5f);

		for (int x = 0; x < width; ++x)
		{
			for (int y = 0; y < height; ++y)
			{
				pixelSetter.SetChannel<CommonClass::RGBA::RED>  (         x / 512.0f);
				pixelSetter.SetChannel<CommonClass::RGBA::GREEN>(         y / 512.0f);
				pixelSetter.SetChannel<CommonClass::RGBA::ALPHA>( ( x + y ) / 1024.0f);

				testImage.SetPixel(x, y, pixelSetter);
			}
		}

		testImage.SaveTo("OutputTestImage\\ThisImageIsForTest.png");

		std::cout << "Please check the project folder, and see the Image \".\\OutputTestImage\\ThisImageIsForTest.png\"\n"
				  << "check it with the file in the same folder \"CheckWithMe.png\""
				  << "if the image is right, input a single number\n"
				  << "else just hit ENTER, which means there's error:";

		char ch = std::getchar();

		// if the character is not number, then the image is wrong.
		if (ch < '0' || ch > '9')
		{
			errorLogger.addErrorCount(true);
		}
		else
		{
			// consume the extra 'ENTER'.
			std::getchar();
		}

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
			std::printf("## check perpendicular vector3 %f.\n", dpValue);

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

}// AddTestUnit()

}// namespace TestUnit

int main()
{
	TestUnit::testMain();
	return 0;
}
