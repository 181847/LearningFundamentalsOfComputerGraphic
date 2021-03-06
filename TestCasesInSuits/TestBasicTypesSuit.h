#pragma once
/*!
    \brief
    In this head file, I'm going to transfer all the tests in Project'TestBasicTypes'
    into using TestSuit modules.
    [2018/7/25 9:22]
*/
#include "CommonHeaders.h"
#include <iomanip>

using namespace CommonClass;

template<typename ... CASE_TYPE_LIST>
class BasicTypesTestSuit_BASE: public SuitForPipline<CASE_TYPE_LIST...>
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

class TestVector2Case :public CaseForPipline
{
public:
    TestVector2Case() : CaseForPipline("test vector2") {}
    
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
                vector2(comf1 * comf3, comf2 * comf4));

            TEST_ASSERT(
                (CommonClass::dotProd(cmp1, cmp2))
                ==
                (comf1 * comf3 + comf2 * comf4));
        }// end for
    }// end Run
};

class CaseForVector3 : public CaseForPipline
{
public:
    CaseForVector3():CaseForPipline("test vector3"){}
    
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
                vector3(comfArr[0] * comfArr[3], comfArr[1] * comfArr[4], comfArr[2] * comfArr[5]));

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

class CaseForRGBA : public CaseForPipline
{
public:
    CaseForRGBA() :CaseForPipline("test RGBA") {}

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
                comf1 * cmp1    // switch RGBA and scalar
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

class CaseForRGB : public CaseForPipline
{
public:
    CaseForRGB() :CaseForPipline("test RGB") {}

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

class CaseForRGBAndRGBA : public CaseForPipline
{
public:
    CaseForRGBAndRGBA() :CaseForPipline("test casting between RGB and RGBA") {}

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

            RGBA    color4(comuArr[0], comuArr[1], comuArr[2], comuArr[3]);
            RGB        color3(comuArr[4], comuArr[5], comuArr[6]);

            TEST_ASSERT(Cast(color4)                == RGB    (comuArr[0], comuArr[1], comuArr[2]));
            TEST_ASSERT(Cast(color3)                == RGBA    (comuArr[4], comuArr[5], comuArr[6], RGBA::ALPHA_CHANNEL_OPAQUE));
            TEST_ASSERT(Cast(color3, comuArr[7])    == RGBA    (comuArr[4], comuArr[5], comuArr[6], comuArr[7]));
        }
    }
};

class CaseForMaterialProperty : public CaseForPipline
{
public:
    CaseForMaterialProperty() :CaseForPipline("check material construct properties") {}

    virtual void Run() override
    {
        const unsigned int MAX_REFLECT_INDEX = 5;

        for (int i = 0; i < 20; ++i)
        {
            vector3 color = GetRandomVector3ForRGB();
            vector3 rFresnel0 = GetRandomVector3ForRGB();
            unsigned int randomReflectIndex = 1 + mtr.Random(MAX_REFLECT_INDEX - 1);

            Material mat1;                              // default mat, white color, fresnel coefficient is zero
            Material mat2(color, rFresnel0);            // specific the fresnel coefficient with RGB.
            Material mat3(color, randomReflectIndex);   // calculate fresnel coefficient using reflect index.

            TEST_ASSERT(mat1.m_kDiffuse            == vector3::WHITE);
            TEST_ASSERT(mat1.m_rFresnel_0        == vector3::BLACK);

            TEST_ASSERT(mat2.m_kDiffuse            == color);
            TEST_ASSERT(mat2.m_rFresnel_0        == rFresnel0);

            // manully calculate the fresnel coefficient.
            Types::F32 reflectIndexToFresnel0 = (randomReflectIndex - 1) * 1.0f / (randomReflectIndex + 1);
            reflectIndexToFresnel0 *= reflectIndexToFresnel0;

            TEST_ASSERT(mat3.m_kDiffuse            == color);
            TEST_ASSERT(mat3.m_rFresnel_0        == vector3(reflectIndexToFresnel0, reflectIndexToFresnel0, reflectIndexToFresnel0));

        }
    }
};

class CaseForVector3Normalize : public CaseForPipline
{
public:
    CaseForVector3Normalize() : CaseForPipline("test vector3 normalize") {}

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

class CaseForHvector : public CaseForPipline
{
public:
    CaseForHvector() :CaseForPipline("vector4 operation tests") {}

