#pragma once
/*!
    \brief this head file define may useful base class for Case and Suit.
*/

#include "CommonHeaders.h"
#include <filesystem>
#undef RGB

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
        explicit SimplePoint(const hvector& pos = hvector(), const hvector& normal = hvector())
            :m_position(pos), m_rayIndex(normal)
        {
            // empty
        }
    };

public:
    // membbers
    int COMMON_PIXEL_WIDTH      = 512;
    int COMMON_PIXEL_HEIGHT     = 512;

    // render boundary can be used in orthographic camera.
    float COMMON_RENDER_LEFT      = -3.0f;
    float COMMON_RENDER_RIGHT     = +3.0f;
    float COMMON_RENDER_BOTTOM    = -3.0f;
    float COMMON_RENDER_TOP       = +3.0f;

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
        viewport.left = 0.0f;
        viewport.right = static_cast<Types::F32>(COMMON_PIXEL_WIDTH - 1);
        viewport.bottom = 0.0f;
        viewport.top = static_cast<Types::F32>(COMMON_PIXEL_HEIGHT - 1);
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

    /*!
        \brief pixel shader for vertex that have normal.
    */
    static std::function<RGBA(const ScreenSpaceVertexTemplate*)> GetPixelShaderWithNormal()
    {
        return [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
            const SimplePoint* pPoint = reinterpret_cast<const SimplePoint*>(pVertex);
            vector3 WarmDirection = Normalize(vector3(1.0f, 1.0f, 0.0f));
            
            vector3 normal = pPoint->m_rayIndex.ToVector3();
            Types::F32 kw = 0.5f * (1 + dotProd(normal, WarmDirection));

            RGB result = kw * RGB::BLUE + (1 - kw) * RGB::RED;
            return Cast(result);
        };
    }

    static typename std::function<void(const unsigned char *, ScreenSpaceVertexTemplate *)> GetVertexShaderWithNormal(Transform& trs, Transform& perspect, Transform& normalTrs)
    {
        return [&trs, &perspect, &normalTrs](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pDestV)->void {
            const SimplePoint* pSrcH = reinterpret_cast<const SimplePoint*>(pSrcVertex);
            SimplePoint* pDestH = reinterpret_cast<SimplePoint*>(pDestV);

            hvector inViewPos = trs * pSrcH->m_position;

            pDestH->m_position = perspect * inViewPos;
            //pDestH->m_position = pSrcH->m_position;
            hvector normal = pSrcH->m_rayIndex;
            normal.m_w = 0.0f;
            pDestH->m_rayIndex = normalTrs * normal;
        };
    }
        
};

/*!
    \brief base case class that have common function for and useful Environment for pipline testing.
    for the Suit class, you should use SuitForPipline.
*/
class CaseForPipline : public TestSuit::Case
{
protected:
    using Super = CaseForPipline;

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
        const float START_RADIUS = 0.3f;
        const Types::U32 NUM_ROUNDS = 12;
        const float RADIO_OFFSET = 0.0f; // Add this offset to every angle inside the sphereRay generating process.

                                         // create line segments in sphere ray.
        SphereRay([&numIndices, &outVertexData, &outIndices](HELP_SPHERE_RAY_LAMBDA_PARAMETERS)->void {

            const unsigned int theIndexOfOneLine = 64;
            // only get one line for convenience of debugging
            //if (lineIndex == theIndexOfOneLine)
            //{
            // add start vertex and its index
            SimplePoint start(hvector(x0, y0, 0.0f));
            start.m_rayIndex.m_x = static_cast<Types::F32>(roundIndex);
            start.m_rayIndex.m_y = static_cast<Types::F32>(lineIndex);
            start.m_rayIndex.m_z = 0.0f;
            outVertexData.push_back(start);
            outIndices.push_back(numIndices++);

            // add end vertex and its index
            SimplePoint end(hvector(x1, y1, 0.0f));
            end.m_rayIndex.m_x = static_cast<Types::F32>(roundIndex);
            end.m_rayIndex.m_y = static_cast<Types::F32>(lineIndex);
            end.m_rayIndex.m_z = 1.0f;
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
    CommonClass::RGB GetRandomRGB()
    {
        return CommonClass::RGB(mtr.Random(), mtr.Random(), mtr.Random());
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
    void BlockShowImg(Image * pImg, const std::wstring title) const
    {
        assert(pImg != nullptr);
        ImageWindow imgWnd(pImg, title);
        imgWnd.BlockShow();
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

        auto poly = std::make_unique<CommonClass::Polygon>(
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
        last four points is on the bottom, ranged ALSO in clockwise
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
    std::array<std::unique_ptr<CommonClass::Polygon>, 6> CreatBox(const std::array<vector3, 8>& points)
    {
        std::array<std::unique_ptr<CommonClass::Polygon>, 6> boxPolys;

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
        \brief get the path for output files.
    */
    virtual std::wstring GetStoragePath() const
    {
        return OUTPUT_PATH;
    }

    /*!
        \brief get safe storage path, if the path not exist, create it right now.
    */
    virtual std::wstring GetSafeStoragePath() const
    {
        auto path = GetStoragePath();
        if ( ! std::experimental::filesystem::exists(path))
        {
            std::experimental::filesystem::create_directory(path);
        }
        return path;
    }

    /*!
        \brief save the image to the sub folder with specific name, and show the image in a block thread with the title as same as the image.
        \param pipline the pipline to show
        \param nameOfImgAndWnd image name without .png and the window title at the same time
    */
    void SaveAndShowPiplineBackbuffer(const Pipline & pipline, const std::wstring& nameOfImgAndWindow) const
    {
        pipline.m_backBuffer->SaveTo(GetSafeStoragePath() + nameOfImgAndWindow + L".png");
        BlockShowImg(pipline.m_backBuffer.get(), nameOfImgAndWindow);
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

