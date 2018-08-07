#include "CaseAndSuitForRasterizeTriangle.h"

void CASE_NAME_IN_RASTER_TRI(DrawTriInScreenSpace)::Run()
{
    auto pipline = pEnvironment->GetCommonPipline();

    auto pso = pipline->GetPSO();

    pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
        return RGBA::BLACK;
    };

    // set viewport, because in the triangle rasterization, we need viewport to limit the triangle boundary.
    Viewport viewport;
    viewport.left = 0;
    viewport.right = pEnvironment->COMMON_PIXEL_WIDTH - 1;
    viewport.bottom = 0;
    viewport.top = pEnvironment->COMMON_PIXEL_HEIGHT - 1;
    pso->SetViewport(viewport);

    std::array<hvector, 3> triv = {
        hvector(2.0f, 400.0f),
        hvector(200.0f,  10.0f),
        hvector(400.0f, 300.0f)
    };

    std::array<ScreenSpaceVertexTemplate*, 3> vInScreen;
    for (size_t i = 0; i < vInScreen.size(); ++i)
    {
        vInScreen[i] = reinterpret_cast<ScreenSpaceVertexTemplate*>(&triv[i]);
    }

    pipline->DrawTriangle(vInScreen[0], vInScreen[2], vInScreen[1], sizeof(hvector));

    std::wstring pictureIndex = L"001";
    SaveAndShowPiplineBackbuffer((*(pipline.get())), L"screenSpaceTriangle_" + pictureIndex);
}

void CASE_NAME_IN_RASTER_TRI(SphereRayTriangle)::Run()
{
    using SimplePoint = CommonEnvironment::SimplePoint;
    static_assert(sizeof(SimplePoint) == 2 * sizeof(hvector), "SimplePoint size is wrong");

    auto pipline = pEnvironment->GetCommonPipline();
    auto pso = pipline->GetPSO();
        
    // the pixel shader will not work
    // due to the imcompletation of the triangle pipeline.
    pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
        const Types::F32   depth      = (pVertex->m_posH.m_z + 1.0f) * 0.5f;
        const SimplePoint* pPoint     = reinterpret_cast<const SimplePoint*>(pVertex);

        const Types::U32   roundIndex = static_cast<const Types::U32>(pPoint->m_rayIndex.m_x);
        const Types::F32   lineIndex  = pPoint->m_rayIndex.m_y;
        Types::F32   red;
        if (31.9 < lineIndex && lineIndex < 32.1)
        {
            red = 1.0f;
        }
        else
        {
            red = 0.0f;
        }

        RGBA               color     (red, 0.0f, 0.0f, 1.0f);
        return color;
    };

    const Types::F32 LEFT(-1.0f), RIGHT(1.0f), BOTTOM(-1.0f), TOP(1.0f), NEAR(-0.5f), FAR(-4.0f);

    // rotate the line a little.
    Transform rotateY = Transform::RotationY(Types::Constant::PI_F / 3.0f);
    Transform rotateZ = Transform::RotationZ(Types::Constant::PI_F / 3.0f);

    Transform moveIntoScree = Transform::Translation(0.0f, 0.0f, -2.0f);

    // perspective transformation
    Transform perTrans = Transform::PerspectiveOG(LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR);
        
    Transform pushInto = Transform::Translation(0.0f, 0.0f, -2.0f);

    Transform mat = perTrans * pushInto * rotateZ * rotateY;

    Transform toView = pushInto * rotateZ * rotateY;

    Transform perspect = perTrans;

    pso->m_vertexShader = [&mat, &toView, &perspect](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pDestV)->void {
        const SimplePoint* pSrcH = reinterpret_cast<const SimplePoint*>(pSrcVertex);
        SimplePoint* pDestH = reinterpret_cast<SimplePoint*>(pDestV);

        hvector inViewPos = toView * pSrcH->m_position;

        pDestH->m_position = perspect * inViewPos;
        //pDestH->m_position = pSrcH->m_position;
        pDestH->m_rayIndex = pSrcH->m_rayIndex;
    };

    std::vector<SimplePoint> points;
    std::vector<unsigned int> indices;
    unsigned int numIndices = 0;

    // create line segments in sphere ray.
    SphereRayTri([&numIndices, &points, &indices](HELP_SPHERE_RAY_TRI_LAMBDA_PARAMETERS)->void {

        //if (lineIndex == 32)
        {
            SimplePoint p0(hvector(x0, y0, 0.0f));
            p0.m_rayIndex.m_x = roundIndex;
            p0.m_rayIndex.m_y = lineIndex;
            p0.m_rayIndex.m_z = 0;
            points.push_back(p0);
            indices.push_back(numIndices++);

            SimplePoint p1(hvector(x1, y1, 0.0f));
            p1.m_rayIndex.m_x = roundIndex;
            p1.m_rayIndex.m_y = lineIndex;
            p1.m_rayIndex.m_z = 1;
            points.push_back(p1);
            indices.push_back(numIndices++);

            SimplePoint p2(hvector(x2, y2, 0.0f));
            p2.m_rayIndex.m_x = roundIndex;
            p2.m_rayIndex.m_y = lineIndex;
            p2.m_rayIndex.m_z = 1;
            points.push_back(p2);
            indices.push_back(numIndices++);
        } // end if lineIndex
    }, 
        0.0f, 0.0f,                     // center location
        0.8f,                           // segment length
        0.5f,                           // start radius
        12                               // num rounds
        ); // radio offset

    auto vertexBuffer = std::make_unique<F32Buffer>(points.size() * sizeof(SimplePoint));
    memcpy(vertexBuffer->GetBuffer(), points.data(), vertexBuffer->GetSizeOfByte());

    pso->m_primitiveType = PrimitiveType::TRIANGLE_LIST;
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(indices, vertexBuffer.get());
    }

    std::wstring pictureIndex = L"007";
    SaveAndShowPiplineBackbuffer((*(pipline.get())), L"sphereTri_" + pictureIndex);

    Image depthImg = ToImage(*(pipline->m_depthBuffer.get()), - 1 / NEAR);
    BlockShowImg(&depthImg, L"the depth buffer of previous window");
}