    virtual void Run() override
    {
        const unsigned int MAX_RAND_INT = 600;

        const unsigned int NUM_LOOP = 2000;
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

            vector4 cmp1(comfArr[0], comfArr[1], comfArr[2], comfArr[3]);
            vector4 cmp2(comfArr[4], comfArr[5], comfArr[6], comfArr[7]);

            // declare some vector4 for result comparison
            vector4 
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

            vector4 temp = cmp1;
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

            // Next codes is about operations between vector4 and scalar
            // vector4 * scalar
            TEST_ASSERT((cmp1 * comfArr[8]) == h_mult_s);

            // scalar * vector4
            TEST_ASSERT((comfArr[8] * cmp1) == s_mult_h);

            // vector4 / scalar
            TEST_ASSERT((cmp1 / comfArr[8]) == h_dive_s);

            // vector4 *= scalar
            temp = cmp1;
            temp *= comfArr[8];
            TEST_ASSERT(temp == h_mult_s);

            // vector4 /= scalar
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

class CaseForTransformConstruct : public CaseForPipline
{
public:
    CaseForTransformConstruct() :CaseForPipline("transform construct tests") {}

    virtual void Run() override
    {
        const unsigned int MAX_RAND_INT = 600;
        
        for (int i = 0; i < 200; ++i)
        {
            COUNT_DETAIL_TIME;

            auto comfArr = GetRandomFloatArray<9>(static_cast<Types::F32>(MAX_RAND_INT));

            // Translation 
            Transform trl = Transform::Translation(comfArr[0], comfArr[1], comfArr[2]);
            Transform trl_compare(
                1.0f, 0.0f, 0.0f, comfArr[0],
                0.0f, 1.0f, 0.0f, comfArr[1],
                0.0f, 0.0f, 1.0f, comfArr[2],
                0.0f, 0.0f, 0.0f, 1.0f);

            TEST_ASSERT(trl == trl_compare);

            // Rotation transformation
            const Types::F32 cosTheta = std::cosf(comfArr[3]),
                             sinTheta = std::sinf(comfArr[3]);

            Transform rtx = Transform::RotationX(comfArr[3]),
                      rty = Transform::RotationY(comfArr[3]),
                      rtz = Transform::RotationZ(comfArr[3]);
            
            Transform rtx_compare(
                        1.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, cosTheta, -sinTheta, 0.0f,
                        0.0f, sinTheta, cosTheta, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f
                      ),
                      rty_compare(
                          cosTheta, 0.0f, sinTheta, 0.0f,
                          0.0f, 1.0f, 0.0f, 0.0f,
                          -sinTheta, 0.0f, cosTheta, 0.0f,
                          0.0f, 0.0f, 0.0f, 1.0f
                      ),
                      rtz_compare(
                          cosTheta, -sinTheta, 0.0f, 0.0f,
                          sinTheta, cosTheta, 0.0f, 0.0f,
                          0.0f, 0.0f, 1.0f, 0.0f,
                          0.0f, 0.0f, 0.0f, 1.0f
                      );
            
            TEST_ASSERT(rtx == rtx_compare);
            TEST_ASSERT(rty == rty_compare);
            TEST_ASSERT(rtz == rtz_compare);

            TEST_ASSERT(rtx != trl);
            TEST_ASSERT(rty != trl);
            TEST_ASSERT(rtz != trl);
            TEST_ASSERT(rtx != rty);
            TEST_ASSERT(rtz != rty);

            // Viewport transformation
            const Types::U32 
                comu1(mtr.Random(MAX_RAND_INT)),
                comu2(mtr.Random(MAX_RAND_INT)),
                comu3(mtr.Random(MAX_RAND_INT)), 
                comu4(mtr.Random(MAX_RAND_INT));

            // boundries
            const Types::U32 left = comu1, right = comu1 + comu2, bottom = comu3, top = comu3 + comu4;

            Transform viewt = Transform::Viewport(static_cast<Types::F32>(left), static_cast<Types::F32>(right), static_cast<Types::F32>(bottom), static_cast<Types::F32>(top));

            // build nine vector4 for view port transformation test.
            std::array<vector4, 9> normalizedPos;
            std::array<Types::F32, 3> constFloats = { -1.0f, 0.0f, 1.0f };
            for (int i = 0; i < 3; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    normalizedPos[i * 3 + j] = vector4(constFloats[i], constFloats[j], 0.0f, 1.0f);
                }
            }

            // build nine vector4 which should be the vectors transformed by the Viewport transformation.
            std::array<vector4, 9> screenPos;
            std::array<Types::F32, 3> horizontalPoses = {left - 0.5f, (left + right) * 0.5f , right + 0.5f};
            std::array<Types::F32, 3> verticalPoses   = {bottom - 0.5f, (bottom + top) * 0.5f , top + 0.5f};
            for (int i = 0; i < 3; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    screenPos[i * 3 + j] = vector4(horizontalPoses[i], verticalPoses[j], 0.0f, 1.0f);
                }
            }

            // check whether it is true that "normalizedPos" can be transformed to "screenPos" by "viewt"
            for (int i = 0; i < 9; ++i)
            {
                vector4 convertToViewport(viewt * normalizedPos[i]);
                TEST_ASSERT(AlmostEqual(screenPos[i], convertToViewport));
            }// end for int
        }// end for big loop test
    }// end Run()
};

class CaseForTranslateAndRotateHvector : public CaseForPipline
{
public:
    CaseForTranslateAndRotateHvector() : CaseForPipline("translate and rotate vector4") {}
    
