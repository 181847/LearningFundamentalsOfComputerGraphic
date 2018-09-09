#include "CaseAndSuitForRasterizeTriangle.h"

void CASE_NAME_IN_RASTER_TRI(DrawTriInScreenSpace)::Run()
{
    auto pipline = GetCommonPipline();

    auto pso = pipline->GetPSO();

    pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
        return RGBA::BLACK;
    };

    // set viewport, because in the triangle rasterization, we need viewport to limit the triangle boundary.
    Viewport viewport;
    viewport.left = 0;
    viewport.right = COMMON_PIXEL_WIDTH - 1.0f;
    viewport.bottom = 0;
    viewport.top = COMMON_PIXEL_HEIGHT - 1.0f;
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
    auto pipline = GetCommonPipline();
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
            p0.m_rayIndex.m_x = roundIndex * 1.0f;
            p0.m_rayIndex.m_y = lineIndex * 1.0f;
            p0.m_rayIndex.m_z = 0.0f;
            points.push_back(p0);
            indices.push_back(numIndices++);

            SimplePoint p1(hvector(x1, y1, 0.0f));
            p1.m_rayIndex.m_x = roundIndex * 1.0f;
            p1.m_rayIndex.m_y = lineIndex * 1.0f;
            p1.m_rayIndex.m_z = 1.0f;
            points.push_back(p1);
            indices.push_back(numIndices++);

            SimplePoint p2(hvector(x2, y2, 0.0f));
            p2.m_rayIndex.m_x = roundIndex * 1.0f;
            p2.m_rayIndex.m_y = lineIndex * 1.0f;
            p2.m_rayIndex.m_z = 1.0f;
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
    auto pipline = GetCommonPipline();
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
    auto pipline = GetCommonPipline();
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
    auto pipline = GetCommonPipline();
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
    auto pipline = GetCommonPipline();
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

    std::wstring pictureIndex = L"008";
    SaveAndShowPiplineBackbuffer((*(pipline.get())), L"cube_" + pictureIndex);

    Image depthImg = ToImage(*(pipline->m_depthBuffer.get()), -1 / NEAR);
    BlockShowImg(&depthImg, L"the depth buffer of previous cube");
    depthImg.SaveTo(this->GetSafeStoragePath() + L"cube_" + pictureIndex + L"_depth.png");
}

void CASE_NAME_IN_RASTER_TRI(CylinderMesh)::Run()
{
    auto pipline = GetCommonPipline();
    auto pso = pipline->GetPSO();

    pso->m_cullFace = CullFace::COUNTER_CLOCK_WISE;

    pso->m_pixelShader = GetPixelShaderWithNormal();

    const Types::F32 LEFT(-1.0f), RIGHT(1.0f), BOTTOM(-1.0f), TOP(1.0f), NEAR(-1.0f), FAR(-10.0f);

    // rotate the line a little.
    Types::F32 pitch(3.14f * 3 / 4), yaw(3.14f / 4), roll(0 * 3.14f / 3);

    // perspective transformation
    Transform perspect = Transform::PerspectiveOG(LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR);

    Transform trs = Transform::TRS(vector3(-1.0f, 0.0f, -3.0f), vector3(pitch, yaw, roll), vector3(1.5f, 2.1f, 1.0f));
    Transform normalTrs = Transform::InverseTRS(vector3(-1.0f, 0.0f, -3.0f), vector3(pitch, yaw, roll), vector3(1.5f, 2.1f, 1.0f)).T();

    pso->m_vertexShader = GetVertexShaderWithNormal(trs, perspect, normalTrs);

    std::vector<SimplePoint> points;
    std::vector<unsigned int> indices;
    auto meshData = GeometryBuilder::BuildCylinder(0.6f, 0.8f, 0.8f, 50, 1, true);
    indices = meshData.m_indices;
    points.clear();

    for (const auto& vertex : meshData.m_vertices)
    {
        points.push_back(SimplePoint(vertex.m_pos.ToHvector(), vertex.m_normal.ToHvector(0.0f)));
    }

    auto vertexBuffer = std::make_unique<F32Buffer>(points.size() * sizeof(SimplePoint));
    memcpy(vertexBuffer->GetBuffer(), points.data(), vertexBuffer->GetSizeOfByte());

    pso->m_primitiveType = PrimitiveType::TRIANGLE_LIST;
    pso->m_cullFace = CullFace::CLOCK_WISE;
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(indices, vertexBuffer.get());
    }

    // change the trs matrix and draw same cube with different instance
    trs = Transform::TRS(vector3(0.8f, -0.6f, -2.0f), vector3(pitch, yaw + 3.14f / 3, roll), vector3(0.1f, 1.0f, 0.4f));
    normalTrs = Transform::InverseTRS(vector3(0.8f, -0.6f, -2.0f), vector3(pitch, yaw + 3.14f / 3, roll), vector3(0.1f, 1.0f, 0.4f)).T();
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(indices, vertexBuffer.get());
    }

    // change the trs matrix and draw same cube with different instance
    trs = Transform::TRS(vector3(0.4f, 0.6f, -3.0f), vector3(pitch, yaw + 3.14f / 2.f, roll + 3.14f / 8.f), vector3(1.2f, 2.0f, 1.4f));
    normalTrs = Transform::InverseTRS(vector3(0.4f, 0.6f, -3.0f), vector3(pitch, yaw + 3.14f / 2.f, roll + 3.14f / 8.f), vector3(1.2f, 2.0f, 1.4f)).T();
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(indices, vertexBuffer.get());
    }

    std::wstring pictureIndex = L"002";
    SaveAndShowPiplineBackbuffer((*(pipline.get())), L"cylinder_" + pictureIndex);

    Image depthImg = ToImage(*(pipline->m_depthBuffer.get()), -1 / NEAR);
    BlockShowImg(&depthImg, L"the depth buffer of previous cylinder");
    depthImg.SaveTo(this->GetSafeStoragePath() + L"cylinder_" + pictureIndex + L"_depth.png");
}