void CASE_NAME_IN_RASTER_TRI(TriangleCut)::Run()
{
    // create and set a pipline.
    auto pipline = pEnvironment->GetCommonPipline();
    // create and config pipeline state object
    auto pso = pipline->GetPSO();

    pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
        return RGBA::BLACK;
    };

    std::array<hvector, 3> triv = {
        hvector(  2.0f, 400.0f, 1.0f),
        hvector(200.0f,  10.0f, 2.0f),
        hvector(400.0f, 300.0f, -1.0f)
    };
    const size_t VERTEX_SIZE = sizeof(hvector);

    std::array<ScreenSpaceVertexTemplate*, 3> vInScreen;
    for (size_t i = 0; i < vInScreen.size(); ++i)
    {
        vInScreen[i] = reinterpret_cast<ScreenSpaceVertexTemplate*>(&triv[i]);
    }
    
    // define the plane equation to cut triangle
    class PositiveZPlane : public HPlaneEquation
    {
    public:
        Types::F32 eval(const hvector& pointH) override
        {
            return pointH.m_z;
        }

        Types::F32 cutCoefficient(const hvector& point1, const hvector& point2) override
        {
            return (0.0f - point1.m_z) / (point2.m_z - point1.m_z);
        }
    };

    PositiveZPlane posZPlane;
    TrianglePair cutResult(TrianglePair::ZERO, VERTEX_SIZE);
    {
        COUNT_DETAIL_TIME;
        TrianglePair cutResutlInTimer(posZPlane.CutTriangle(vInScreen[0], vInScreen[1], vInScreen[2], VERTEX_SIZE));
        cutResult = std::move(cutResutlInTimer);
    }

    switch (cutResult.m_count)
    {
    case 0:
        std::cout << "triangle reject" << std::endl;
        break;

    case 1:
    case 3:
        pipline->DrawTriangle(
            cutResult.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::ONE_TRI_1),
            cutResult.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::ONE_TRI_2),
            cutResult.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::ONE_TRI_3),
            VERTEX_SIZE);
        break;

    case 2:
        /*pipline->DDrawTriangle(
            cutResult.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_1_1),
            cutResult.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_1_2),
            cutResult.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_1_3),
            VERTEX_SIZE);*/
        pipline->DrawTriangle(
            cutResult.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_2_1),
            cutResult.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_2_2),
            cutResult.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_2_3),
            VERTEX_SIZE);
        break;
    }

    std::wstring pictureIndex = L"001";
    SaveAndShowPiplineBackbuffer((*(pipline.get())), L"triangleCut_" + pictureIndex);
}