    virtual void Run() override 
    {
        const unsigned int MAX_RAND_INT = 600;

        for (int i = 0; i < 200; ++i)
        {
            COUNT_DETAIL_TIME;

            auto comfArr = GetRandomFloatArray<9>(static_cast<Types::F32>(MAX_RAND_INT));

            Transform trl = Transform::Translation(comfArr[0], comfArr[1], comfArr[2]);    // translation matrix

            vector4 vt(comfArr[4], comfArr[5], comfArr[6]);    // the vector used to test translation matrix 

            TEST_ASSERT(
                vector4(comfArr[4] + comfArr[0], comfArr[5] + comfArr[1], comfArr[6] + comfArr[2])
                == (trl * vt));

            const Types::F32 pi_div_2 = Types::Constant::PI_F * 0.5f;

            // build rotations about axis of x/y/z
            const Transform
                rotX = Transform::RotationX(pi_div_2),
                rotY = Transform::RotationY(pi_div_2),
                rotZ = Transform::RotationZ(pi_div_2);
            std::array<Transform, 3> rotations = { rotX, rotY, rotZ }; // each one rotate pi/2 about x,y,z


            std::array<vector4, 8> midVs;   // eight vector point at eight quadrants of the space.
            for (unsigned int i = 0; i < midVs.size(); ++i)
            {
                Types::F32 cx, cy, cz;

                cx = (i & 4) ? +1.0f : -1.0f;
                cy = (i & 2) ? +1.0f : -1.0f;
                cz = (i & 1) ? +1.0f : -1.0f;

                midVs[i] = vector4(cx, cy, cz, 1.0f);
            }


            /*!
                \brief preRotOrder (previous rotation order)
                assist rotation check, used with array "midVs" and "rotations"
                For example:
                preRotOrder[0][0] = 2 means that rotating vector "midVs[0]" with "rotations[0]" will end up with "midVs[2]"
                preRotOrder[2][1] = 3 means that rotating vector "midVs[2]" with "rotations[1]" will end up with "midVs[3]"
            */
            const unsigned preRotOrder[8][3] = {
                { 2, 1, 4 },
                { 0, 5, 5 },
                { 3, 3, 0 },
                { 1, 7, 1 },
                { 6, 0, 6 },
                { 4, 4, 7 },
                { 7, 2, 2 },
                { 5, 6, 3 }
            };

            /*!
                \brief for each vector of "midVs", rotate them with matrixes in "rotations"
            */
            for (unsigned int i = 0; i < midVs.size(); ++i)
            {
                for (unsigned int j = 0; j < rotations.size(); ++j)
                {
                    TEST_ASSERT(AlmostEqual(rotations[j] * midVs[i], midVs[preRotOrder[i][j]], 1e-7f));

                    // ensure the test not always return true;
                    TEST_ASSERT(!AlmostEqual(rotations[j] * midVs[i] + vector4(0.5f, 0.0f, 0.0f), midVs[preRotOrder[i][j]], 1e-7f));
                }
            }

            // old rotations tests, just rotate axis x/y/z and get the results of x -> y -> z -> x...
            vector4 axisx(1.0f, 0.0f, 0.0f, 1.0f);
            vector4 axisy(0.0f, 1.0f, 0.0f, 1.0f);
            vector4 axisz(0.0f, 0.0f, 1.0f, 1.0f);
            vector4 axisx_compare = rotY * axisz;
            vector4 axisy_compare = rotZ * axisx;
            vector4 axisz_compare = rotX * axisy;

            TEST_ASSERT(AlmostEqual(axisx, axisx_compare, 1e-7f));
            TEST_ASSERT(AlmostEqual(axisy, axisy_compare, 1e-7f));
            TEST_ASSERT(AlmostEqual(axisz, axisz_compare, 1e-7f));

            // next few check ensure that test is not always return true.
            TEST_ASSERT(!AlmostEqual(axisx, axisy_compare, 1e-7f));
            TEST_ASSERT(!AlmostEqual(axisy, axisz_compare, 1e-7f));
            TEST_ASSERT(!AlmostEqual(axisz, axisx_compare, 1e-7f));

        }// end for loop
    }// end Run()
};

class CaseForF32Buffer : public CaseForPipline
{
public:
    CaseForF32Buffer() : CaseForPipline("translate and rotate vector4") {}

    virtual void Run() override
    {
        const unsigned int MAX_RAND_INT = 100;
        const unsigned int SIZE_OF_FLOAT = sizeof(Types::F32);

        std::vector<Types::F32> storeF32s;

        auto checkWithStdVector = [this, &storeF32s, &SIZE_OF_FLOAT](std::unique_ptr<F32Buffer> buffer)->void {
            TEST_ASSERT(storeF32s.size() * SIZE_OF_FLOAT == buffer->GetSizeOfByte());
            const unsigned int NUM_CHECKED_FLOATS = buffer->GetSizeOfByte() / SIZE_OF_FLOAT;
            Types::F32 * pBuffer = reinterpret_cast<Types::F32*>(buffer->GetBuffer());


            for (unsigned int i = 0; i < NUM_CHECKED_FLOATS; ++i)
            {
                TEST_ASSERT(storeF32s[i] == pBuffer[i]);
            }
        };

        for (int i = 0; i < 30; ++i)
        {
            const unsigned int NUM_FLOATS = mtr.Random(MAX_RAND_INT);

            storeF32s.resize(NUM_FLOATS);
            auto floatBuffer = std::make_unique<F32Buffer>(NUM_FLOATS * SIZE_OF_FLOAT);

            Types::F32 * pBuffer = reinterpret_cast<Types::F32*>(floatBuffer->GetBuffer());
            for (unsigned int i = 0; i < NUM_FLOATS; ++i)
            {
                Types::F32 comf1(1.0f * mtr.Random(MAX_RAND_INT) / (mtr.Random(MAX_RAND_INT) + 1));
                pBuffer[i] = comf1;
                storeF32s[i] = comf1;
            }// end for i < NUM_FLOATS

            checkWithStdVector(std::move(floatBuffer));
        }// end for i < 30
    }// end Run()
};

class CaseForCoordinateFrameConstruct : public CaseForPipline
{
public:
    CaseForCoordinateFrameConstruct() : CaseForPipline("coordinate frame construct") {}

    virtual void Run() override
    {

        const unsigned int SCALE_FLOAT = 2;

        std::array<Types::F32, 9> rfarr;

        for (int i = 0; i < 20; ++i)
        {
            for (auto & randFloat : rfarr)
            {
                randFloat = SCALE_FLOAT * (mtr.Random() * 2.0f - 1.0f);
            }

            vector3 tu(rfarr[0], rfarr[1], rfarr[2]);
            vector3 tv(rfarr[3], rfarr[4], rfarr[5]);
            vector3 te(rfarr[6], rfarr[7], rfarr[8]);

            CoordinateFrame tcf(tu, tv, te);

            TEST_ASSERT(AlmostPerpendicular(tcf.m_u, tcf.m_v, 1e-6f));
            TEST_ASSERT(AlmostPerpendicular(tcf.m_u, tcf.m_w, 1e-6f));
            TEST_ASSERT(AlmostPerpendicular(tcf.m_v, tcf.m_w, 1e-6f));

            TEST_ASSERT(tcf.m_e == te);
        }// end for
    }// end Run()
};

class CaseForOrthographicTransformationTest : public CaseForPipline
{
public:
    CaseForOrthographicTransformationTest() : CaseForPipline("orthographic transformation test") {}

