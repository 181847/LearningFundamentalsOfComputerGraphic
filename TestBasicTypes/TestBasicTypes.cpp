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
#include "../CommonClasses/hvector.h"
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
    } while (!allowZeroVector && randVec.m_x == 0.0f && randVec.m_y == 0.0f && randVec.m_z == 0.0f);


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

			RGBA cmp1(comu1, comu2, comu3, comu4);
			RGBA cmp2(comu5, comu6, comu7, comu8);

			/*!
				\brief recover the cmp1's value
			*/
			auto recoverCMP1 = [&cmp1, comu1, comu2, comu3, comu4]()
			{
				cmp1 = RGBA(comu1, comu2, comu3, comu4);
			};

			// default constructor can ignore alpha channel, the alpha of the pixel will be set to max(opaque).
			errorLogger.LogIfNotEq(
                RGBA(comu1, comu2, comu3),
                RGBA(comu1, comu2, comu3, RGBA::ALPHA_CHANNEL_OPAQUE));

			// assign
			cmp1 = cmp2;
			errorLogger.LogIfNotEq(
				cmp1,
                RGBA(comu5, comu6, comu7, comu8));
			recoverCMP1();

			// cmp1 + cmp2
			//cmp1 = cmp1 + cmp2;
			errorLogger.LogIfNotEq( 
				cmp1 + cmp2,
                RGBA(clampChannel(comu1 + comu5), clampChannel(comu2 + comu6), clampChannel(comu3 + comu7), clampChannel(comu4 + comu8)));
			//recoverCMP1();

			// cmp1 - cmp2
            //cmp1 = cmp1 - cmp2;
			errorLogger.LogIfNotEq(
				cmp1 - cmp2,
                RGBA(clampChannel(comu1 - comu5), clampChannel(comu2 - comu6), clampChannel(comu3 - comu7), clampChannel(comu4 - comu8)));
			//recoverCMP1();
			
			// cmp1 * cmp2
			//cmp1 = cmp1 * cmp2,
			errorLogger.LogIfNotEq(
				cmp1 * cmp2,
				RGBA(clampChannel(comu1 * comu5), clampChannel(comu2 * comu6), clampChannel(comu3 * comu7), clampChannel(comu4 * comu8)));
			//recoverCMP1();

			// cmp1 * scalar
			//cmp1.MulRGB(comf1),
			errorLogger.LogIfNotEq(
				cmp1 * comf1,
                RGBA(clampChannel(comu1 * comf1), clampChannel(comu2 * comf1), clampChannel(comu3 * comf1), clampChannel(comu4 * comf1)));
			errorLogger.LogIfNotEq(
			    comf1 * cmp1,       // switch RGBA and scalar
				RGBA(clampChannel(comu1 * comf1), clampChannel(comu2 * comf1), clampChannel(comu3 * comf1), clampChannel(comu4 * comf1)));
			//recoverCMP1();

			// cmp1 / scalar
			//Types::F32 reciprocalComf2 = 1.0f / comf2;
			//cmp1.DivRGB(comf2),
			errorLogger.LogIfNotEq(
				cmp1 / comf2,
				RGBA(clampChannel(comu1 / comf2), clampChannel(comu2 / comf2), clampChannel(comu3 / comf2), clampChannel(comu4 / comf2)));
			//recoverCMP1();
		}
	TEST_UNIT_END;
#pragma endregion