void CASE_NAME_IN_RASTER_TRI(SphereMesh)::Run()
{
    using SimplePoint = SimplePoint;
    static_assert(sizeof(SimplePoint) == 2 * sizeof(hvector), "SimplePoint size is wrong");

    auto pipline = GetCommonPipline();
    auto pso = pipline->GetPSO();

    pso->m_cullFace = CullFace::COUNTER_CLOCK_WISE;

    pso->m_pixelShader = GetPixelShaderWithNormal();

    const Types::F32 LEFT(-1.0f), RIGHT(1.0f), BOTTOM(-1.0f), TOP(1.0f), NEAR(-1.0f), FAR(-10.0f);

    // rotate the line a little.
    Types::F32 pitch(3.14f * 3.f / 4.f), yaw(3.14f / 4.f), roll(0.f * 3.14f / 3.f);

    // perspective transformation
    Transform perspect = Transform::PerspectiveOG(LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR);

    Transform trs = Transform::TRS(vector3(-1.0f, 0.0f, -3.0f), vector3(pitch, yaw, roll), vector3(1.5f, 2.1f, 1.0f));
    Transform normalTrs = Transform::InverseTRS(vector3(-1.0f, 0.0f, -3.0f), vector3(pitch, yaw, roll), vector3(1.5f, 2.1f, 1.0f)).T();

    pso->m_vertexShader = GetVertexShaderWithNormal(trs, perspect, normalTrs);

    std::vector<SimplePoint> points;
    std::vector<unsigned int> indices;
    auto meshData = GeometryBuilder::BuildSphere(0.8f, 16, 16);
    indices = meshData.m_indices;
    points.clear();

    for (const auto& vertex : meshData.m_vertices)
    {
        points.push_back(SimplePoint(vertex.m_pos.ToHvector(), vertex.m_normal.ToHvector(0.0f)));
    }

    auto vertexBuffer = std::make_unique<F32Buffer>(points.size() * sizeof(SimplePoint));
    memcpy(vertexBuffer->GetBuffer(), points.data(), vertexBuffer->GetSizeOfByte());

    pso->m_primitiveType = PrimitiveType::TRIANGLE_LIST;
    pso->m_cullFace = CullFace::CLOCK_WISE;
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(indices, vertexBuffer.get());
    }

    // change the trs matrix and draw same cube with different instance
    trs = Transform::TRS(vector3(-0.1f, 1.4f, -3.2f), vector3(pitch, yaw + 3.14f / 3, roll), vector3(1.5f, 1.5f, 1.5f));
    normalTrs = Transform::InverseTRS(vector3(-0.1f, 1.4f, -3.2f), vector3(pitch, yaw + 3.14f / 3, roll), vector3(1.5f, 1.5f, 1.5f)).T();
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(indices, vertexBuffer.get());
    }

    // change the trs matrix and draw same cube with different instance
    trs = Transform::TRS(vector3(1.4f, 0.6f, -3.0f), vector3(pitch, yaw + 3.14f / 2.f, roll + 3.14f / 8.f), vector3(0.5f, 0.5f, 0.5f));
    normalTrs = Transform::InverseTRS(vector3(1.4f, 0.6f, -3.0f), vector3(pitch, yaw + 3.14f / 2.f, roll + 3.14f / 8.f), vector3(0.5f, 0.5f, 0.5f)).T();
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(indices, vertexBuffer.get());
    }

    std::wstring pictureIndex = L"002";
    SaveAndShowPiplineBackbuffer((*(pipline.get())), L"sphere_" + pictureIndex);

    Image depthImg = ToImage(*(pipline->m_depthBuffer.get()), -1 / NEAR);
    BlockShowImg(&depthImg, L"the depth buffer of previous sphere");
    depthImg.SaveTo(this->GetSafeStoragePath() + L"sphere_" + pictureIndex + L"_depth.png");
}

