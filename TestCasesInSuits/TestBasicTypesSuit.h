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

class TestVector2Case :public TestSuit::Case
{
public:
	TestVector2Case() : Case("test vector2") {}
	
	virtual void Run() override
	{
		RandomTool::MTRandom mtr;
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
	}// end fun
};

using BasicTypesTestSuit_Runable = BasicTypesTestSuit_BASE<CaseBasicVector2, TestVector2Case>;


