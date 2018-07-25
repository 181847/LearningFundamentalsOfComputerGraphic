#pragma once
/*!
	\brief
	In this head file, I'm going to transfer all the tests in Project'TestBasicTypes'
	into using TestSuit modules.
	[2018/7/25 9:22]
*/

#include <MyTools/TestTool/Suit.h>
#include <MyTools/RandomToolNeedLib/MTRandom.h>
#pragma comment(lib, "MyTools/MyTool.lib")

// tools about debugging
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
#include "../CommonClasses/Transform.h"
#include "../CommonClasses/Pipline.h"
#include "../CommonClasses/CoordinateFrame.h"
#include "../CommonClasses/FixPointNumber.h"
#include "../CommonClasses/EFloat.h"
#include "../CommonClasses/DebugConfigs.h"
#include "../CommonClasses/EdgeEquation2D.h"
#pragma comment(lib, "CommonClasses.lib")

using namespace CommonClass;

template<typename ... CASE_TYPE_LIST>
class BasicTypesTestSuit_BASE: public TestSuit::Suit<CASE_TYPE_LIST...>
{
	const std::string SuitName = "BasicTypes test";

public:
    /*!
        \brief run before all start.
    */
    virtual void PrepareTotal() override
    {
        printf("%s start.\n", SuitName.c_str());
    }

    /*!
        \brief run before each case run.
        \param pTheCase the case that will be run, don't worry about the memory of pTheCase right now.
        \return a pointer can point to any thing, this pointer will be passed to the case.
    */
    virtual void * PrepareBeforeEachCase(TestSuit::Case * pTheCase) override
    {
		return nullptr;
    }

    /*!
        \brief run after each case
        \param pTheCase the case stops
        \param pEnvironment the pointer that be returned by PrepareBeforeEachCase(...), 
               Please take care of the memory where the pointer points to by yourself.
    */
    virtual void FinishEachCase(TestSuit::Case * pTheCase, void * pEnvironment) override
    {
    }

    /*!
        \brief run when all case finished.
    */
    virtual void FinishAllCases() override
	{
		printf("%s end.\n", SuitName.c_str());
    }
};

/*!
	\brief a base class for all cases that need Random number generator.
*/
class CaseContainRandomTool : public TestSuit::Case
{
public:
	/*!
		\brief the random number generator.
	*/
	RandomTool::MTRandom mtr;

	CaseContainRandomTool(const std::string& caseName) : Case(caseName) {}
	
	/*!
		\brief clamp the channel to [0.0f, 1.0f]
		\param ch the channel value to be clamped.
	*/
	Types::F32 ClampChannel(Types::F32 ch)
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
	}

	/*!
		\brief get random rgb color.
	*/
	RGB GetRandomRGB()
	{
		return RGB(mtr.Random(), mtr.Random(), mtr.Random());
	}

	/*!
		\brief get random vector3.
		if you want to set the seed, try mtr.SetRandomSeed(...);
	*/
	CommonClass::vector3 GetRandomVector3(bool allowZeroVector = true, const int MAX_INT = 200)
	{
		CommonClass::vector3 randVec;
		do
		{
			randVec = CommonClass::vector3(
				(mtr.Random() - 0.5f) * (mtr.Random(MAX_INT) + 1),
				(mtr.Random() - 0.5f) * (mtr.Random(MAX_INT) + 1),
				(mtr.Random() - 0.5f) * (mtr.Random(MAX_INT) + 1));

			// if allowZeroVector is false, loop until a none zero vector
		} while (!allowZeroVector && randVec.m_x == 0.0f && randVec.m_y == 0.0f && randVec.m_z == 0.0f);


		return randVec;
	}

};

class CaseBasicVector2 :public TestSuit::Case
{
public:
	CaseBasicVector2() : Case("test vector2") {}

	virtual void Run() override
	{
		CommonClass::vector2 v2;

		TEST_ASSERT(v2.m_x == 0.0f);
		TEST_ASSERT(v2.m_y == 0.0f);
	}
};

class TestVector2Case :public CaseContainRandomTool
{
public:
	TestVector2Case() : CaseContainRandomTool("test vector2") {}
	
