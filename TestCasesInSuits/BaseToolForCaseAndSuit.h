#pragma once
/*!
    \brief this head file define may useful base class for Case and Suit.
*/

#include "CommonHeaders.h"

using namespace CommonClass;

/*!
    \brief the common environment for pipline, for example: some useful pixel shader, pipline stateObject.
*/
class CommonEnvironment
{
public:
    // temp struct for generous case
    struct SimplePoint
    {
    public:
        hvector m_position;
        /*!
            \brief used to store the sphere ray location information
            x - round index
            y - line index in the round
            z = 0 means it's start point
            z = 1 means it's end point
        */
        hvector m_rayIndex;
        explicit SimplePoint(const hvector& pos)
            :m_position(pos)
        {
            // empty
        }
    };

public:
    // membbers
    int COMMON_PIXEL_WIDTH      = 512;
    int COMMON_PIXEL_HEIGHT     = 512;

    // render boundary can be used in orthographic camera.
    int COMMON_RENDER_LEFT      = -3.0f;
    int COMMON_RENDER_RIGHT     = +3.0f;
    int COMMON_RENDER_BOTTOM    = -3.0f;
    int COMMON_RENDER_TOP       = +3.0f;

public:
    CommonEnvironment()
    {
        // empty
    }

    /*!
        \brief Build common PiplineStateObject for using.
    */
    std::unique_ptr<PiplineStateObject> GetCommonPSO()
    {
        auto pso = std::make_unique<PiplineStateObject>();
        pso->m_primitiveType = PrimitiveType::LINE_LIST;
        pso->m_vertexLayout.vertexShaderInputSize = sizeof(SimplePoint);
        pso->m_vertexLayout.pixelShaderInputSize = sizeof(SimplePoint);
        pso->SetViewport(GetCommonViewport());

        return std::move(pso);
    }

    /*!
        \brief Build common viewport by the default view size defined in CommonEnvironment.
    */
    Viewport GetCommonViewport()
    {
        Viewport viewport;
        viewport.left = 0;
        viewport.right = COMMON_PIXEL_WIDTH - 1;
        viewport.bottom = 0;
        viewport.top = COMMON_PIXEL_HEIGHT - 1;
        return viewport;
    }

    /*!
        \brief Build common Pipline for drawing, 
        all you need do is to set PixelShader/VertexShader, 
        and call drawing function with mesh data.
    */
    std::unique_ptr<Pipline> GetCommonPipline()
    {
        // create and set a pipline.
        auto pipline = std::make_unique<Pipline>();
        pipline->SetPSO(std::move(GetCommonPSO()));

        // set a back buffer
        pipline->SetBackBuffer(std::make_unique<RasterizeImage>(
            COMMON_PIXEL_WIDTH,
            COMMON_PIXEL_HEIGHT,
            RGBA::WHITE));

        return std::move(pipline);
    }
};

/*!
    \brief base case class that have common function for and useful Environment for pipline testing.
    for the Suit class, you should use SuitForPipline.
*/
class CaseForPipline : public TestSuit::Case
{
public:
    using SimplePoint = CommonEnvironment::SimplePoint;
    static_assert(sizeof(SimplePoint) == 2 * sizeof(hvector), "The size of SimplePoint is not matched for this case.");

protected:
    /*!
        \brief the environment pointer for configuration.
    */
    CommonEnvironment * pEnvironment;

    /*!
        \brief the random number generator.
    */
    RandomTool::MTRandom mtr;

    /*!
        \brief a common path for output, which will have a back slash at the end of the string.
    */
    const std::wstring OUTPUT_PATH = L"../OutputTestImage/";

public:
    CaseForPipline(const std::string& caseName) : Case(caseName) {}

    virtual void SetEnvironment(void * pEnvironment) override
    {
        assert(pEnvironment != nullptr);
        this->pEnvironment = reinterpret_cast<CommonEnvironment*>(pEnvironment);
    }

