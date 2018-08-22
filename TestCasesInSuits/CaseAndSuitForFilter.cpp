#include "CaseAndSuitForFilter.h"
#include <windowsx.h>

void CASE_NAME_IN_FILTER(Basic)::Run()
{
    using Loc = Filter::Loc;

    Loc a = { 0, 0 };

    auto nextPos = a.Go<Loc::UP>();
    TEST_ASSERT(nextPos == Loc({  0, +1 }));
    nextPos = a.Go<Loc::DOWN>();
    TEST_ASSERT(nextPos == Loc({ 0, -1 }));
    nextPos = a.Go<Loc::LEFT>();
    TEST_ASSERT(nextPos == Loc({ -1,  0 }));
    nextPos = a.Go<Loc::RIGHT>();
    TEST_ASSERT(nextPos == Loc({ +1,  0 }));

    int step = 2;
    nextPos = a.Go<Loc::UP>(step);
    TEST_ASSERT(nextPos == Loc({ 0, +2 }));
    nextPos = a.Go<Loc::DOWN>(step);
    TEST_ASSERT(nextPos == Loc({ 0, -2 }));
    nextPos = a.Go<Loc::LEFT>(step);
    TEST_ASSERT(nextPos == Loc({ -2,  0 }));
    nextPos = a.Go<Loc::RIGHT>(step);
    TEST_ASSERT(nextPos == Loc({ +2,  0 }));
}

void CASE_NAME_IN_FILTER(GenerateLocMap)::Run()
{
    using Loc = Filter::Loc;
    
    Filter tf(1, 5, 5, 0);

    Loc bottomLeft = { 0, 0 };

    auto posList = tf.GenerateSampleLocations(bottomLeft);

    Loc boundary = { 5, 5 };

    for (auto const & pos : posList)
    {
        TEST_ASSERT(boundary.IsInclude(pos));
    }
}

void CASE_NAME_IN_FILTER(Exercise)::Run()
{
    using SimplePoint = CommonEnvironment::SimplePoint;
    static_assert(sizeof(SimplePoint) == 2 * sizeof(hvector), "SimplePoint size is wrong");

    auto pipline = pEnvironment->GetCommonPipline();
    auto pso = pipline->GetPSO();

    // the pixel shader will not work
    // due to the imcompletation of the triangle pipeline.
    pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
        const SimplePoint* pPoint = reinterpret_cast<const SimplePoint*>(pVertex);

        return RGBA(pPoint->m_rayIndex.m_x, pPoint->m_rayIndex.m_y, pPoint->m_rayIndex.m_z);
    };

    const Types::F32 LEFT(-1.0f), RIGHT(1.0f), BOTTOM(-1.0f), TOP(1.0f), NEAR(-1.0f), FAR(-10.0f);

    // rotate the line a little.
    Types::F32 pitch(3.14f * 3.f / 4.f), yaw(3.14f / 4.f), roll(0.f * 3.14f / 3.f);

    // perspective transformation
    Transform perspect = Transform::PerspectiveOG(LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR);

    Transform trs = Transform::TRS(vector3(-1.0f, 0.0f, -3.0f), vector3(pitch, yaw, roll), vector3(1.5f, 2.1f, 1.0f));

    Transform mat = perspect * trs;

    pso->m_vertexShader = [&mat, &trs, &perspect](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pDestV)->void {
        const SimplePoint* pSrcH = reinterpret_cast<const SimplePoint*>(pSrcVertex);
        SimplePoint* pDestH = reinterpret_cast<SimplePoint*>(pDestV);

        hvector inViewPos = trs * pSrcH->m_position;

        pDestH->m_position = perspect * inViewPos;
        //pDestH->m_position = pSrcH->m_position;
        pDestH->m_rayIndex = pSrcH->m_rayIndex;
    };

    std::vector<SimplePoint> points;
    std::vector<unsigned int> indices;
    unsigned int numIndices = 0;

    std::vector<vector3> positions;
    GeometryBuilder::BuildCube(1.0f, &positions, &indices);

    points.clear();
    int count = 0;
    for (const auto& pos : positions)
    {
        SimplePoint sp(hvector(pos.m_x, pos.m_y, pos.m_z));
        sp.m_rayIndex = hvector(
            static_cast<Types::F32>((count >> 2) % 2),
            static_cast<Types::F32>((count) % 2),
            static_cast<Types::F32>((count >> 1) % 2));
        points.push_back(sp);

        ++count;
    }

    auto vertexBuffer = std::make_unique<F32Buffer>(points.size() * sizeof(SimplePoint));
    memcpy(vertexBuffer->GetBuffer(), points.data(), vertexBuffer->GetSizeOfByte());

    pso->m_primitiveType = PrimitiveType::TRIANGLE_LIST;
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(indices, vertexBuffer.get());
    }

    // change the trs matrix and draw same cube with different instance
    trs = Transform::TRS(vector3(0.8f, -0.6f, -2.0f), vector3(pitch, yaw + 3.14f / 3, roll), vector3(0.1f, 1.0f, 0.4f));
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(indices, vertexBuffer.get());
    }

    // change the trs matrix and draw same cube with different instance
    trs = Transform::TRS(vector3(0.4f, 0.6f, -3.0f), vector3(pitch, yaw + 3.14f / 2.f, roll + 3.14f / 8.f), vector3(1.2f, 2.0f, 1.4f));
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(indices, vertexBuffer.get());
    }

    std::wstring pictureIndex = L"009";
    SaveAndShowPiplineBackbuffer((*(pipline.get())), L"cube_" + pictureIndex);

    Image depthImg = ToImage(*(pipline->m_depthBuffer.get()), -1 / NEAR);
    BlockShowImg(&depthImg, L"the depth buffer of previous cube");
    depthImg.SaveTo(this->GetStoragePath() + L"cube_" + pictureIndex + L"_depth.png");

    Filter filter2x2x2(2, 2, 2, 0);

    //DebugGuard<DEBUG_FILTER> guard;
    auto convResult1 = filter2x2x2.Convolve(*(pipline->m_backBuffer.get()));
    BlockShowImg(&convResult1, L"convolution result1");
    convResult1.SaveTo(this->GetStoragePath() + L"cube_" + pictureIndex + L"_convolution_2x2x2.png");


    Filter filter1x3x3(1, 3, 3, 0);
    auto convResult2 = filter1x3x3.Convolve(*(pipline->m_backBuffer.get()));
    BlockShowImg(&convResult2, L"convolution result2");
    convResult2.SaveTo(this->GetStoragePath() + L"cube_" + pictureIndex + L"_convolution_1x3x3.png");

    auto convResult3 = filter2x2x2.Convolve(convResult2);
    BlockShowImg(&convResult3, L"re convolve convolution result 2");
    convResult3.SaveTo(this->GetStoragePath() + L"cube_" + pictureIndex + L"_convolution2_2x2x2.png");
}