	virtual void Run() override
	{
		const unsigned int MAX_RAND_INT = 600;

		for (int i = 0; i < 200; ++i)
		{
			const Types::F32    comf1(1.0f * mtr.Random(MAX_RAND_INT) / (mtr.Random(MAX_RAND_INT) + 1)),
				comf2(1.0f * mtr.Random(MAX_RAND_INT) / (mtr.Random(MAX_RAND_INT) + 1)),
				comf3(1.0f * mtr.Random(MAX_RAND_INT) / (mtr.Random(MAX_RAND_INT) + 1)),
				comf4(1.0f * mtr.Random(MAX_RAND_INT) / (mtr.Random(MAX_RAND_INT) + 1));
			CommonClass::vector2 cmp1(comf1, comf2);
			CommonClass::vector2 cmp2(comf3, comf4);

			TEST_ASSERT(
				(cmp1 + cmp2)
				==
				(CommonClass::vector2(comf1 + comf3, comf2 + comf4)));

			TEST_ASSERT(
				(cmp1 - cmp2)
				==
				(CommonClass::vector2(comf1 - comf3, comf2 - comf4)));

			TEST_ASSERT(
				(cmp1 * cmp2)
				==
				(comf1 * comf3 + comf2 * comf4));

			TEST_ASSERT(
				(CommonClass::dotProd(cmp1, cmp2))
				==
				(comf1 * comf3 + comf2 * comf4));
		}// end for
	}// end Run
};

class CaseForVector3 : public CaseContainRandomTool
{
public:
	CaseForVector3():CaseContainRandomTool("test vector3"){}
	
	virtual void Run() override
	{
		const unsigned int MAX_RAND_INT = 600;

		for (int i = 0; i < 200; ++i)
		{
			std::array<Types::F32, 6> comfArr;
			for (auto& comf : comfArr)
			{
				comf = 1.0f * mtr.Random(MAX_RAND_INT) / (mtr.Random(MAX_RAND_INT) + 1);
			}

			CommonClass::vector3 cmp1(comfArr[0], comfArr[1], comfArr[2]);
			CommonClass::vector3 cmp2(comfArr[3], comfArr[4], comfArr[5]);

			// cmp1 + cmp2
			TEST_ASSERT(
				(cmp1 + cmp2)
				==
				CommonClass::vector3(comfArr[0] + comfArr[3], comfArr[1] + comfArr[4], comfArr[2] + comfArr[5]));

			// cmp1 - cmp2
			TEST_ASSERT(
				(cmp1 - cmp2)
				==
				CommonClass::vector3(comfArr[0] - comfArr[3], comfArr[1] - comfArr[4], comfArr[2] - comfArr[5]));

			// cmp1 * cmp2
			TEST_ASSERT(
				(cmp1 * cmp2)
				==
				(comfArr[0] * comfArr[3] + comfArr[1] * comfArr[4] + comfArr[2] * comfArr[5]));

			// cmp1 * cmp2
			TEST_ASSERT(
				CommonClass::dotProd(cmp1, cmp2)
				==
				(comfArr[0] * comfArr[3] + comfArr[1] * comfArr[4] + comfArr[2] * comfArr[5]));

			// cmp1 X cmp2
			TEST_ASSERT(
				CommonClass::crossProd(cmp1, cmp2)
				==
				CommonClass::vector3(
					comfArr[1] * comfArr[5] - comfArr[4] * comfArr[2],
					comfArr[3] * comfArr[2] - comfArr[0] * comfArr[5],
					comfArr[0] * comfArr[4] - comfArr[3] * comfArr[1]));

			// vector3 * float
			Types::F32 randomFloat = mtr.Random();
			TEST_ASSERT(
				(cmp1 * randomFloat)
				==
				vector3(
					comfArr[0] * randomFloat,
					comfArr[1] * randomFloat,
					comfArr[2] * randomFloat));
			TEST_ASSERT(
				(randomFloat * cmp2)
				==
				vector3(
					comfArr[3] * randomFloat,
					comfArr[4] * randomFloat,
					comfArr[5] * randomFloat));

			// - vector3
			TEST_ASSERT(
				(-cmp1)
				==
				vector3(-comfArr[0], -comfArr[1], -comfArr[2]));
		}// end for loop
	}// end Run
};