#pragma region test RGB +/ -/ rgbMulti/ *(scalar)/ /(scalar)
	TEST_UNIT_START("test RGB +/ -/ */ *(scalar)/ /(scalar)")
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

			RGB cmp1(comu1, comu2, comu3);
			RGB cmp2(comu5, comu6, comu7);

			/*!
				\brief recover the cmp1's value
			*/
			auto recoverCMP1 = [&cmp1, comu1, comu2, comu3, comu4]()
			{
				cmp1 = RGB(comu1, comu2, comu3);
			};

			// assign
			cmp1 = cmp2;
			errorLogger.LogIfNotEq(
				cmp1,
                RGB(comu5, comu6, comu7));
			recoverCMP1();

			// cmp1 + cmp2
			//cmp1 = cmp1 + cmp2;
			errorLogger.LogIfNotEq( 
				cmp1 + cmp2,
                RGB(clampChannel(comu1 + comu5), clampChannel(comu2 + comu6), clampChannel(comu3 + comu7)));
			//recoverCMP1();

			// cmp1 - cmp2
            //cmp1 = cmp1 - cmp2;
			errorLogger.LogIfNotEq(
				cmp1 - cmp2,
                RGB(clampChannel(comu1 - comu5), clampChannel(comu2 - comu6), clampChannel(comu3 - comu7)));
			//recoverCMP1();
			
			// cmp1 * cmp2
			//cmp1 = cmp1 * cmp2,
			errorLogger.LogIfNotEq(
				cmp1 * cmp2,
				RGB(clampChannel(comu1 * comu5), clampChannel(comu2 * comu6), clampChannel(comu3 * comu7)));
			//recoverCMP1();

			// cmp1 * scalar
			//cmp1.MulRGB(comf1),
			errorLogger.LogIfNotEq(
				cmp1 * comf1,
                RGB(clampChannel(comu1 * comf1), clampChannel(comu2 * comf1), clampChannel(comu3 * comf1)));
			errorLogger.LogIfNotEq(
			    comf1 * cmp1,       // switch RGBA and scalar
				RGB(clampChannel(comu1 * comf1), clampChannel(comu2 * comf1), clampChannel(comu3 * comf1)));
			//recoverCMP1();

			// cmp1 / scalar
			//Types::F32 reciprocalComf2 = 1.0f / comf2;
			//cmp1.DivRGB(comf2),
			errorLogger.LogIfNotEq(
				cmp1 / comf2,
				RGB(clampChannel(comu1 / comf2), clampChannel(comu2 / comf2), clampChannel(comu3 / comf2)));
			//recoverCMP1();
		}
	TEST_UNIT_END;
#pragma endregion

#pragma region cast between RGBA and RGB
    TEST_UNIT_START("cast between RGBA and RGB")
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

            RGBA color4(comu1, comu2, comu3, comu4);
            RGB color3(comu5, comu6, comu7);

            errorLogger.LogIfNotEq(Cast(color4), RGB(comu1, comu2, comu3));
            errorLogger.LogIfNotEq(Cast(color3), RGBA(comu5, comu6, comu7, RGBA::ALPHA_CHANNEL_OPAQUE));
            errorLogger.LogIfNotEq(Cast(color3, comu8), RGBA(comu5, comu6, comu7, comu8));
        }
    TEST_UNIT_END;
#pragma endregion

#pragma region check Material construct properties
    TEST_UNIT_START("check Material construct properties")
        using namespace CommonClass;
        const unsigned int MAX_REFLECT_INDEX = 5;
        // reset random seed
        globalMtr.SetRandomSeed(1);

        for (int i = 0; i < 20; ++i)
        {
            RGB color = GetRandomRGB();
            RGB rFresnel0 = GetRandomRGB();
            unsigned int randomReflectIndex = 1 + globalMtr.Random(MAX_REFLECT_INDEX - 1);

            Material mat1;                              // default mat, white color, fresnel coefficient is zero
            Material mat2(color, rFresnel0);            // specific the fresnel coefficient with RGB.
            Material mat3(color, randomReflectIndex);   // calculate fresnel coefficient using reflect index.

            errorLogger.LogIfNotEq(mat1.m_kDiffuse, RGB::WHITE);
            errorLogger.LogIfNotEq(mat1.m_rFresnel_0, RGB::BLACK);

            errorLogger.LogIfNotEq(mat2.m_kDiffuse, color);
            errorLogger.LogIfNotEq(mat2.m_rFresnel_0, rFresnel0);

            // manully calculate the fresnel coefficient.
            Types::F32 reflectIndexToFresnel0 = (randomReflectIndex - 1) * 1.0f / (randomReflectIndex + 1);
            reflectIndexToFresnel0 *= reflectIndexToFresnel0;

            errorLogger.LogIfNotEq(mat3.m_kDiffuse, color);
            errorLogger.LogIfNotEq(mat3.m_rFresnel_0, RGB(reflectIndexToFresnel0, reflectIndexToFresnel0, reflectIndexToFresnel0));

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

TEST_MODULE_END

