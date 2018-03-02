// For Unit Test
#include <MyTools\UnitTestModules.h>
#include <iostream>
#include <MyTools\RandomToolNeedLib\MTRandom.h>
#pragma comment(lib, "MyTools\\RandomToolNeedLib\\LibForMTRandomAndPrimeSearch.lib")

// include test target code
#include "../CommonClasses/vector2.h"
#include "../CommonClasses/vector3.h"
#include "../CommonClasses/RGBA.h"
#pragma comment(lib, "CommonClasses.lib")


DECLARE_TEST_UNITS;

namespace TestUnit
{
void GetReady() {}
void AfterTest() {}

void AddTestUnit()
{
#pragma region Try the testUnit
	TEST_UNIT_START("a test always success")
		return true;
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

#pragma region test RGBA +/ -/ */ *(scalar)/ /(scalar)
	TEST_UNIT_START("test RGBA +/ -/ */ *(scalar)/ /(scalar)")
		RandomTool::MTRandom mtr;
		const unsigned int MAX_RAND_INT = 255;

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
			const Types::F32	comf1(mtr.Random()),
								comf2(mtr.Random());

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
				CommonClass::RGBA(comu1, comu2, comu3, RGBA::ALPHA_CHANNEL_OPAQUE);
			);

			// assign rgb only
			cmp1.AssignRGB(cmp2);
			errorLogger.LogIfNotEq(
				cmp1,
				CommonClass::RGBA(comu5, comu6, comu7, comu8));
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
				CommonClass::RGBA(clampChannel((comu1 * comu5) >> 8), clampChannel((comu2 - comu6) >> 8), clampChannel((comu3 - comu7) >> 8), comu4));	// '>> 8' because we scale 8bit channel value back to [0, 255]
			recoverCMP1();

			// cmp1 * scalar rgb only
			cmp1.MulRGB(comf1),
			errorLogger.LogIfNotEq(
				cmp1,
				CommonClass::RGBA(clampChannel(comu1 * comf1), clampChannel(comu2 * comf1), clampChannel(comu3 * comf1), comu4));
			recoverCMP1();

			// cmp1 / scalar rgb only
			cmp1.DivRGB(comf2),
			errorLogger.LogIfNotEq(
				cmp1,
				CommonClass::RGBA(clampChannel(comu1 / comf2), clampChannel(comu2 / comf2), clampChannel(comu3 / comf2), comu4));
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
				revertChannel(comu4 - comu8));
			recoverCMP1();

			// alpha mul
			cmp1.MulAlpha(comu8);
			errorLogger.LogIfNotEq(
				cmp1.m_a,
				clampChannel((comu1 * comu8) >> 8));
			recoverCMP1();

			// alpha mul float
			cmp1.MulAlpha(comf1);
			errorLogger.LogIfNotEq(
				cmp1.m_a,
				clampChannel((comu1 * comf1)));
			recoverCMP1();

			// alpha div float
			cmp1.DivAlpha(comf2);
			errorLogger.LogIfNotEq(
				cmp1.m_a,
				clampChannel((comu1 / comf2)));
			recoverCMP1();


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