class CaseForRGBA : public CaseContainRandomTool
{
public:
	CaseForRGBA() :CaseContainRandomTool("test RGBA") {}

	virtual void Run() override
	{
		const unsigned int MAX_RAND_INT = 64;

		for (int i = 0; i < 200; ++i)
		{
			/*!
				make up the RGBA channel
			*/
			std::array<Types::F32, 8> comuArr;
			for (auto& comu : comuArr)
			{
				comu = mtr.Random();
			}

			/*!
				scale the RGB with float.
			*/
			const Types::F32    comf1(mtr.Random() * mtr.Random(MAX_RAND_INT)),
								comf2(mtr.Random() * mtr.Random(MAX_RAND_INT));

			RGBA cmp1(comuArr[0], comuArr[1], comuArr[2], comuArr[3]);
			RGBA cmp2(comuArr[4], comuArr[5], comuArr[6], comuArr[7]);

			/*!
				\brief recover the cmp1's value
			*/
			auto recoverCMP1 = [&comuArr, &cmp1]()
			{
				cmp1 = RGBA(comuArr[0], comuArr[1], comuArr[2], comuArr[3]);
			};

			// default constructor can ignore alpha channel, the alpha of the pixel will be set to max(opaque).
			TEST_ASSERT(
				RGBA(comuArr[0], comuArr[1], comuArr[2])
				==
				RGBA(comuArr[0], comuArr[1], comuArr[2], RGBA::ALPHA_CHANNEL_OPAQUE));

			// assign
			cmp1 = cmp2;
			TEST_ASSERT(
				cmp1
				==
				RGBA(comuArr[4], comuArr[5], comuArr[6], comuArr[7]));
			recoverCMP1();

			// cmp1 + cmp2
			//cmp1 = cmp1 + cmp2;
			TEST_ASSERT(
				cmp1 + cmp2
				==
				RGBA(ClampChannel(comuArr[0] + comuArr[4]), ClampChannel(comuArr[1] + comuArr[5]), ClampChannel(comuArr[2] + comuArr[6]), ClampChannel(comuArr[3] + comuArr[7])));
			//recoverCMP1();

			// cmp1 - cmp2
			//cmp1 = cmp1 - cmp2;
			TEST_ASSERT(
				cmp1 - cmp2
				==
				RGBA(ClampChannel(comuArr[0] - comuArr[4]), ClampChannel(comuArr[1] - comuArr[5]), ClampChannel(comuArr[2] - comuArr[6]), ClampChannel(comuArr[3] - comuArr[7])));
			//recoverCMP1();

			// cmp1 * cmp2
			//cmp1 = cmp1 * cmp2,
			TEST_ASSERT(
				cmp1 * cmp2
				==
				RGBA(ClampChannel(comuArr[0] * comuArr[4]), ClampChannel(comuArr[1] * comuArr[5]), ClampChannel(comuArr[2] * comuArr[6]), ClampChannel(comuArr[3] * comuArr[7])));
			//recoverCMP1();

			// cmp1 * scalar
			//cmp1.MulRGB(comf1),
			TEST_ASSERT(
				cmp1 * comf1
				==
				RGBA(ClampChannel(comuArr[0] * comf1), ClampChannel(comuArr[1] * comf1), ClampChannel(comuArr[2] * comf1), ClampChannel(comuArr[3] * comf1)));
			TEST_ASSERT(
				comf1 * cmp1	// switch RGBA and scalar
				==       
				RGBA(ClampChannel(comuArr[0] * comf1), ClampChannel(comuArr[1] * comf1), ClampChannel(comuArr[2] * comf1), ClampChannel(comuArr[3] * comf1)));
			//recoverCMP1();

			// cmp1 / scalar
			//Types::F32 reciprocalComf2 = 1.0f / comf2;
			//cmp1.DivRGB(comf2),
			TEST_ASSERT(
				cmp1 / comf2
				==
				RGBA(ClampChannel(comuArr[0] / comf2), ClampChannel(comuArr[1] / comf2), ClampChannel(comuArr[2] / comf2), ClampChannel(comuArr[3] / comf2)));
			//recoverCMP1();
		}// end for
	}// end Run
};