void CASE_NAME_IN_RASTER_TRI(SphereMeshInWireframe)::Run()
{

    using SimplePoint = SimplePoint;
    static_assert(sizeof(SimplePoint) == 2 * sizeof(hvector), "SimplePoint size is wrong");

    auto pipline = GetCommonPipline();
    auto pso = pipline->GetPSO();

    pso->m_cullFace = CullFace::COUNTER_CLOCK_WISE;

    pso->m_pixelShader = GetPixelShaderWithNormal();

    const Types::F32 LEFT(-1.0f), RIGHT(1.0f), BOTTOM(-1.0f), TOP(1.0f), NEAR(-1.0f), FAR(-10.0f);

    // rotate the line a little.
    Types::F32 pitch(3.14f * 3.f / 4.f), yaw(3.14f / 4.f), roll(0.f * 3.14f / 3.f);

    // perspective transformation
    Transform perspect = Transform::PerspectiveOG(LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR);

    Transform trs = Transform::TRS(vector3(-1.0f, 0.0f, -3.0f), vector3(pitch, yaw, roll), vector3(1.5f, 2.1f, 1.0f));
    Transform normalTrs = Transform::InverseTRS(vector3(-1.0f, 0.0f, -3.0f), vector3(pitch, yaw, roll), vector3(1.5f, 2.1f, 1.0f)).T();

    pso->m_vertexShader = GetVertexShaderWithNormal(trs, perspect, normalTrs);

    std::vector<SimplePoint> points;
    std::vector<unsigned int> indices;
    auto meshData = GeometryBuilder::BuildSphere(0.8f, 16, 16);
    indices = meshData.m_indices;
    points.clear();

    for (const auto& vertex : meshData.m_vertices)
    {
        points.push_back(SimplePoint(vertex.m_pos.ToHvector(), vertex.m_normal.ToHvector(0.0f)));
    }

    auto vertexBuffer = std::make_unique<F32Buffer>(points.size() * sizeof(SimplePoint));
    memcpy(vertexBuffer->GetBuffer(), points.data(), vertexBuffer->GetSizeOfByte());

    pso->m_primitiveType = PrimitiveType::TRIANGLE_LIST;
    pso->m_fillMode = FillMode::WIREFRAME;
    pso->m_cullFace = CullFace::NONE;
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(indices, vertexBuffer.get());
    }

    pso->m_fillMode = FillMode::SOLIDE;
    // change the trs matrix and draw same cube with different instance
    trs = Transform::TRS(vector3(-0.1f, 1.4f, -3.2f), vector3(pitch, yaw + 3.14f / 3, roll), vector3(1.5f, 1.5f, 1.5f));
    normalTrs = Transform::InverseTRS(vector3(-0.1f, 1.4f, -3.2f), vector3(pitch, yaw + 3.14f / 3, roll), vector3(1.5f, 1.5f, 1.5f)).T();
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(indices, vertexBuffer.get());
    }

    pso->m_fillMode = FillMode::WIREFRAME;
    // change the trs matrix and draw same cube with different instance
    trs = Transform::TRS(vector3(0.4f, 0.6f, -3.0f), vector3(pitch, yaw + 3.14f / 2.f, roll + 3.14f / 8.f), vector3(0.5f, 0.5f, 0.5f));
    normalTrs = Transform::InverseTRS(vector3(0.4f, 0.6f, -3.0f), vector3(pitch, yaw + 3.14f / 2.f, roll + 3.14f / 8.f), vector3(0.5f, 0.5f, 0.5f)).T();
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(indices, vertexBuffer.get());
    }

    std::wstring pictureIndex = L"001";
    SaveAndShowPiplineBackbuffer((*(pipline.get())), L"sphere_wireframe_" + pictureIndex);

    Image depthImg = ToImage(*(pipline->m_depthBuffer.get()), -1 / NEAR);
    BlockShowImg(&depthImg, L"the depth buffer of previous sphere");
    depthImg.SaveTo(this->GetSafeStoragePath() + L"sphere_wireframe_" + pictureIndex + L"_depth.png");
}