    virtual void Run() override
    {
        const unsigned int MAX_INT = 1080;

        for (int loopCount = 0; loopCount < 20; ++loopCount)
        {

            // six plane parameterss
            const Types::F32 
                LEFT    (-(mtr.Random() - 0.1f) * MAX_INT),
                RIGHT   ( (mtr.Random() + 0.1f) * MAX_INT),
                BOTTOM  (-(mtr.Random() - 0.1f) * MAX_INT),
                TOP     ( (mtr.Random() + 0.1f) * MAX_INT),
                NEAR    ((-mtr.Random() - 0.1f) * MAX_INT),
                FAR     ( NEAR - (mtr.Random() + 0.1f) * MAX_INT); // far plane is more negative than near plane

            // the tested transformation matrix
            Transform orthTransformation = Transform::OrthographicTransOG(LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR);

            // the array help to build hvectors before transformation
            std::array<Types::F32, 3> 
                X_VALUES = {LEFT,   0.5f * (LEFT + RIGHT), RIGHT}, 
                Y_VALUES = {BOTTOM, 0.5f * (BOTTOM + TOP), TOP},
                Z_VALUES = {NEAR,   0.5f * (NEAR + FAR),   FAR };

            // the array help to test hvectors after transformation
            std::array<Types::F32, 3>
                X_NDC = { -1.0f, 0.0f, +1.0f },
                Y_NDC = { -1.0f, 0.0f, +1.0f },
                Z_NDC = {  0.0f, 0.5f, +1.0f };// notice: Z_NDC, +1.0f is for near plane, negative is for far plane

            std::array<vector4, 27> testHvectors;   // the vectors to be transformed
            std::array<vector4, 27> expectedVec;    // the expected result of the transformation result
        
            for (unsigned int i = 0; i < testHvectors.size(); ++i)
            {
                const unsigned int 
                    indexX = i % 3,
                    indexY = (i / 3) % 3,
                    indexZ = (i / 9) % 3;

                testHvectors[i] = vector4(X_VALUES[indexX], Y_VALUES[indexY], Z_VALUES[indexZ]);
                expectedVec[i]  = vector4(X_NDC[indexX],    Y_NDC[indexY],    Z_NDC[indexZ]);
            }// end for build hvectors

            for (unsigned int i = 0; i < testHvectors.size(); ++i)
            {
                vector4 transResult(orthTransformation * testHvectors[i]);
                TEST_ASSERT(AlmostEqual(transResult, expectedVec[i], 1e-6f));

                // transform another vector4, but check with the same expectedVec[i]
                // CREATE AN ERROR
                vector4 mustFailResult(orthTransformation * testHvectors[(i + 4) % testHvectors.size()]);

                // ensure the result is not always success
                TEST_ASSERT( ! AlmostEqual(mustFailResult, expectedVec[i], 1e-6f));
            }// end for testHvectors

        }// end for loopCount
    }
};

class CaseForTransformationMatrixConcation : public CaseForPipline
{
public:
    CaseForTransformationMatrixConcation() : CaseForPipline("transformation matrix concation") {}

    virtual void Run() override
    {
        const unsigned int MAX_RAND_INT = 600;

        // the transform is default to be a unit matrix.
        const Transform UNIT_MATRIX;
        
        for (int i = 0; i < 200; ++i)
        {
            COUNT_DETAIL_TIME;

            auto comfArr = GetRandomFloatArray<9>();

            // NOTICE, when build those matrix, multiply them by the unit matrix.
            Transform trl = UNIT_MATRIX * Transform::Translation(comfArr[0], comfArr[1], comfArr[2]);    // translation matrix

            vector4 vt(comfArr[4], comfArr[5], comfArr[6]);    // the vector used to test translation matrix 

            TEST_ASSERT(
                vector4(comfArr[4] + comfArr[0], comfArr[5] + comfArr[1], comfArr[6] + comfArr[2]) 
                == (trl * vt));
            
            const Types::F32 pi_div_2 = Types::Constant::PI_F * 0.5f;

            // build rotations about axis of x/y/z

            // NOTICE, when build those matrix, multiply them by the unit matrix.
            const Transform
                rotX = UNIT_MATRIX * Transform::RotationX(pi_div_2),
                rotY = UNIT_MATRIX * Transform::RotationY(pi_div_2),
                rotZ = UNIT_MATRIX * Transform::RotationZ(pi_div_2);
            std::array<Transform, 3> rotations = { rotX, rotY, rotZ }; // each one rotate pi/2 about x,y,z


            std::array<vector4, 8> midVs;   // eight vector point at eight quadrants of the space.
            for (unsigned int i = 0; i < midVs.size(); ++i)
            {
                Types::F32 cx, cy, cz;
                
                cx = (i & 4) ? +1.0f : -1.0f;
                cy = (i & 2) ? +1.0f : -1.0f;
                cz = (i & 1) ? +1.0f : -1.0f;

                midVs[i] = vector4(cx, cy, cz, 1.0f);
            }

            
            /*!
                \brief preRotOrder (previous rotation order)
                assist rotation check, used with array "midVs" and "rotations"
                For example:
                preRotOrder[0][0] = 2 means that rotating vector "midVs[0]" with "rotations[0]" will end up with "midVs[2]"
                preRotOrder[2][1] = 3 means that rotating vector "midVs[2]" with "rotations[1]" will end up with "midVs[3]"
            */
            const unsigned preRotOrder[8][3] = {
                { 2, 1, 4 },
                { 0, 5, 5 },
                { 3, 3, 0 },
                { 1, 7, 1 },
                { 6, 0, 6 },
                { 4, 4, 7 },
                { 7, 2, 2 },
                { 5, 6, 3 }
            };

            /*!
                \brief for each vector of "midVs", rotate them with matrixes in "rotations"
            */
            for (unsigned int i = 0; i < midVs.size(); ++i)
            {
                for (unsigned int j = 0; j < rotations.size(); ++j)
                {
                    TEST_ASSERT(AlmostEqual(rotations[j] * midVs[i], midVs[preRotOrder[i][j]], 1e-7f));

                    // ensure the test not always return true;
                    TEST_ASSERT( ! AlmostEqual(rotations[j] * midVs[i] + vector4(0.5f, 0.0f, 0.0f), midVs[preRotOrder[i][j]], 1e-7f));
                }
            }
            
            // old rotations tests, just rotate axis x/y/z and get the results of x -> y -> z -> x...
            vector4 axisx(1.0f, 0.0f, 0.0f, 1.0f);
            vector4 axisy(0.0f, 1.0f, 0.0f, 1.0f);
            vector4 axisz(0.0f, 0.0f, 1.0f, 1.0f);
            vector4 axisx_compare = rotY * axisz;
            vector4 axisy_compare = rotZ * axisx;
            vector4 axisz_compare = rotX * axisy;

            TEST_ASSERT(AlmostEqual(axisx, axisx_compare, 1e-7f));
            TEST_ASSERT(AlmostEqual(axisy, axisy_compare, 1e-7f));
            TEST_ASSERT(AlmostEqual(axisz, axisz_compare, 1e-7f));

            // next few check ensure that test is not always return true.
            TEST_ASSERT( ! AlmostEqual(axisx, axisy_compare, 1e-7f));
            TEST_ASSERT( ! AlmostEqual(axisy, axisz_compare, 1e-7f));
            TEST_ASSERT( ! AlmostEqual(axisz, axisx_compare, 1e-7f));
            
        }
    }
};

class CaseForFixPointNumber : public CaseForPipline
{
public:
    CaseForFixPointNumber() : CaseForPipline("Fix Point Number basic test") {}