class CaseForRGB : public CaseContainRandomTool
{
public:
	CaseForRGB() :CaseContainRandomTool("test RGB") {}

	virtual void Run() override
	{
        const unsigned int MAX_RAND_INT = 64;

        /*!
            \brief clamp the channel to [0.0f, 1.0f]
        */
        auto ClampChannel = [](Types::F32 ch)-> Types::F32
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
			std::array<Types::F32, 8> comuArr;
			for (auto& comu : comuArr)
			{
				comu = mtr.Random();
			}

            /*!
                scale the RGB with float.
            */
            const Types::F32    comf1(mtr.Random() * mtr.Random(MAX_RAND_INT)),
                                comf2(mtr.Random() * mtr.Random(MAX_RAND_INT));

            RGB cmp1(comuArr[0], comuArr[1], comuArr[2]);
            RGB cmp2(comuArr[4], comuArr[5], comuArr[6]);

            /*!
                \brief recover the cmp1's value
            */
            auto recoverCMP1 = [&cmp1, &comuArr]()
            {
                cmp1 = RGB(comuArr[0], comuArr[1], comuArr[2]);
            };

            // assign
			cmp1 = cmp2;
			TEST_ASSERT(
                cmp1
				==
                RGB(comuArr[4], comuArr[5], comuArr[6]));
            recoverCMP1();

            // cmp1 + cmp2
			//cmp1 = cmp1 + cmp2;
			TEST_ASSERT(
				cmp1 + cmp2
				==
                RGB(ClampChannel(comuArr[0] + comuArr[4]), ClampChannel(comuArr[1] + comuArr[5]), ClampChannel(comuArr[2] + comuArr[6])));
            //recoverCMP1();

            // cmp1 - cmp2
			//cmp1 = cmp1 - cmp2;
			TEST_ASSERT(
				cmp1 - cmp2
				==
                RGB(ClampChannel(comuArr[0] - comuArr[4]), ClampChannel(comuArr[1] - comuArr[5]), ClampChannel(comuArr[2] - comuArr[6])));
            //recoverCMP1();
            
            // cmp1 * cmp2
			//cmp1 = cmp1 * cmp2,
			TEST_ASSERT(
				cmp1 * cmp2
				==
                RGB(ClampChannel(comuArr[0] * comuArr[4]), ClampChannel(comuArr[1] * comuArr[5]), ClampChannel(comuArr[2] * comuArr[6])));
            //recoverCMP1();

            // cmp1 * scalar
			//cmp1.MulRGB(comf1),
			TEST_ASSERT(
				cmp1 * comf1
				==
				RGB(ClampChannel(comuArr[0] * comf1), ClampChannel(comuArr[1] * comf1), ClampChannel(comuArr[2] * comf1)));
			TEST_ASSERT(
				comf1 * cmp1 // switch RGBA and scalar
				==       
                RGB(ClampChannel(comuArr[0] * comf1), ClampChannel(comuArr[1] * comf1), ClampChannel(comuArr[2] * comf1)));
            //recoverCMP1();

            // cmp1 / scalar
            //Types::F32 reciprocalComf2 = 1.0f / comf2;
			//cmp1.DivRGB(comf2),
			TEST_ASSERT(
				cmp1 / comf2
				==
                RGB(ClampChannel(comuArr[0] / comf2), ClampChannel(comuArr[1] / comf2), ClampChannel(comuArr[2] / comf2)));
            //recoverCMP1();
        }// end for
	}// end Run
};

class CaseForRGBAndRGBA : public CaseContainRandomTool
{
public:
	CaseForRGBAndRGBA() :CaseContainRandomTool("test casting between RGB and RGBA") {}