void CASE_NAME_IN_RASTER_TRI(GeoSphereMesh)::Run()
{

    using SimplePoint = SimplePoint;
    static_assert(sizeof(SimplePoint) == 2 * sizeof(hvector), "SimplePoint size is wrong");

    auto pipline = GetCommonPipline();
    auto pso = pipline->GetPSO();

    pso->m_cullFace = CullFace::COUNTER_CLOCK_WISE;

    pso->m_pixelShader = GetPixelShaderWithNormal();

    const Types::F32 LEFT(-1.0f), RIGHT(1.0f), BOTTOM(-1.0f), TOP(1.0f), NEAR(-1.0f), FAR(-10.0f);

    // rotate the line a little.
    Types::F32 pitch(3.14f * 3.f / 4.f), yaw(3.14f / 4.f), roll(0.f * 3.14f / 3.f);

    // perspective transformation
    Transform perspect = Transform::PerspectiveOG(LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR);

    Transform trs = Transform::TRS(vector3(-1.0f, 0.0f, -3.0f), vector3(pitch, yaw, roll), vector3(1.5f, 2.1f, 1.0f));
    Transform normalTrs = Transform::InverseTRS(vector3(-1.0f, 0.0f, -3.0f), vector3(pitch, yaw, roll), vector3(1.5f, 2.1f, 1.0f)).T();

    pso->m_vertexShader = GetVertexShaderWithNormal(trs, perspect, normalTrs);

    std::vector<SimplePoint> points;
    std::vector<unsigned int> indices;
    auto meshData = GeometryBuilder::BuildGeoSphere(0.8f, 2);
    indices = meshData.m_indices;
    points.clear();

    for (const auto& vertex : meshData.m_vertices)
    {
        points.push_back(SimplePoint(vertex.m_pos.ToHvector(), vertex.m_normal.ToHvector(0.0f)));
    }

    auto vertexBuffer = std::make_unique<F32Buffer>(points.size() * sizeof(SimplePoint));
    memcpy(vertexBuffer->GetBuffer(), points.data(), vertexBuffer->GetSizeOfByte());

    pso->m_primitiveType = PrimitiveType::TRIANGLE_LIST;
    pso->m_cullFace = CullFace::CLOCK_WISE;
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(indices, vertexBuffer.get());
    }

    pso->m_fillMode = FillMode::WIREFRAME;
    // change the trs matrix and draw same cube with different instance
    trs = Transform::TRS(vector3(-0.1f, 1.4f, -3.2f), vector3(pitch, yaw + 3.14f / 3, roll), vector3(1.5f, 1.5f, 1.5f));
    normalTrs = Transform::InverseTRS(vector3(-0.1f, 1.4f, -3.2f), vector3(pitch, yaw + 3.14f / 3, roll), vector3(1.5f, 1.5f, 1.5f)).T();
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(indices, vertexBuffer.get());
    }

    pso->m_fillMode = FillMode::WIREFRAME;
    // change the trs matrix and draw same cube with different instance
    trs = Transform::TRS(vector3(0.4f, 0.6f, -4.0f), vector3(pitch, yaw + 3.14f / 2.f, roll + 3.14f / 8.f), vector3(0.8f, 0.8f, 0.8f));
    normalTrs = Transform::InverseTRS(vector3(0.4f, 0.6f, -4.0f), vector3(pitch, yaw + 3.14f / 2.f, roll + 3.14f / 8.f), vector3(0.8f, 0.8f, 0.8f)).T();
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(indices, vertexBuffer.get());
    }

    std::wstring pictureIndex = L"001";
    SaveAndShowPiplineBackbuffer((*(pipline.get())), L"geosphere_" + pictureIndex);

    Image depthImg = ToImage(*(pipline->m_depthBuffer.get()), -1 / NEAR);
    BlockShowImg(&depthImg, L"the depth buffer of previous geosphere");
    depthImg.SaveTo(this->GetSafeStoragePath() + L"geosphere_" + pictureIndex + L"_depth.png");
}