void CASE_NAME_IN_RASTER_TRI(MultipleCutTriangle)::Run()
{
    auto pipline = pEnvironment->GetCommonPipline();
    auto pso = pipline->GetPSO();

    pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
        return RGBA::BLACK;
    };

    std::array<hvector, 3> triv = {
        hvector(  2.0f, 400.0f, 1.0f),
        hvector(200.0f,  10.0f, 2.0f),
        hvector(400.0f, 300.0f, -1.0f)
    };
    const size_t VERTEX_SIZE = sizeof(hvector);

    std::array<ScreenSpaceVertexTemplate*, 3> vInScreen;
    for (size_t i = 0; i < vInScreen.size(); ++i)
    {
        vInScreen[i] = reinterpret_cast<ScreenSpaceVertexTemplate*>(&triv[i]);
    }
    
    // the cut plane only cut xy plane,
    // in the other words, the plane is always perpendicular to xy plane
    class XYCutPlane : public HPlaneEquation
    {
    public:
        const float m_A, m_B, m_C;
    public:
        XYCutPlane(const float A, const float B, const float C)
            :m_A(A), m_B(B), m_C(C)
        {
            // empty
        }

        Types::F32 eval(const hvector& pointH) override
        {
            return m_A * pointH.m_x + m_B * pointH.m_y + m_C;
        }

        Types::F32 cutCoefficient(const hvector& point1, const hvector& point2) override
        {
            float x1(point1.m_x), y1(point1.m_y), x2(point2.m_x), y2(point2.m_y);
            float numerator = - (m_A * x1 + m_B * y1 + m_C);
            float denominator = m_A * (x2 - x1) + m_B * (y2 - y1);
            return numerator / denominator;
        }
    };

    std::vector<std::unique_ptr<HPlaneEquation>> cutPlanes;
    cutPlanes.push_back(std::make_unique<XYCutPlane>( 248.0f,  -57.0f,   -3893.0f));
    cutPlanes.push_back(std::make_unique<XYCutPlane>( 247.0f,  248.0f,  -67192.0f));
    cutPlanes.push_back(std::make_unique<XYCutPlane>(-185.0f,  293.0f,   14290.0f));
    cutPlanes.push_back(std::make_unique<XYCutPlane>(-282.0f, -353.0f,  151912.0f));

    std::vector<TrianglePair> cutResults;
    {
        COUNT_DETAIL_TIME;
        pipline->FrustumCutTriangle(vInScreen[0], vInScreen[1], vInScreen[2], VERTEX_SIZE,
            &cutResults, cutPlanes);
    }

    // lambda function to draw one TrianglePair.
    auto drawCutResult = [&pipline, &VERTEX_SIZE](TrianglePair& tp)->void {
        switch (tp.m_count)
        {
        case 0:
            std::cout << "triangle reject" << std::endl;
            break;// case 0

        case 1:
        case 3:
            pipline->DrawTriangle(
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::ONE_TRI_1),
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::ONE_TRI_2),
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::ONE_TRI_3),
                VERTEX_SIZE);
            break;// case 1 and 3

        case 2:
            pipline->DrawTriangle(
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_1_1),
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_1_2),
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_1_3),
                VERTEX_SIZE);
            pipline->DrawTriangle(
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_2_1),
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_2_2),
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_2_3),
                VERTEX_SIZE);
            break;// case 2
        }// end switch tp.m_count
    };// end lambda drawCutResult

    //drawCutResult(cutResults[5]);

    for (auto & result : cutResults)
    {
        drawCutResult(result);
    }

    std::wstring pictureIndex = L"005";
    SaveAndShowPiplineBackbuffer((*(pipline.get())), L"triangleMultipleCut_" + pictureIndex);
}