	virtual void Run() override
	{
		const unsigned int MAX_RAND_INT = 64;

		for (int i = 0; i < 200; ++i)
		{
			/*!
				make up the RGBA channel
			*/
			std::array<Types::F32, 8> comuArr;
			for (auto& comu : comuArr)
			{
				comu = mtr.Random();
			}

			/*!
				scale the RGB with float.
			*/
			const Types::F32    comf1(mtr.Random() * mtr.Random(MAX_RAND_INT)),
				comf2(mtr.Random() * mtr.Random(MAX_RAND_INT));

			RGBA	color4(comuArr[0], comuArr[1], comuArr[2], comuArr[3]);
			RGB		color3(comuArr[4], comuArr[5], comuArr[6]);

			TEST_ASSERT(Cast(color4)				== RGB	(comuArr[0], comuArr[1], comuArr[2]));
			TEST_ASSERT(Cast(color3)				== RGBA	(comuArr[4], comuArr[5], comuArr[6], RGBA::ALPHA_CHANNEL_OPAQUE));
			TEST_ASSERT(Cast(color3, comuArr[7])	== RGBA	(comuArr[4], comuArr[5], comuArr[6], comuArr[7]));
		}
	}
};

class CaseForMaterialProperty : public CaseContainRandomTool
{
public:
	CaseForMaterialProperty() :CaseContainRandomTool("check material construct properties") {}

	virtual void Run() override
	{
		const unsigned int MAX_REFLECT_INDEX = 5;

		for (int i = 0; i < 20; ++i)
		{
			RGB color = GetRandomRGB();
			RGB rFresnel0 = GetRandomRGB();
			unsigned int randomReflectIndex = 1 + mtr.Random(MAX_REFLECT_INDEX - 1);

			Material mat1;                              // default mat, white color, fresnel coefficient is zero
			Material mat2(color, rFresnel0);            // specific the fresnel coefficient with RGB.
			Material mat3(color, randomReflectIndex);   // calculate fresnel coefficient using reflect index.

			TEST_ASSERT(mat1.m_kDiffuse			== RGB::WHITE);
			TEST_ASSERT(mat1.m_rFresnel_0		== RGB::BLACK);

			TEST_ASSERT(mat2.m_kDiffuse			== color);
			TEST_ASSERT(mat2.m_rFresnel_0		== rFresnel0);

			// manully calculate the fresnel coefficient.
			Types::F32 reflectIndexToFresnel0 = (randomReflectIndex - 1) * 1.0f / (randomReflectIndex + 1);
			reflectIndexToFresnel0 *= reflectIndexToFresnel0;

			TEST_ASSERT(mat3.m_kDiffuse			== color);
			TEST_ASSERT(mat3.m_rFresnel_0		== RGB(reflectIndexToFresnel0, reflectIndexToFresnel0, reflectIndexToFresnel0));

		}
	}
};

class CaseForVector3Normalize : public CaseContainRandomTool
{
public:
	CaseForVector3Normalize() : CaseContainRandomTool("test vector3 normalize") {}

	virtual void Run() override
	{
		using namespace MathTool;

		mtr.SetRandomSeed(5);

		for (int i = 0; i < 20; ++i)
		{
			vector3 v1(GetRandomVector3(false));

			v1 = Normalize(v1);

			TEST_ASSERT(almost_equal(Length(v1), 1.0f, 8));

			v1.m_x *= 2.0f;
			v1.m_y *= 2.0f;
			v1.m_z *= 3.0f;

			// insurance, make vector's length not equal to unit.
			TEST_ASSERT( ! almost_equal(Length(v1), 1.0f, 8));
		}// end for
	}// end Run
};

class CaseForHvector : public CaseContainRandomTool
{
public:
	CaseForHvector() :CaseContainRandomTool("hvector operation tests") {}