    virtual void Run() override
    {
        FixPointNumber a(100);
        FixPointNumber b(100.0f);

        TEST_ASSERT(a.ToFloat() == 100.0f);
        TEST_ASSERT(b.ToFloat() == 100.0f);

        RandomTool::MTRandom mtr;

        /*!
        \brief return a random float in a large range.
        */
        auto RandFloat = [&mtr]()->Types::F32 {
            const int MAX_INT = 180;
            return (mtr.Random() * 2.0f - 1.0f) * MAX_INT;
        };

        const unsigned int NUM_FLOAT_PAIRS = 16;
        std::array<Types::F32, NUM_FLOAT_PAIRS> farr1;
        std::array<Types::F32, NUM_FLOAT_PAIRS> farr2;
        std::array<FixPointNumber, NUM_FLOAT_PAIRS> fparr1;
        std::array<FixPointNumber, NUM_FLOAT_PAIRS> fparr2;
        for (int i = 0; i < NUM_FLOAT_PAIRS; ++i)
        {
            farr1[i] = RandFloat();
            farr2[i] = RandFloat();

            fparr1[i] = FixPointNumber(farr1[i]);
            fparr2[i] = FixPointNumber(farr2[i]);
        }

        for (int i = 0; i < NUM_FLOAT_PAIRS; ++i)
        {
            const Types::F32 correctResult = farr1[i] * farr2[i];
            const Types::F32 testResult = (fparr1[i] * fparr2[i]).ToFloat();
            TEST_ASSERT(MathTool::AlmostEqual(correctResult, testResult, 0.01f));
        }

        // the fixed point number is not completed, so this test will always failed, 
        // until you fix all the bugs.
        std::cout << "the fixed point number is not completed, so this test will always failed, until you fix all the bugs.\n";
        CountOneError();
    }
};

class CaseForFloatFoundFind : public CaseForPipline
{
public:
    CaseForFloatFoundFind() : CaseForPipline("float bound find") {}

    /*!
        \brief this function is for floating point number round error analysis.
    */
    template<typename RESULT_T, typename LONG_RESULT_T, typename BOUND_T>
    void ShowNumberInBound(const RESULT_T value, const RESULT_T errorBound, const LONG_RESULT_T preciseValue, const BOUND_T low, const BOUND_T high)
    {
        std::cout
            << std::setprecision(std::numeric_limits<LONG_RESULT_T>::digits10 + 1) // remember to include <iomanip>
            << "||||\nlow resolution number = " << value << std::endl
            << "error bound           = " << errorBound << std::endl
            << "high precise number   = " << preciseValue << std::endl
            << "bounded interval      = [ " << low << " , " << high << " ]" << std::endl
            << "||||\n" << std::endl;
    }

    virtual void Run() override
    {
        const unsigned int MAX_INT = 100;

        for (int loopTest = 0; loopTest < 20; ++loopTest)
        {

            float a = mtr.Random() * MAX_INT;
            float b = mtr.Random() * MAX_INT;
            long double la = a, lb = b;

            const int sizeofDouble = sizeof(long double);

            float c = a * b;
            long double lc = la * lb;

            float errorBound = std::abs(c) * gamma(1);

            float low = c - errorBound, high = c + errorBound;

            // ensure the bound include the more precise value which is processed by production of double.
            TEST_ASSERT(low <= lc && lc <= high);

            ShowNumberInBound(c, errorBound, lc, low, high);

            // next codes will check whether the error bound caculateion still apply to sqrt operation
            double lSqrtC = std::sqrt(lc);

            float sqrtC = std::sqrt(c);

            float sqrtErrorBound = std::abs(sqrtC) * gamma(2);

            float sqrtLow = sqrtC - sqrtErrorBound, sqrtHigh = sqrtC + sqrtErrorBound;

            TEST_ASSERT(sqrtLow <= lSqrtC && lSqrtC <= sqrtHigh);

            ShowNumberInBound(sqrtC, sqrtErrorBound, lSqrtC, sqrtLow, sqrtHigh);

        } // end for loopTest
    }// end Run()
};

class CaseForEFloat : public CaseForPipline
{
public:
    CaseForEFloat() : CaseForPipline("EFloat tool test") {}