void CASE_NAME_IN_RASTER_TRI(AbstractFrustrumCut)::Run()
{
    auto pipline = pEnvironment->GetCommonPipline();
    auto pso = pipline->GetPSO();

    pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
        return RGBA::BLACK;
    };

    std::array<hvector, 3> triv = {
        hvector(  2.0f, 400.0f,  2.0f, 1.0f),
        hvector(200.0f,  10.0f,  1.0f, 1.0f),
        hvector(400.0f, 300.0f, -3.0f, -1.0f)
    };
    const size_t VERTEX_SIZE = sizeof(hvector);

    std::array<ScreenSpaceVertexTemplate*, 3> vInScreen;
    for (size_t i = 0; i < vInScreen.size(); ++i)
    {
        vInScreen[i] = reinterpret_cast<ScreenSpaceVertexTemplate*>(&triv[i]);
    }

    //using PlaneType = FrustumHPlaneEquation<2, false>;

    using PlaneType = WZeroHPlaneEquation;

    PlaneType cutPlane;
    
    TrianglePair cutResult(TrianglePair::ZERO, VERTEX_SIZE);

    std::vector<TrianglePair> cutResults;
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        cutResult = cutPlane.CutTriangle(vInScreen[0], vInScreen[1], vInScreen[2], VERTEX_SIZE);
    }

    // lambda function to draw one TrianglePair.
    auto drawCutResult = [&pipline, &VERTEX_SIZE](TrianglePair& tp)->void {
        switch (tp.m_count)
        {
        case 0:
            std::cout << "triangle reject" << std::endl;
            break;// case 0

        case 1:
        case 3:
            pipline->DrawTriangle(
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::ONE_TRI_1),
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::ONE_TRI_2),
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::ONE_TRI_3),
                VERTEX_SIZE);
            break;// case 1 and 3

        case 2:
            pipline->DrawTriangle(
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_1_1),
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_1_2),
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_1_3),
                VERTEX_SIZE);
            pipline->DrawTriangle(
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_2_1),
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_2_2),
                tp.GetVertexPointer<ScreenSpaceVertexTemplate>(TrianglePair::Index::TWO_TRI_2_3),
                VERTEX_SIZE);
            break;// case 2
        }// end switch tp.m_count
    };// end lambda drawCutResult

    //drawCutResult(cutResults[5]);

    drawCutResult(cutResult);

    std::wstring pictureIndex = L"001";
    SaveAndShowPiplineBackbuffer((*(pipline.get())), L"frustumPlaneCut_" + pictureIndex);
}

void CASE_NAME_IN_RASTER_TRI(CubeMesh)::Run()
{
    using SimplePoint = CommonEnvironment::SimplePoint;
    static_assert(sizeof(SimplePoint) == 2 * sizeof(hvector), "SimplePoint size is wrong");

    auto pipline = pEnvironment->GetCommonPipline();
    auto pso = pipline->GetPSO();

    pso->m_cullFace = CullFace::COUNTER_CLOCK_WISE;

    // the pixel shader will not work
    // due to the imcompletation of the triangle pipeline.
    pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
        const SimplePoint* pPoint = reinterpret_cast<const SimplePoint*>(pVertex);

        return RGBA(pPoint->m_rayIndex.m_x, pPoint->m_rayIndex.m_y, pPoint->m_rayIndex.m_z);
    };

    const Types::F32 LEFT(-1.0f), RIGHT(1.0f), BOTTOM(-1.0f), TOP(1.0f), NEAR(-1.0f), FAR(-10.0f);

    // rotate the line a little.
    Types::F32 pitch(3.14f * 3 / 4), yaw(3.14 / 4), roll(0 * 3.14 / 3);

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
        sp.m_rayIndex = hvector( (count >> 2) % 2, (count) % 2, (count >> 1) % 2);
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
    trs = Transform::TRS(vector3(0.4f, 0.6f, -3.0f), vector3(pitch, yaw + 3.14f / 2, roll + 3.14 / 8), vector3(1.2f, 2.0f, 1.4f));
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(indices, vertexBuffer.get());
    }

    std::wstring pictureIndex = L"008";
    SaveAndShowPiplineBackbuffer((*(pipline.get())), L"cube_" + pictureIndex);

    Image depthImg = ToImage(*(pipline->m_depthBuffer.get()), -1 / NEAR);
    BlockShowImg(&depthImg, L"the depth buffer of previous cube");
    depthImg.SaveTo(this->GetStoragePath() + L"cube_" + pictureIndex + L"_depth.png");
}