	virtual void Run() override
	{
		const unsigned int MAX_RAND_INT = 600;

		const unsigned int NUM_LOOP = 2000000;
		for (int i = 0; i < NUM_LOOP; ++i)
		{
			COUNT_DETAIL_TIME;

			if (i % 100 == 0)
			{
				ShowProgress(i * 1.0f / NUM_LOOP);
			}

			// build random numbers
			std::array<Types::F32, 9> comfArr;
			for (auto& comf : comfArr)
			{
				comf = 1.0f * mtr.Random(MAX_RAND_INT) / (mtr.Random(MAX_RAND_INT) + 1);
			}

			// some number will be divide, so next codes ensure they will never be zero.
			std::array<int, 4> indicesCannotBeZero = { 4, 5, 6, 8 };
			for (auto index : indicesCannotBeZero)
			{
				while (comfArr[index] == 0.0f)
				{ 
					comfArr[index] = 1.0f * mtr.Random(MAX_RAND_INT) + 1 / (mtr.Random(MAX_RAND_INT) + 1);
				}
			}

			hvector cmp1(comfArr[0], comfArr[1], comfArr[2], comfArr[3]);
			hvector cmp2(comfArr[4], comfArr[5], comfArr[6], comfArr[7]);

			// declare some hvector for result comparison
			hvector 
				h_plus_h(comfArr[0] + comfArr[4], comfArr[1] + comfArr[5], comfArr[2] + comfArr[6], comfArr[3]),
				h_minu_h(comfArr[0] - comfArr[4], comfArr[1] - comfArr[5], comfArr[2] - comfArr[6], comfArr[3]),
				h_mult_h(comfArr[0] * comfArr[4], comfArr[1] * comfArr[5], comfArr[2] * comfArr[6], comfArr[3]),
				h_dive_h(comfArr[0] / comfArr[4], comfArr[1] / comfArr[5], comfArr[2] / comfArr[6], comfArr[3]),
				h_mult_s(comfArr[0] * comfArr[8], comfArr[1] * comfArr[8], comfArr[2] * comfArr[8], comfArr[3]),
				s_mult_h(comfArr[0] * comfArr[8], comfArr[1] * comfArr[8], comfArr[2] * comfArr[8], comfArr[3]),
				h_dive_s(comfArr[0] / comfArr[8], comfArr[1] / comfArr[8], comfArr[2] / comfArr[8], comfArr[3]);

			TEST_ASSERT((cmp1 + cmp2) == h_plus_h);

			TEST_ASSERT((cmp1 - cmp2) == h_minu_h);

			TEST_ASSERT((cmp1 * cmp2) == h_mult_h);

			TEST_ASSERT((cmp1 / cmp2) == h_dive_h);

			hvector temp = cmp1;
			temp += cmp2;
			TEST_ASSERT(temp == h_plus_h);

			temp = cmp1;
			temp -= cmp2;
			TEST_ASSERT(temp == h_minu_h);

			temp = cmp1;
			temp *= cmp2;
			TEST_ASSERT(temp == h_mult_h);

			temp = cmp1;
			temp /= cmp2;
			TEST_ASSERT(temp == h_dive_h);

			// Next codes is about operations between hvector and scalar
			// hvector * scalar
			TEST_ASSERT((cmp1 * comfArr[8]) == h_mult_s);

			// scalar * hvector
			TEST_ASSERT((comfArr[8] * cmp1) == s_mult_h);

			// hvector / scalar
			TEST_ASSERT((cmp1 / comfArr[8]) == h_dive_s);

			// hvector *= scalar
			temp = cmp1;
			temp *= comfArr[8];
			TEST_ASSERT(temp == h_mult_s);

			// hvector /= scalar
			temp = cmp1;
			temp /= comfArr[8];
			TEST_ASSERT(temp == h_dive_s);

			// Next codes ensure that the comparison is not always return true.
			// next comparison will all return false.
			TEST_ASSERT(!(h_mult_h == h_dive_h));
			TEST_ASSERT(!(h_plus_h == h_minu_h));

		}// end for
	}// end Run
};

// assembly the cases
using BasicTypesTestSuit_Runable = 
BasicTypesTestSuit_BASE
<
	CaseBasicVector2, 
	TestVector2Case, 
	CaseForVector3, 
	CaseForRGBA, 
	CaseForRGB, 
	CaseForRGBAndRGBA, 
	CaseForMaterialProperty,
	CaseForVector3Normalize,
	CaseForHvector
>;