void CASE_NAME_IN_RASTER_TRI(UsingCameraFrame)::Run()
{
    static_assert(sizeof(SimplePoint) == 2 * sizeof(hvector), "SimplePoint size is wrong");

    // build two frame, to render the scene in different view.
    const unsigned int CAM1 = 0, CAM2 = 1;
    std::array<std::unique_ptr<CommonClass::Pipline>,           2> piplines = { GetCommonPipline() , GetCommonPipline() };
    std::array<std::shared_ptr<CommonClass::PiplineStateObject>,2> PSOs     = { piplines[CAM1]->GetPSO(),          piplines[CAM2]->GetPSO()};

    // perspective transformation
    const Types::F32 LEFT(-1.0f), RIGHT(1.0f), BOTTOM(-1.0f), TOP(1.0f), NEAR(-1.0f), FAR(-10.0f);
    Transform perspect = Transform::PerspectiveOG(LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR);

    Types::F32 pitch(3.14f * 3.f / 4.f), yaw(3.14f / 4.f), roll(0.f * 3.14f / 3.f);
    std::array<ConstantBufferForInstance, 3> instances;
    //instances[0].m_toWorld          = Transform();
    //instances[0].m_toWorldInverse   = Transform();
    instances[0].m_toWorld          = Transform::TRS        (vector3(0.0f, -1.0f, 0.0f), vector3(0, 0, 0),                             vector3(1.5f, 2.1f, 1.0f));
    instances[0].m_toWorldInverse   = Transform::InverseTRS (vector3(0.0f, -1.0f, 0.0f), vector3(0, 0, 0),                             vector3(1.5f, 2.1f, 1.0f));
    instances[1].m_toWorld          = Transform::TRS        (vector3(-0.1f, 1.4f, -3.2f), vector3(pitch, yaw + 3.14f / 3, roll),                 vector3(1.5f, 1.5f, 1.5f));
    instances[1].m_toWorldInverse   = Transform::InverseTRS (vector3(-0.1f, 1.4f, -3.2f), vector3(pitch, yaw + 3.14f / 3, roll),                 vector3(1.5f, 1.5f, 1.5f));
    instances[2].m_toWorld          = Transform::TRS        (vector3(0.4f, 0.6f, -4.0f),  vector3(pitch, yaw + 3.14f / 2.f, roll + 3.14f / 8.f), vector3(0.8f, 0.8f, 0.8f));
    instances[2].m_toWorldInverse   = Transform::InverseTRS (vector3(0.4f, 0.6f, -4.0f),  vector3(pitch, yaw + 3.14f / 2.f, roll + 3.14f / 8.f), vector3(0.8f, 0.8f, 0.8f));

    std::wstring pictureIndex = L"005";
    std::array<CameraFrame, 2> cameraFrames = {
        CameraFrame(vector3(1.0f, 0.0f, 1.0f) * 3.0f, vector3(0.0f, 0.0f, 0.0f)),
        CameraFrame(vector3(1.0f, 0.5f, -1.0f) * 4.0f, vector3(0.0f, 0.0f, 0.0f))};
    std::array<ConstantBufferForCamera, 2> cameraBuffer;
    cameraBuffer[CAM1].m_toCamera           = cameraFrames[CAM1].WorldToLocal();
    cameraBuffer[CAM1].m_toCameraInverse    = cameraFrames[CAM1].LocalToWorld();
    cameraBuffer[CAM1].m_project            = perspect;
    cameraBuffer[CAM2].m_toCamera           = cameraFrames[CAM2].WorldToLocal();
    cameraBuffer[CAM2].m_toCameraInverse    = cameraFrames[CAM2].LocalToWorld();
    cameraBuffer[CAM2].m_project            = perspect;

    ConstantBufferForInstance instanceBufAgent;// agent buffer for setting instance data
    // two pipline will share the same instanceData, but with different Camera.
    PSOs[CAM1]->m_vertexShader = GetVertexShaderWithNormalAndConstantBuffer(instanceBufAgent, cameraBuffer[CAM1]);
    PSOs[CAM2]->m_vertexShader = GetVertexShaderWithNormalAndConstantBuffer(instanceBufAgent, cameraBuffer[CAM2]);

    // set pixel shader, two pipline will share the same pixel shader.
    PSOs[CAM1]->m_pixelShader = GetPixelShaderWithNormal();
    PSOs[CAM2]->m_pixelShader = PSOs[CAM1]->m_pixelShader;

    // build mesh data
    std::vector<SimplePoint>  vertices;
    std::vector<unsigned int> indices;
    auto meshData = GeometryBuilder::BuildCylinder(0.6f, 0.8f, 0.8f, 50, 3, true);
    indices = meshData.m_indices;
    vertices.clear();
    for (const auto& vertex : meshData.m_vertices)
    {
        vertices.push_back(SimplePoint(vertex.m_pos.ToHvector(), vertex.m_normal.ToHvector(0.0f)));
    }
    auto vertexBuffer = std::make_unique<F32Buffer>(vertices.size() * sizeof(SimplePoint));
    memcpy(vertexBuffer->GetBuffer(), vertices.data(), vertexBuffer->GetSizeOfByte());
    
    for (auto& pso : PSOs)
    {
        pso->m_primitiveType = PrimitiveType::TRIANGLE_LIST;
        pso->m_cullFace      = CullFace::CLOCK_WISE;
        pso->m_fillMode      = FillMode::SOLIDE;
    }
    instanceBufAgent = instances[0];
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        piplines[CAM1]->DrawInstance(indices, vertexBuffer.get());
        piplines[CAM2]->DrawInstance(indices, vertexBuffer.get());
    }
    
    for (auto& pso : PSOs) { pso->m_fillMode = FillMode::SOLIDE; }
    instanceBufAgent = instances[1];
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        piplines[CAM1]->DrawInstance(indices, vertexBuffer.get());
        piplines[CAM2]->DrawInstance(indices, vertexBuffer.get());
    }

    for (auto& pso : PSOs) { pso->m_fillMode = FillMode::WIREFRAME; }
    instanceBufAgent = instances[2];
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        piplines[CAM1]->DrawInstance(indices, vertexBuffer.get());
        piplines[CAM2]->DrawInstance(indices, vertexBuffer.get());
    }

    for (unsigned int i = 0; i < piplines.size(); ++i)
    {
        std::wstring pictureNameWithNoExt = L"geosphere_" + pictureIndex + L"_cam" + std::to_wstring(i);
        SaveAndShowPiplineBackbuffer((*(piplines[i].get())), pictureNameWithNoExt);
        Image depthImg = ToImage(*(piplines[i]->m_depthBuffer.get()), -1 / NEAR);
        BlockShowImg(&depthImg, L"the depth buffer of previous geosphere");
        depthImg.SaveTo(this->GetSafeStoragePath() + pictureNameWithNoExt + L"_depth.png");
    }
}