    virtual void Run() override
    {
        const unsigned int MAX_INT = 100;

        const unsigned int MAX_NUM = 50;
    
        for (int numLoop = 0; numLoop < 20; ++numLoop)
        {
            ShowProgress(numLoop * 1.0f / 20);

            // original number array
            std::array<float, MAX_NUM> farr;

            // assign numbers to the Array "farr".
            farr[0] = 0.0f;
            farr[1] = -0.0f;
            farr[2] = mtr.Random();

            const int POSITIVE_INF = 3;
            const int NEGATIVE_INF = 4;
            const int POSITIVE_MAX = 5;
            const int NEGATIVE_MAX = 6;
            farr[POSITIVE_INF] = +std::numeric_limits<float>::infinity();
            farr[NEGATIVE_INF] = -std::numeric_limits<float>::infinity();
            farr[POSITIVE_MAX] = +std::numeric_limits<float>::max();
            farr[NEGATIVE_MAX] = -std::numeric_limits<float>::max();

            for (unsigned int i = NEGATIVE_MAX + 1; i < farr.size(); ++i)
            {
                farr[i] = (mtr.Random() - 0.5f) * 2.0f * MAX_INT;
            }

            // float up
            std::array<float, farr.size()> fuparr;

            // float down
            std::array<float, farr.size()> fdownarr;

            for (unsigned int i = 0; i < farr.size(); ++i)
            {
                //BREAK_POINT_IF(i == 4);
                fuparr[i] = NextFloatUp(farr[i]);
                fdownarr[i] = NextFloatDown(farr[i]);
            }

            for (unsigned int i = 0; i < farr.size(); ++i)
            {
                if (i == POSITIVE_INF)
                {
                    TEST_ASSERT(fdownarr[i] < farr[i]);
                    TEST_ASSERT(std::isinf(fuparr[i]));// move +inf up will stay +inf
                }
                else if (i == NEGATIVE_INF)
                {
                    TEST_ASSERT(std::isinf(fdownarr[i]));// move -inf down will stay -inf
                    TEST_ASSERT(fuparr[i] > farr[i]);
                }
                else if (i == POSITIVE_MAX)
                {
                    TEST_ASSERT(fdownarr[i] < farr[i]);
                    TEST_ASSERT(std::isinf(fuparr[i]) && fuparr[i] > 0.);// move +max up will end with +inf
                }
                else if (i == NEGATIVE_MAX)
                {
                    TEST_ASSERT(std::isinf(fdownarr[i]) && fdownarr[i] < 0.);// move -max down will end with -inf
                    TEST_ASSERT(fuparr[i] > farr[i]);
                }
                else // normal test
                {
                    TEST_ASSERT(fuparr[i] > farr[i]);
                    TEST_ASSERT(fdownarr[i] < farr[i]);
                }// end else normal test

            }// end for test farr.


        }// end for numLoop
    }
};

class CaseForEFloatConstructTest : public CaseForPipline
{
public:
    CaseForEFloatConstructTest() : CaseForPipline("EFloat construct") {}

    virtual void Run() override
    {
        const int MAX_INT = 2;
        const int MAX_NUM = 7;

        for (int numLoop = 0; numLoop < 20; ++numLoop)
        {
            // original number array
            std::array<float, MAX_NUM> farr;
            std::array<EFloat, farr.size()> efarr;

            // assign numbers to the Array "farr".
            farr[0] = 0.0f;
            farr[1] = -0.0f;
            farr[2] = mtr.Random();

            const int POSITIVE_INF = 3;
            const int NEGATIVE_INF = 4;
            const int POSITIVE_MAX = 5;
            const int NEGATIVE_MAX = 6;
            farr[POSITIVE_INF] = +std::numeric_limits<float>::infinity();
            farr[NEGATIVE_INF] = -std::numeric_limits<float>::infinity();
            farr[POSITIVE_MAX] = +std::numeric_limits<float>::max();
            farr[NEGATIVE_MAX] = -std::numeric_limits<float>::max();

            for (unsigned int i = NEGATIVE_MAX + 1; i < farr.size(); ++i)
            {
                farr[i] = (mtr.Random() - 0.5f) * 2.0f * MAX_INT;
                efarr[i] = EFloat(farr[i]);
            }

            for (auto & ef : efarr)
            {
                ef.Check();
            }
        }// end for numLoop
    }
};

class CaseForEFloatOperatorTest : public CaseForPipline
{
public:
    CaseForEFloatOperatorTest() : CaseForPipline("EFloat operator test") {}

