#include "BaseToolForCaseAndSuit.h"
#include "BaseToolForCaseAndSuit.h"

using namespace CommonClass;

void CaseForPipline::BuildSphereRayLineData(std::vector<SimplePoint>& outVertexData, std::vector<unsigned int>& outIndices)
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
        SimplePoint start(vector4(x0, y0, 0.0f));
        start.m_rayIndex.m_x = static_cast<Types::F32>(roundIndex);
        start.m_rayIndex.m_y = static_cast<Types::F32>(lineIndex);
        start.m_rayIndex.m_z = 0.0f;
        outVertexData.push_back(start);
        outIndices.push_back(numIndices++);

        // add end vertex and its index
        SimplePoint end(vector4(x1, y1, 0.0f));
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

Types::F32 CaseForPipline::ClampChannel(Types::F32 ch)
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

CommonClass::RGB CaseForPipline::GetRandomRGB()
{
    return CommonClass::RGB(mtr.Random(), mtr.Random(), mtr.Random());
}

CommonClass::vector3 CaseForPipline::GetRandomVector3(bool allowZeroVector /*= true*/, const int MAX_INT /*= 200*/)
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

Types::F32 CaseForPipline::RandomFloat(const Types::F32 MAX_RADIUS /*= 200.0f*/)
{
    assert(MAX_RADIUS > 0.0f);
    return (mtr.Random() - 0.5f) * 2.f * MAX_RADIUS;
}

Types::F32 CaseForPipline::RandomFloatNotZero(const Types::F32 MAX_RADIUS /*= 200.0f*/)
{
    assert(MAX_RADIUS > 0.0f);
    float ret = 0.0f;
    do
    {
        ret = RandomFloat(MAX_RADIUS);
    } while (ret == 0.0f);
    return ret;
}

void CaseForPipline::BlockShowImg(const Image * pImg, const std::wstring title) const
{
    assert(pImg != nullptr);
    ImageWindow imgWnd(pImg, title);
    imgWnd.BlockShow();
}

std::unique_ptr<CommonClass::Polygon> CaseForPipline::CreatQuadPoly(const vector3 &p1, const vector3 &p2, const vector3 &p3, const vector3 &p4)
{
    auto poly = std::make_unique<CommonClass::Polygon>(
        p1,
        p2,
        p3
        );

    poly->AddPoint(p4);

    return poly;
}

std::array<std::unique_ptr<CommonClass::Polygon>, 6> CaseForPipline::CreatBox(const std::array<vector3, 8>& points)
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

std::wstring CaseForPipline::GetStoragePath() const
{
    return OUTPUT_PATH;
}

std::wstring CaseForPipline::GetSafeStoragePath() const
{
    auto path = GetStoragePath();
    if (!std::experimental::filesystem::exists(path))
    {
        std::experimental::filesystem::create_directories(path);
    }
    return path;
}

void CaseForPipline::SaveAndShowPiplineBackbuffer(const Pipline & pipline, const std::wstring& nameOfImgAndWindow) const
{
    SaveAndShow(*(pipline.m_backBuffer), nameOfImgAndWindow);
}

void CaseForPipline::SaveAndShow(const Image & img, const std::wstring & nameWithOutExt) const
{
    img.SaveTo(GetSafeStoragePath() + nameWithOutExt + L".png");
    BlockShowImg(&img, nameWithOutExt);
}