    /*!
        \brief fill in the mesh data with sphere ray line list.
        \param outVetexData return vertex data
        \param outIndices return indices in line list.
    */
    void BuildSphereRayLineData(std::vector<SimplePoint>& outVertexData, std::vector<unsigned int>& outIndices)
    {
        std::vector<SimplePoint> points;
        std::vector<unsigned int> indices;
        unsigned int numIndices = 0;

        const float CENTER_X = 0.0f;
        const float CENTER_Y = 0.0f;
        const float SEGMENT_LENGTH = 0.3f;
        const float START_RADIUS = 0.3;
        const float NUM_ROUNDS = 12;
        const float RADIO_OFFSET = 0.0f; // Add this offset to every angle inside the sphereRay generating process.

                                         // create line segments in sphere ray.
        SphereRay([&numIndices, &outVertexData, &outIndices](HELP_SPHERE_RAY_LAMBDA_PARAMETERS)->void {

            const unsigned int theIndexOfOneLine = 64;
            // only get one line for convenience of debugging
            //if (lineIndex == theIndexOfOneLine)
            //{
            // add start vertex and its index
            SimplePoint start(hvector(x0, y0, 0.0f));
            start.m_rayIndex.m_x = roundIndex;
            start.m_rayIndex.m_y = lineIndex;
            start.m_rayIndex.m_z = 0;
            outVertexData.push_back(start);
            outIndices.push_back(numIndices++);

            // add end vertex and its index
            SimplePoint end(hvector(x1, y1, 0.0f));
            end.m_rayIndex.m_x = roundIndex;
            end.m_rayIndex.m_y = lineIndex;
            end.m_rayIndex.m_z = 1;
            outVertexData.push_back(end);
            outIndices.push_back(numIndices++);
            //}
        },
            CENTER_X, CENTER_Y, // center location
            SEGMENT_LENGTH,     // segment length
            START_RADIUS,       // start radius
            NUM_ROUNDS,         // num rounds
            RADIO_OFFSET        // radio offset
            );// end calling SphereRay()
    }// end BuildSphereRayTriangleMeshData()

    
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

    /*!
        \brief create a random float number between [-MAX_RADIUS, +MAX_RADIUS]
        \param MAX_RADIUS max absolute value of the float.
    */
    Types::F32 RandomFloat(const Types::F32 MAX_RADIUS = 200.0f)
    {
        assert(MAX_RADIUS > 0.0f);
        return (mtr.Random() - 0.5f) * 2.f * MAX_RADIUS;
    }
    
    /*!
        \brief create a float number between [-MAX_RADIUS, 0) ^ (0, +MAX_RADIUS]
        \param MAX_RADIUS max absolute value of the float.
    */
    Types::F32 RandomFloatNotZero(const Types::F32 MAX_RADIUS = 200.0f)
    {
        assert(MAX_RADIUS > 0.0f);
        float ret = 0.0f;
        do 
        {
            ret = RandomFloat(MAX_RADIUS);
        } while (ret == 0.0f);
        return ret;
    }

    /*!
        \brief create an array contain random float  [-200.0f, +200.0f]
        \param MAX_RADIUS max absolute value of the float.
    */
    template<unsigned int COUNT>
    std::array<Types::F32, COUNT> GetRandomFloatArray(const Types::F32 MAX_RADIUS = 200.0f)
    {
        assert(MAX_RADIUS > 0.0f);
        std::array<Types::F32, COUNT> retArr;
        for (auto& number : retArr)
        {
            number = RandomFloat(MAX_RADIUS);
        }
        return retArr;
    }
    
    /*!
        \brief create a float number between [-200.0f, 0) ^ (0, +200.0f]
        \param MAX_RADIUS max absolute value of the float.
    */
    template<unsigned int COUNT>
    std::array<Types::F32, COUNT> GetRandomFloatNotZeroArray(const Types::F32 MAX_RADIUS = 200.0f)
    {
        assert(MAX_RADIUS > 0.0f);
        std::array<Types::F32, COUNT> retArr;
        for (auto& number : retArr)
        {
            number = RandomFloatNotZero(MAX_RADIUS);
        }
        return retArr;
    }

    /*!
        \brief show the image and block the thread until user close the window.
        \param pImg the image to be shown
        \param title the title of the window
    */
    void BlockShowImg(Image * pImg, const std::wstring title)
    {
        assert(pImg != nullptr);
        ImageWindow imgWnd(pImg, title);
        imgWnd.BlockShow();
    }
};

/*!
    \Brief base suit for pipline test, this class will have an special environment, 
    to fully take use of this suit, please inherit CaseForPipline to write you own case.
*/
template<typename ... CASE_TYPE_LIST>
class SuitForPipline: public TestSuit::Suit<CASE_TYPE_LIST...>
{
public:
    virtual void * PrepareBeforeEachCase(TestSuit::Case * pTheCase) override
    {
        CommonEnvironment * pNewEnv = new CommonEnvironment;
        return pNewEnv;
    }

    virtual void FinishEachCase(TestSuit::Case * pTheCase, void * pEnvironment) override
    {
        delete reinterpret_cast<CommonEnvironment*>(pEnvironment);
    }

};