void CASE_NAME_IN_RASTER_TRI(PixelShading)::Run()
{
    static_assert(sizeof(SimplePoint) == 2 * sizeof(hvector), "SimplePoint size is wrong");

    auto pipline = GetCommonPipline();
    std::shared_ptr<CommonClass::PiplineStateObject> PSO = pipline->GetPSO();
    PSO->m_vertexLayout.vertexShaderInputSize = sizeof(SimplePoint);
    PSO->m_vertexLayout.pixelShaderInputSize = sizeof(PSIn);

    // perspective transformation
    const Types::F32 LEFT(-1.0f), RIGHT(1.0f), BOTTOM(-1.0f), TOP(1.0f), NEAR(-1.0f), FAR(-10.0f);
    Transform perspect = Transform::PerspectiveOG(LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR);

    Types::F32 pitch(3.14f * 3.f / 4.f), yaw(3.14f / 4.f), roll(0.f * 3.14f / 3.f);
    std::array<ObjectInstance, 3> objInstances;
    // 1
    objInstances[0].m_position  = vector3(0.0f, 0.0f, 0.0f);
    objInstances[0].m_rotation  = vector3(0, 0, 0);
    objInstances[0].m_scale     = vector3(1.5f, 2.1f, 1.0f);
    // 2
    objInstances[1].m_position  = vector3(1.0f, 1.4f, 0.0f);
    objInstances[1].m_rotation  = vector3(pitch, yaw + 3.14f / 3, roll);
    objInstances[1].m_scale     = vector3(1.5f, 1.5f, 1.5f);
    // 3
    objInstances[2].m_position  = vector3(-1.0f, 2.8f, 0.0f);
    objInstances[2].m_rotation  = vector3(pitch, yaw + 3.14f / 2.f, roll + 3.14f / 8.f);
    objInstances[2].m_scale     = vector3(0.8f, 0.8f, 0.8f);

    std::array<ConstantBufferForInstance, 3> instanceBuffers;
    for (unsigned int i = 0; i < instanceBuffers.size(); ++i)
    {
        instanceBuffers[i].m_toWorld        = Transform::       TRS(objInstances[i].m_position, objInstances[i].m_rotation, objInstances[i].m_scale);
        instanceBuffers[i].m_toWorldInverse = Transform::InverseTRS(objInstances[i].m_position, objInstances[i].m_rotation, objInstances[i].m_scale);
        instanceBuffers[i].m_material.m_diffuse = RGB::RED * 0.5f;
        instanceBuffers[i].m_material.m_shiness = 1024.0f;
        instanceBuffers[i].m_material.m_fresnelR0 = MaterialBuffer::FresnelR0_byReflectionIndex(4);
    }// end for

    std::wstring pictureIndex = L"023";
    CameraFrame cameraFrames(vector3(0.0f, 0.0f, 1.0f) * 3.0f /* location */, vector3(0.0f, 0.0f, 0.0f) /* target */);
    ConstantBufferForCamera cameraBuffer;
    cameraBuffer.m_toCamera         = cameraFrames.WorldToLocal();
    cameraBuffer.m_toCameraInverse  = cameraFrames.LocalToWorld();
    cameraBuffer.m_camPos           = cameraFrames.m_origin;
    cameraBuffer.m_project          = perspect;
    cameraBuffer.m_numLights        = 1;
    cameraBuffer.m_ambientColor     = RGB::RED * RGB(0.05f, 0.05f, 0.05f);
    cameraBuffer.m_lights[0]        = {vector3(5.0f, -1.0f, 2.0f), RGB(1.0f, 1.0f, 0.5f)};

    ConstantBufferForInstance instanceBufAgent;// agent buffer for setting instance data
    // set VS and PS
    PSO->m_vertexShader = GetVertexShaderWithVSOut( instanceBufAgent, cameraBuffer);
    PSO->m_pixelShader  = GetPixelShaderWithPSIn(   instanceBufAgent, cameraBuffer);

    // build mesh data
    std::vector<SimplePoint>  vertices;
    std::vector<unsigned int> indices;
    //auto meshData = GeometryBuilder::BuildCylinder(0.6f, 0.8f, 0.8f, 32, 3, true);
    auto meshData = GeometryBuilder::BuildGeoSphere(0.8f, 2);
    indices = meshData.m_indices;
    vertices.clear();
    for (const auto& vertex : meshData.m_vertices)
    {
        vertices.push_back(SimplePoint(vertex.m_pos.ToHvector(), vertex.m_normal.ToHvector(0.0f)));
    }
    auto vertexBuffer = std::make_unique<F32Buffer>(vertices.size() * sizeof(decltype(vertices)::value_type));
    memcpy(vertexBuffer->GetBuffer(), vertices.data(), vertexBuffer->GetSizeOfByte());

    
    PSO->m_primitiveType = PrimitiveType::TRIANGLE_LIST;
    PSO->m_cullFace      = CullFace::CLOCK_WISE;
    PSO->m_fillMode      = FillMode::SOLIDE;
    instanceBufAgent     = instanceBuffers[0];
    {
        COUNT_DETAIL_TIME;
        DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(indices, vertexBuffer.get());
    }

    PSO->m_fillMode  = FillMode::SOLIDE;
    instanceBufAgent = instanceBuffers[1];
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(indices, vertexBuffer.get());
    }

    PSO->m_fillMode  = FillMode::WIREFRAME;
    instanceBufAgent = instanceBuffers[2];
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(indices, vertexBuffer.get());
    }

    std::wstring pictureNameWithNoExt = L"geosphere_simpleEquation_" + pictureIndex;
    SaveAndShowPiplineBackbuffer((*(pipline.get())), pictureNameWithNoExt);
    //Image depthImg = ToImage(*(pipline->m_depthBuffer.get()), -1 / NEAR);
    //BlockShowImg(&depthImg, L"the depth buffer of previous geosphere");
    //depthImg.SaveTo(this->GetSafeStoragePath() + pictureNameWithNoExt + L"_depth.png");
}