    virtual void Run() override
    {
        const int MAX_INT = 2;
        const int MAX_NUM = 7;

        const bool OUTPUT_RESUTL_TO_CONSOLE = false;

        for (int numLoop = 0; numLoop < 30; ++numLoop)
        {
            // original number array
            std::array<std::array<float, MAX_NUM>, 2> farr;
            std::array<std::array<EFloat, MAX_NUM>, 2> efarr;

            // start efloat construction
            for (int i = 0; i < 2; ++i)
            {
                for (int j = 0; j < MAX_NUM; ++j)
                {
                    farr[i][j] = (mtr.Random() - 0.5f) * 2.0f * MAX_INT;
                    efarr[i][j] = EFloat(farr[i][j]);
                }
            }// end for farr[i] construction

            for (int i = 0; i < MAX_NUM; ++i)
            {
                EFloat efResult;
                float floatResult;

                // operator +
                efResult = efarr[0][i] + efarr[1][i];
                floatResult = farr[0][i] + farr[1][i];
                TEST_ASSERT(floatResult == static_cast<Types::F32>(efResult));
                if (OUTPUT_RESUTL_TO_CONSOLE)
                {
                    std::cout << farr[0][i] << " + " << farr[1][i] << " = \n"
                              << efResult << "\n\n";
                }

                // operator -
                efResult = efarr[0][i] - efarr[1][i];
                floatResult = farr[0][i] - farr[1][i];
                TEST_ASSERT(floatResult == static_cast<Types::F32>(efResult));
                if (OUTPUT_RESUTL_TO_CONSOLE)
                {
                    std::cout << farr[0][i] << " - " << farr[1][i] << " = \n"
                        << efResult << "\n\n";
                }

                // operator *
                efResult = efarr[0][i] * efarr[1][i];
                floatResult = farr[0][i] * farr[1][i];
                TEST_ASSERT(floatResult == static_cast<Types::F32>(efResult));
                if (OUTPUT_RESUTL_TO_CONSOLE)
                {
                    std::cout << farr[0][i] << " * " << farr[1][i] << " = \n"
                        << efResult << "\n\n";
                }

                // operator /
                efResult = efarr[0][i] / efarr[1][i];
                floatResult = farr[0][i] / farr[1][i];
                TEST_ASSERT(floatResult == static_cast<Types::F32>(efResult));
                if (OUTPUT_RESUTL_TO_CONSOLE)
                {
                    std::cout << farr[0][i] << " / " << farr[1][i] << " = \n"
                        << efResult << "\n\n";
                }

                // abs
                // using the EFloat from previous divided result, which will have some error bound.
                const EFloat efWithBound = efResult;
                efResult = abs(efWithBound);
                TEST_ASSERT(efResult.LowerBound() >= 0.0f);
                TEST_ASSERT(efResult.UpperBound() >= 0.0f);
                TEST_ASSERT(static_cast<Types::F32>(efResult) >= 0.0f);
                TEST_ASSERT(std::abs(static_cast<Types::F32>(efWithBound)) == static_cast<Types::F32>(efResult));
                if (OUTPUT_RESUTL_TO_CONSOLE)
                {
                    std::cout << "original = " << efWithBound << std::endl
                              << "absolute = " << efResult << std::endl;
                }

                // square root just use the absolute value from the abs(efWithBound)
                const EFloat absWithBound = efResult;
                efResult = sqrt(absWithBound);
                TEST_ASSERT(static_cast<Types::F32>(efResult) == std::sqrt(static_cast<Types::F32>(absWithBound)));
                if (OUTPUT_RESUTL_TO_CONSOLE)
                {
                    std::cout << "square root = " << efResult << std::endl;
                }

                // negative
                efResult = -efWithBound;
                TEST_ASSERT( 
                    - (static_cast<Types::F32>(efWithBound)) 
                    == static_cast<Types::F32>(efResult));
                TEST_ASSERT(
                    - efWithBound.LowerBound() 
                    == efResult.UpperBound());
                TEST_ASSERT(
                    -efWithBound.UpperBound()
                    == efResult.LowerBound());
                if (OUTPUT_RESUTL_TO_CONSOLE)
                {
                    std::cout << "original = " << efWithBound << std::endl
                              << "negative = " << efResult << std::endl;
                }
            }// end for operation tests

        }// end for numLoop
    }
};

/*!
    \brief this struct is for unit test of DebugClient.
*/
struct TestDebugConf
{
    static bool Active;
    enum { ENABLE_CLIENT = 1 };
};
bool TestDebugConf::Active = false;

class CaseForDebugClientTest : public CaseForPipline
{
public:
    CaseForDebugClientTest() : CaseForPipline("DebugClient test") {}

    virtual void Run() override
    {
        // if you have ensure the DebugClient is working,
        // and don't want to be borthered by the interruption of the test inside.
        // set next ENABLE_THIS_TEST to false, and the interruption will be ignored inside.
        const bool ENABLE_THIS_TEST = false;

        if (!ENABLE_THIS_TEST)
        {
            std::cout << "DebugClient test has been skipped, this test will always failed.\n";
            CountOneError();
            return; // return 1 for empethis that there is some unexpected situation.
        }

        // not hit
        DEBUG_CLIENT(TestDebugConf);

        TestDebugConf::Active = true;

        // you should have encounter a assertion here.
        DEBUG_CLIENT(TestDebugConf);

        // disable it again,
        TestDebugConf::Active = false;
        DEBUG_CLIENT(TestDebugConf);

        // using DebugGuard to assist DebugClient
        {
            // inside this scope
            // the TestDebugConf::Active will be set to true
            DebugGuard<TestDebugConf> guard;

            // you should have encounter a assertion here.
            DEBUG_CLIENT(TestDebugConf);

        }// out side that scop TestDebugConf will be set to false.
        DEBUG_CLIENT(TestDebugConf);


        // test that the additional condition can control DebugClient
        {
            DebugGuard<TestDebugConf> guard;

            // event the TestDebugConf::Active is setted to true,
            // we pass the condition false to disable that.
            DEBUG_CLIENT(TestDebugConf, false);

            // you should have encounter a assertion here.
            DEBUG_CLIENT(TestDebugConf, true);
        }
    }
};

class CaseForTriangleRegionBoundary : public CaseForPipline
{
public:
    CaseForTriangleRegionBoundary() : CaseForPipline("triangle region boundary in pipeline") {}

    virtual void Run() override
    {

        // temp struct for line drawing.
        struct SimplePoint
        {
        public:
            vector4 m_position;
            SimplePoint(const vector4& pos)
                :m_position(pos)
            {
                // empty
            }
        };
        // create and config pipeline state object
        auto pso = std::make_unique<PiplineStateObject>();
        pso->m_primitiveType = PrimitiveType::LINE_LIST;
        pso->m_vertexLayout.vertexShaderInputSize = sizeof(vector4);
        pso->m_vertexLayout.pixelShaderInputSize = sizeof(vector4);

        pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->vector4 {
            return vector4::RED;
        };

        vector3 localU(1.0f, 0.0f, 1.0f);
        vector3 localV(0.0f, 1.0f, 0.0f);
        vector3 coordPos(0.0f, 0.0f, 0.0f);
        CoordinateFrame coordFrm(localU, localV, coordPos);
        Transform toWorld = coordFrm.GetTransformLocalToWorld();

        pso->m_vertexShader = [&toWorld](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pDestV)->void {
            memcpy(pDestV, pSrcVertex, sizeof(SimplePoint));
        };

        Viewport viewport;
        viewport.left = 0;
        viewport.right = graphicToolSet.COMMON_PIXEL_WIDTH - 1.0f;
        viewport.bottom = 0;
        viewport.top = graphicToolSet.COMMON_PIXEL_HEIGHT - 1.0f;
        pso->SetViewport(viewport);

        // create and set a pipeline.
        Pipline pipline;
        pipline.SetPSO(std::move(pso));

        // set a backbuffer
        pipline.SetBackBuffer(std::make_shared<Image>(
            graphicToolSet.COMMON_PIXEL_WIDTH,
            graphicToolSet.COMMON_PIXEL_HEIGHT,
            RGBA::WHITE));

        RandomTool::MTRandom mtr;

        const float scaleRandomRange = 3.0f;
        auto RandomScale = [&scaleRandomRange, &mtr]()->float {
            return scaleRandomRange * 2.0f * (mtr.Random() - 1.0f);
        };

        for (int numLoop = 0; numLoop < 200; ++numLoop)
        {
            // make three vertex that shock around the boundary
            // -----------------
            // X               X
            // |               |
            // |               |
            // |               |
            // X               |
            // -----------------
            vector4
                v1(viewport.left + RandomScale(),
                    viewport.top + RandomScale()),
                v2(viewport.right + RandomScale(),
                    viewport.top + RandomScale()),
                v3(viewport.left + RandomScale(),
                    viewport.bottom + RandomScale());
            std::array<const ScreenSpaceVertexTemplate*, 3> pv = {
                reinterpret_cast<const ScreenSpaceVertexTemplate*>(&v1),
                reinterpret_cast<const ScreenSpaceVertexTemplate*>(&v2),
                reinterpret_cast<const ScreenSpaceVertexTemplate*>(&v3)
            };

            std::array<Types::U32, 2> minBound, maxBound;

            {
                COUNT_DETAIL_TIME;
                pipline.FindTriangleBoundary(pv[0], pv[1], pv[2], &minBound, &maxBound);
            }

            const int x = 0, y = 1;
            TEST_ASSERT(minBound[x] >= viewport.left);
            TEST_ASSERT(minBound[y] >= viewport.bottom);
            TEST_ASSERT(maxBound[x] <= viewport.right);
            TEST_ASSERT(maxBound[y] <= viewport.top);

            TEST_ASSERT(minBound[x] < maxBound[x]);
            TEST_ASSERT(minBound[y] < maxBound[y]);
        }// end for numLoop
    }
};

class CaseForEdgeEquation : public CaseForPipline
{
public:
    CaseForEdgeEquation() : CaseForPipline("EdgeEquation test") {}

    virtual void Run() override
    {
        const int SCALE_XY = 200;
        const int numPoints = 40;

        auto RandomXY = [&SCALE_XY, this]()->float {
            return 2.0f * (mtr.Random() - 0.5f) * SCALE_XY;
        };

        for (int numLoop = 0; numLoop < 40; ++numLoop)
        {
            vector4 p1(RandomXY(), RandomXY()), p2(RandomXY(), RandomXY());
            EdgeEquation2D f12(p1, p2);

            // get the left side point, and right side point.
            vector4 direction = p2 - p1;
            vector4 perpendicular = direction;
            std::swap(perpendicular.m_x, perpendicular.m_y);

            std::array<vector4, 2> LRPerp = { perpendicular, perpendicular };
            const int left = 0, right = 1;
            LRPerp[left].m_x *= -1.0f;
            LRPerp[right].m_y *= -1.0f;

            for (int chooseSide = left; chooseSide <= right; ++chooseSide)
            {
                for (int i = 0; i < numPoints; ++i)
                {
                    const float along((mtr.Random() - 0.5f) * 4.0f), side(4.0f * mtr.Random() + 0.001f);
                    vector4 point = p1 + along * direction + side * LRPerp[chooseSide];
                    if (chooseSide == left)
                    {
                        TEST_ASSERT(f12.eval(point.m_x, point.m_y) > 0.0f);
                    }
                    else // choose right
                    {
                        TEST_ASSERT(f12.eval(point.m_x, point.m_y) < 0.0f);
                    }
                }// end for numPoints
            }// end for chooseSide from left to right
        }// end for numLoop
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
    CaseForHvector,
    CaseForTransformConstruct,
    CaseForTranslateAndRotateHvector,
    CaseForF32Buffer,
    CaseForCoordinateFrameConstruct,
    CaseForOrthographicTransformationTest,
    CaseForTransformationMatrixConcation,
    CaseForFixPointNumber,
    CaseForFloatFoundFind,
    CaseForEFloat,
    CaseForEFloatConstructTest,
    CaseForEFloatOperatorTest,
    CaseForDebugClientTest,
    CaseForTriangleRegionBoundary,
    CaseForEdgeEquation
>;


