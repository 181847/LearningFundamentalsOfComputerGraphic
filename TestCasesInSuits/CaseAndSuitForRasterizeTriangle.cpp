#include "CaseAndSuitForRasterizeTriangle.h"

void CASE_NAME_IN_RASTER_TRI(DrawTriInScreenSpace)::Run()
{
    auto pipline = graphicToolSet.GetCommonPipline();

    auto pso = pipline->GetPSO();

    pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->vector4 {
        return vector4::BLACK;
    };

    // set viewport, because in the triangle rasterization, we need viewport to limit the triangle boundary.
    Viewport viewport;
    viewport.left = 0;
    viewport.right = graphicToolSet.COMMON_PIXEL_WIDTH - 1.0f;
    viewport.bottom = 0;
    viewport.top = graphicToolSet.COMMON_PIXEL_HEIGHT - 1.0f;
    pso->SetViewport(viewport);

    std::array<vector4, 3> triv = {
        vector4(2.0f, 400.0f),
        vector4(200.0f,  10.0f),
        vector4(400.0f, 300.0f)
    };

    std::array<ScreenSpaceVertexTemplate*, 3> vInScreen;
    for (size_t i = 0; i < vInScreen.size(); ++i)
    {
        vInScreen[i] = reinterpret_cast<ScreenSpaceVertexTemplate*>(&triv[i]);
    }

    pipline->DrawTriangle(vInScreen[0], vInScreen[2], vInScreen[1], sizeof(vector4));

    std::wstring pictureIndex = L"001";
    SaveAndShowPiplineBackbuffer((*(pipline.get())), L"screenSpaceTriangle_" + pictureIndex);
}

void CASE_NAME_IN_RASTER_TRI(SphereRayTriangle)::Run()
{
    auto pipline = graphicToolSet.GetCommonPipline();
    auto pso = pipline->GetPSO();
        
    // the pixel shader will not work
    // due to the imcompletation of the triangle pipeline.
    pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->vector4 {
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

        vector4            color     (red, 0.0f, 0.0f, 1.0f);
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

        vector4 inViewPos = toView * pSrcH->m_position;

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
            SimplePoint p0(vector4(x0, y0, 0.0f));
            p0.m_rayIndex.m_x = roundIndex * 1.0f;
            p0.m_rayIndex.m_y = lineIndex * 1.0f;
            p0.m_rayIndex.m_z = 0.0f;
            points.push_back(p0);
            indices.push_back(numIndices++);

            SimplePoint p1(vector4(x1, y1, 0.0f));
            p1.m_rayIndex.m_x = roundIndex * 1.0f;
            p1.m_rayIndex.m_y = lineIndex * 1.0f;
            p1.m_rayIndex.m_z = 1.0f;
            points.push_back(p1);
            indices.push_back(numIndices++);

            SimplePoint p2(vector4(x2, y2, 0.0f));
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
    auto pipline = graphicToolSet.GetCommonPipline();
    // create and config pipeline state object
    auto pso = pipline->GetPSO();

    pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->vector4 {
        return vector4::BLACK;
    };

    std::array<vector4, 3> triv = {
        vector4(  2.0f, 400.0f, 1.0f),
        vector4(200.0f,  10.0f, 2.0f),
        vector4(400.0f, 300.0f, -1.0f)
    };
    const size_t VERTEX_SIZE = sizeof(vector4);

    std::array<ScreenSpaceVertexTemplate*, 3> vInScreen;
    for (size_t i = 0; i < vInScreen.size(); ++i)
    {
        vInScreen[i] = reinterpret_cast<ScreenSpaceVertexTemplate*>(&triv[i]);
    }
    
    // define the plane equation to cut triangle
    class PositiveZPlane : public HPlaneEquation
    {
    public:
        Types::F32 eval(const vector4& pointH) override
        {
            return pointH.m_z;
        }

        Types::F32 cutCoefficient(const vector4& point1, const vector4& point2) override
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
    auto pipline = graphicToolSet.GetCommonPipline();
    auto pso = pipline->GetPSO();

    pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->vector4 {
        return vector4::BLACK;
    };

    std::array<vector4, 3> triv = {
        vector4(  2.0f, 400.0f, 1.0f),
        vector4(200.0f,  10.0f, 2.0f),
        vector4(400.0f, 300.0f, -1.0f)
    };
    const size_t VERTEX_SIZE = sizeof(vector4);

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

        Types::F32 eval(const vector4& pointH) override
        {
            return m_A * pointH.m_x + m_B * pointH.m_y + m_C;
        }

        Types::F32 cutCoefficient(const vector4& point1, const vector4& point2) override
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
    auto pipline = graphicToolSet.GetCommonPipline();
    auto pso = pipline->GetPSO();

    pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->vector4 {
        return vector4::BLACK;
    };

    std::array<vector4, 3> triv = {
        vector4(  2.0f, 400.0f,  2.0f, 1.0f),
        vector4(200.0f,  10.0f,  1.0f, 1.0f),
        vector4(400.0f, 300.0f, -3.0f, -1.0f)
    };
    const size_t VERTEX_SIZE = sizeof(vector4);

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
    auto pipline = graphicToolSet.GetCommonPipline();
    auto pso = pipline->GetPSO();

    CommonRenderingBuffer renderingBuffer;
    for (auto& instance : renderingBuffer.objInstances)
    {
        instance.m_position = instance.m_position + vector3(0.0f, 0.0f, -10.0f);
        //instance.m_rotation = instance.m_rotation + vector3(0.0f, 0.2f, 0.0f);
    }
    renderingBuffer.RebuildInstanceBuffer();
    Transform trsAgent, normalTrsAgent;
    pso->m_pixelShader  = graphicToolSet.GetPixelShaderWithNormal();
    pso->m_vertexShader = graphicToolSet.GetVertexShaderWithNormal(trsAgent, renderingBuffer.perspect, normalTrsAgent);

    const auto& mesh = renderingBuffer.prebuildMeshData[CommonRenderingBuffer::M_CUBE];

    pso->m_cullFace         = CullFace::COUNTER_CLOCK_WISE;
    pso->m_primitiveType    = PrimitiveType::TRIANGLE_LIST;
    pso->m_cullFace         = CullFace::CLOCK_WISE;
    pso->m_fillMode         = FillMode::SOLIDE;
    trsAgent                = renderingBuffer.instanceBuffers[0].m_toWorld;
    normalTrsAgent          = renderingBuffer.instanceBuffers[0].m_toWorldInverse;
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
    }

    pso->m_fillMode         = FillMode::SOLIDE;
    trsAgent                = renderingBuffer.instanceBuffers[1].m_toWorld;
    normalTrsAgent          = renderingBuffer.instanceBuffers[1].m_toWorldInverse;
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
    }

    pso->m_fillMode         = FillMode::WIREFRAME;
    trsAgent                = renderingBuffer.instanceBuffers[2].m_toWorld;
    normalTrsAgent          = renderingBuffer.instanceBuffers[2].m_toWorldInverse;
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
    }

    std::wstring pictureIndex = L"010";
    SaveAndShowPiplineBackbuffer((*(pipline.get())), L"cube_" + pictureIndex);

    Image depthImg = ToImage(*(pipline->m_depthBuffer.get()), 1 / renderingBuffer.NEAR_F);
    BlockShowImg(&depthImg, L"the depth buffer of previous cube");
    depthImg.SaveTo(this->GetSafeStoragePath() + L"cube_" + pictureIndex + L"_depth.png");
}

void CASE_NAME_IN_RASTER_TRI(CylinderMesh)::Run()
{
    auto pipline = graphicToolSet.GetCommonPipline();
    auto pso = pipline->GetPSO();

    CommonRenderingBuffer renderingBuffer;
    for (auto& instance : renderingBuffer.objInstances)
    {
        instance.m_position = instance.m_position + vector3(0.0f, -3.0f, -5.0f);
        instance.m_rotation = instance.m_rotation + vector3(0.0f, 0.2f, 0.0f);
    }
    renderingBuffer.RebuildInstanceBuffer();
    Transform trsAgent, normalTrsAgent;
    pso->m_pixelShader  = graphicToolSet.GetPixelShaderWithNormal();
    pso->m_vertexShader = graphicToolSet.GetVertexShaderWithNormal(trsAgent, renderingBuffer.perspect, normalTrsAgent);

    const auto& mesh = renderingBuffer.prebuildMeshData[CommonRenderingBuffer::M_CYLINDER];

    pso->m_cullFace         = CullFace::COUNTER_CLOCK_WISE;
    pso->m_primitiveType    = PrimitiveType::TRIANGLE_LIST;
    pso->m_cullFace         = CullFace::CLOCK_WISE;
    pso->m_fillMode         = FillMode::SOLIDE;
    trsAgent                = renderingBuffer.instanceBuffers[0].m_toWorld;
    normalTrsAgent          = renderingBuffer.instanceBuffers[0].m_toWorldInverse;
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
    }

    pso->m_fillMode         = FillMode::SOLIDE;
    trsAgent                = renderingBuffer.instanceBuffers[1].m_toWorld;
    normalTrsAgent          = renderingBuffer.instanceBuffers[1].m_toWorldInverse;
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
    }

    pso->m_fillMode         = FillMode::WIREFRAME;
    trsAgent                = renderingBuffer.instanceBuffers[2].m_toWorld;
    normalTrsAgent          = renderingBuffer.instanceBuffers[2].m_toWorldInverse;
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
    }

    std::wstring pictureIndex = L"002";
    SaveAndShowPiplineBackbuffer((*(pipline.get())), L"cylinder_" + pictureIndex);

    Image depthImg = ToImage(*(pipline->m_depthBuffer.get()), 1 / renderingBuffer.NEAR_F);
    BlockShowImg(&depthImg, L"the depth buffer of previous cylinder");
    depthImg.SaveTo(this->GetSafeStoragePath() + L"cylinder_" + pictureIndex + L"_depth.png");
}

void CASE_NAME_IN_RASTER_TRI(SphereMesh)::Run()
{
    auto pipline = graphicToolSet.GetCommonPipline();
    auto pso = pipline->GetPSO();

    CommonRenderingBuffer renderingBuffer;
    for (auto& instance : renderingBuffer.objInstances)
    {
        instance.m_position = instance.m_position + vector3(0.0f, -3.0f, -5.0f);
        instance.m_rotation = instance.m_rotation + vector3(0.0f, 0.2f, 0.0f);
    }
    renderingBuffer.RebuildInstanceBuffer();
    Transform trsAgent, normalTrsAgent;
    pso->m_pixelShader  = graphicToolSet.GetPixelShaderWithNormal();
    pso->m_vertexShader = graphicToolSet.GetVertexShaderWithNormal(trsAgent, renderingBuffer.perspect, normalTrsAgent);

    const auto& mesh = renderingBuffer.prebuildMeshData[CommonRenderingBuffer::M_SPHERE];
    
    pso->m_cullFace         = CullFace::COUNTER_CLOCK_WISE;
    pso->m_primitiveType    = PrimitiveType::TRIANGLE_LIST;
    pso->m_cullFace         = CullFace::CLOCK_WISE;
    pso->m_fillMode         = FillMode::SOLIDE;
    trsAgent                = renderingBuffer.instanceBuffers[0].m_toWorld;
    normalTrsAgent          = renderingBuffer.instanceBuffers[0].m_toWorldInverse;
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
    }

    pso->m_fillMode         = FillMode::SOLIDE;
    trsAgent                = renderingBuffer.instanceBuffers[1].m_toWorld;
    normalTrsAgent          = renderingBuffer.instanceBuffers[1].m_toWorldInverse;
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
    }

    pso->m_fillMode         = FillMode::WIREFRAME;
    trsAgent                = renderingBuffer.instanceBuffers[2].m_toWorld;
    normalTrsAgent          = renderingBuffer.instanceBuffers[2].m_toWorldInverse;
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
    }

    std::wstring pictureIndex = L"002";
    SaveAndShowPiplineBackbuffer((*(pipline.get())), L"sphere_" + pictureIndex);

    Image depthImg = ToImage(*(pipline->m_depthBuffer.get()), 1 / renderingBuffer.NEAR_F);
    BlockShowImg(&depthImg, L"the depth buffer of previous sphere");
    depthImg.SaveTo(this->GetSafeStoragePath() + L"sphere_" + pictureIndex + L"_depth.png");
}

void CASE_NAME_IN_RASTER_TRI(GeoSphereMesh)::Run()
{
    auto pipline = graphicToolSet.GetCommonPipline();
    auto pso = pipline->GetPSO();

    CommonRenderingBuffer renderingBuffer;
    for (auto& instance : renderingBuffer.objInstances)
    {
        instance.m_position = instance.m_position + vector3(0.0f, -3.0f, -5.0f);
        instance.m_rotation = instance.m_rotation + vector3(0.0f, 0.2f, 0.0f);
    }
    renderingBuffer.RebuildInstanceBuffer();
    Transform trsAgent, normalTrsAgent;
    pso->m_pixelShader  = graphicToolSet.GetPixelShaderWithNormal();
    pso->m_vertexShader = graphicToolSet.GetVertexShaderWithNormal(trsAgent, renderingBuffer.perspect, normalTrsAgent);

    const auto& mesh = renderingBuffer.prebuildMeshData[CommonRenderingBuffer::M_GEOSPHERE];
    
    pso->m_cullFace         = CullFace::COUNTER_CLOCK_WISE;
    pso->m_primitiveType    = PrimitiveType::TRIANGLE_LIST;
    pso->m_cullFace         = CullFace::CLOCK_WISE;
    pso->m_fillMode         = FillMode::SOLIDE;
    trsAgent                = renderingBuffer.instanceBuffers[0].m_toWorld;
    normalTrsAgent          = renderingBuffer.instanceBuffers[0].m_toWorldInverse;
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
    }

    pso->m_fillMode         = FillMode::SOLIDE;
    trsAgent                = renderingBuffer.instanceBuffers[1].m_toWorld;
    normalTrsAgent          = renderingBuffer.instanceBuffers[1].m_toWorldInverse;
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
    }

    pso->m_fillMode         = FillMode::WIREFRAME;
    trsAgent                = renderingBuffer.instanceBuffers[2].m_toWorld;
    normalTrsAgent          = renderingBuffer.instanceBuffers[2].m_toWorldInverse;
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
    }

    std::wstring pictureIndex = L"001";
    SaveAndShowPiplineBackbuffer((*(pipline.get())), L"geosphere_" + pictureIndex);

    Image depthImg = ToImage(*(pipline->m_depthBuffer.get()), 1 / renderingBuffer.NEAR_F);
    BlockShowImg(&depthImg, L"the depth buffer of previous geosphere");
    depthImg.SaveTo(this->GetSafeStoragePath() + L"geosphere_" + pictureIndex + L"_depth.png");
}

void CASE_NAME_IN_RASTER_TRI(UsingCameraFrame)::Run()
{
    // build two frame, to render the scene in different view.
    const unsigned int CAM1 = 0, CAM2 = 1;
    std::array<std::unique_ptr<CommonClass::Pipline>,           2> piplines = { graphicToolSet.GetCommonPipline() , graphicToolSet.GetCommonPipline() };
    std::array<std::shared_ptr<CommonClass::PiplineStateObject>,2> PSOs     = { piplines[CAM1]->GetPSO(),          piplines[CAM2]->GetPSO()};

    CommonRenderingBuffer renderingBuffer;
    
    std::wstring pictureIndex = L"005";
    std::array<CameraFrame, 2> cameraFrames = {
        CameraFrame(vector3(1.0f, 0.0f, 1.0f) * 3.0f, vector3(0.0f, 0.0f, 0.0f)),
        CameraFrame(vector3(1.0f, 0.5f, -1.0f) * 4.0f, vector3(0.0f, 0.0f, 0.0f))};
    std::array<GraphicToolSet::ConstantBufferForCamera, 2> cameraBuffer;
    cameraBuffer[CAM1].m_toCamera           = cameraFrames[CAM1].WorldToLocal();
    cameraBuffer[CAM1].m_toCameraInverse    = cameraFrames[CAM1].LocalToWorld();
    cameraBuffer[CAM1].m_project            = renderingBuffer.perspect;
    cameraBuffer[CAM2].m_toCamera           = cameraFrames[CAM2].WorldToLocal();
    cameraBuffer[CAM2].m_toCameraInverse    = cameraFrames[CAM2].LocalToWorld();
    cameraBuffer[CAM2].m_project            = renderingBuffer.perspect;

    GraphicToolSet::ConstantBufferForInstance instanceBufAgent;// agent buffer for setting instance data
    // two pipline will share the same instanceData, but with different Camera.
    PSOs[CAM1]->m_vertexShader = graphicToolSet.GetVertexShaderWithNormalAndConstantBuffer(instanceBufAgent, cameraBuffer[CAM1]);
    PSOs[CAM2]->m_vertexShader = graphicToolSet.GetVertexShaderWithNormalAndConstantBuffer(instanceBufAgent, cameraBuffer[CAM2]);

    // set pixel shader, two pipline will share the same pixel shader.
    PSOs[CAM1]->m_pixelShader = graphicToolSet.GetPixelShaderWithNormal();
    PSOs[CAM2]->m_pixelShader = PSOs[CAM1]->m_pixelShader;

    // build mesh data
    std::vector<SimplePoint>  vertices;
    std::vector<unsigned int> indices;
    auto meshData = GeometryBuilder::BuildCylinder(0.6f, 0.8f, 0.8f, 50, 3, true);
    indices = meshData.m_indices;
    vertices.clear();
    for (const auto& vertex : meshData.m_vertices)
    {
        vertices.push_back(SimplePoint(vertex.m_pos.Tovector4(), vertex.m_normal.Tovector4(0.0f)));
    }
    auto vertexBuffer = std::make_unique<F32Buffer>(vertices.size() * sizeof(SimplePoint));
    memcpy(vertexBuffer->GetBuffer(), vertices.data(), vertexBuffer->GetSizeOfByte());
    
    for (auto& pso : PSOs)
    {
        pso->m_primitiveType = PrimitiveType::TRIANGLE_LIST;
        pso->m_cullFace      = CullFace::CLOCK_WISE;
        pso->m_fillMode      = FillMode::SOLIDE;
    }
    instanceBufAgent = renderingBuffer.instanceBuffers[0];
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        piplines[CAM1]->DrawInstance(indices, vertexBuffer.get());
        piplines[CAM2]->DrawInstance(indices, vertexBuffer.get());
    }
    
    for (auto& pso : PSOs) { pso->m_fillMode = FillMode::SOLIDE; }
    instanceBufAgent = renderingBuffer.instanceBuffers[1];
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        piplines[CAM1]->DrawInstance(indices, vertexBuffer.get());
        piplines[CAM2]->DrawInstance(indices, vertexBuffer.get());
    }

    for (auto& pso : PSOs) { pso->m_fillMode = FillMode::WIREFRAME; }
    instanceBufAgent = renderingBuffer.instanceBuffers[2];
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
        Image depthImg = ToImage(*(piplines[i]->m_depthBuffer.get()), 1 / renderingBuffer.NEAR_F);
        BlockShowImg(&depthImg, L"the depth buffer of previous geosphere");
        depthImg.SaveTo(this->GetSafeStoragePath() + pictureNameWithNoExt + L"_depth.png");
    }
}

void CASE_NAME_IN_RASTER_TRI(PixelShading)::Run()
{
    auto pipline = graphicToolSet.GetCommonPipline();
    std::shared_ptr<CommonClass::PiplineStateObject> PSO = pipline->GetPSO();
    PSO->m_vertexLayout.vertexShaderInputSize = sizeof(SimplePoint);
    PSO->m_vertexLayout.pixelShaderInputSize = sizeof(GraphicToolSet::PSIn);

    CommonRenderingBuffer renderingBuffer;

    std::wstring pictureIndex = L"008";
    GraphicToolSet::ConstantBufferForInstance   instanceBufAgent;
    PSO->m_vertexShader = graphicToolSet.GetVertexShaderWithVSOut(instanceBufAgent, renderingBuffer.cameraBuffer);
    PSO->m_pixelShader = graphicToolSet.GetPixelShaderWithPSIn(instanceBufAgent, renderingBuffer.cameraBuffer);

    const auto& mesh = renderingBuffer.prebuildMeshData[CommonRenderingBuffer::M_GEOSPHERE];

    PSO->m_primitiveType = PrimitiveType::TRIANGLE_LIST;
    PSO->m_cullFace = CullFace::CLOCK_WISE;
    PSO->m_fillMode = FillMode::SOLIDE;
    instanceBufAgent = renderingBuffer.instanceBuffers[0];
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
    }

    PSO->m_fillMode = FillMode::SOLIDE;
    instanceBufAgent = renderingBuffer.instanceBuffers[1];
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
    }

    PSO->m_fillMode = FillMode::WIREFRAME;
    instanceBufAgent = renderingBuffer.instanceBuffers[2];
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
    }

    std::wstring pictureNameWithNoExt = L"geosphere_simpleEquation_" + pictureIndex;
    SaveAndShowPiplineBackbuffer((*(pipline.get())), pictureNameWithNoExt);
}

void CASE_NAME_IN_RASTER_TRI(TextureMapping)::Run()
{
    using namespace Types;

    auto pipline = graphicToolSet.GetCommonPipline();
    std::shared_ptr<CommonClass::PiplineStateObject> PSO = pipline->GetPSO();
    PSO->m_vertexLayout.vertexShaderInputSize = sizeof(SimplePoint);
    PSO->m_vertexLayout.pixelShaderInputSize = sizeof(GraphicToolSet::PSIn);

    CommonRenderingBuffer renderingBuffer;

    std::wstring pictureIndex = L"008";
    GraphicToolSet::ConstantBufferForInstance   instanceBufAgent;
    std::shared_ptr<Texture>    textureAgent;
    PSO->m_vertexShader = graphicToolSet.GetVertexShaderWithVSOut(instanceBufAgent, renderingBuffer.cameraBuffer);
    PSO->m_pixelShader = graphicToolSet.GetPixelShaderWithPSInAndTexture(instanceBufAgent, renderingBuffer.cameraBuffer, textureAgent);

    std::wstring geometryName;
    const auto& mesh = renderingBuffer.prebuildMeshData[CommonRenderingBuffer::M_GEOSPHERE];    geometryName = L"geoSphere";

    // load texture
    auto tex = std::make_shared<Texture>();
    tex->LoadFile("d:\\picture\\Maria\\sideShowToy\\bloodborne-lady-maria-of-the-astral-clocktower-statue-prime1-studio-902974-04.jpg");

    // set texture
    textureAgent = tex;

    PSO->m_primitiveType = PrimitiveType::TRIANGLE_LIST;
    PSO->m_cullFace = CullFace::CLOCK_WISE;
    PSO->m_fillMode = FillMode::SOLIDE;
    instanceBufAgent = renderingBuffer.instanceBuffers[0];
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
    }

    PSO->m_fillMode = FillMode::SOLIDE;
    instanceBufAgent = renderingBuffer.instanceBuffers[1];
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
    }

    PSO->m_fillMode = FillMode::WIREFRAME;
    instanceBufAgent = renderingBuffer.instanceBuffers[2];
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
    }

    std::wstring pictureNameWithNoExt = L"texture_mapping_" + pictureIndex + L"_" + geometryName;
    SaveAndShowPiplineBackbuffer((*(pipline.get())), pictureNameWithNoExt);
}

void CASE_NAME_IN_RASTER_TRI(NoiseBumpMap)::Run()
{
    using namespace Types;

    auto pipline = graphicToolSet.GetCommonPipline();
    pipline->ClearBackBuffer(vector4::WHITE * 0.5f);
    auto PSO = pipline->GetPSO();
    PSO->m_vertexLayout.vertexShaderInputSize = sizeof(SimplePoint);
    PSO->m_vertexLayout.pixelShaderInputSize = sizeof(GraphicToolSet::PSIn);

    CommonRenderingBuffer renderingBuffer;

    std::wstring pictureIndex = L"009";
    GraphicToolSet::ConstantBufferForInstance   instanceBufAgent;
    std::shared_ptr<Texture>    textureAgent;
    PSO->m_vertexShader = graphicToolSet.GetVertexShaderWithVSOut(instanceBufAgent, renderingBuffer.cameraBuffer);
    PSO->m_pixelShader = graphicToolSet.GetPixelShaderWithNoiseBumpMap(instanceBufAgent, renderingBuffer.cameraBuffer, textureAgent);

    std::wstring geometryName;
    const auto& mesh = renderingBuffer.prebuildMeshData[CommonRenderingBuffer::M_GEOSPHERE];    geometryName = L"geoSphere";

    // load texture
    auto tex = std::make_shared<Texture>();
    tex->LoadFile("d:\\GitHub\\C_C++\\LearningFundamentalsOfComputerGraphic\\build\\OutputTestImage\\StbImg\\noiseVector3_04.png");
    //tex->LoadFile("d:\\GitHub\\C_C++\\LearningFundamentalsOfComputerGraphic\\build\\OutputTestImage\\StbImg\\noiseVector3_03.png");
    textureAgent = tex;

    PSO->m_primitiveType = PrimitiveType::TRIANGLE_LIST;
    PSO->m_cullFace = CullFace::CLOCK_WISE;
    PSO->m_fillMode = FillMode::SOLIDE;
    instanceBufAgent = renderingBuffer.instanceBuffers[0];
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
    }

    PSO->m_fillMode = FillMode::SOLIDE;
    instanceBufAgent = renderingBuffer.instanceBuffers[1];
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
    }

    PSO->m_fillMode = FillMode::WIREFRAME;
    instanceBufAgent = renderingBuffer.instanceBuffers[2];
    {
        COUNT_DETAIL_TIME;
        //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
        pipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
    }

    std::wstring pictureNameWithNoExt = L"noise_normal_" + pictureIndex + L"_" + geometryName;
    SaveAndShowPiplineBackbuffer((*(pipline.get())), pictureNameWithNoExt);
}

void CASE_NAME_IN_RASTER_TRI(ShadowMap)::Run()
{
    using namespace Types;

    auto pipline = graphicToolSet.GetCommonPipline();
    pipline->ClearBackBuffer(vector4::WHITE * 0.5f);

    auto PSO_backbuffer = pipline->GetPSO();
    PSO_backbuffer->m_vertexLayout.vertexShaderInputSize = sizeof(SimplePoint);
    PSO_backbuffer->m_vertexLayout.pixelShaderInputSize = sizeof(GraphicToolSet::PSIn);

    auto PSO_shadowMap = graphicToolSet.GetCommonPSO();
    PSO_shadowMap->m_vertexLayout.vertexShaderInputSize = sizeof(SimplePoint);
    PSO_shadowMap->m_vertexLayout.pixelShaderInputSize = sizeof(GraphicToolSet::PSIn);

    // extend the spot light fading distance.
    CommonRenderingBuffer renderingBuffer;
    renderingBuffer.cameraBuffer.m_lights[1].m_fadeoffEnd = 10.0f;
    renderingBuffer.objInstances[2].m_scale = 2.0f * vector3::UNIT;
    renderingBuffer.cameraBuffer.m_lights[0].m_position = vector3(-5.0f, 5.0f, 5.0f);
    renderingBuffer.cameraBuffer.m_lights[0].m_fadeoffEnd = 10.0f;
    renderingBuffer.UpdateConstantBuffer();

    GraphicToolSet::ConstantBufferForInstance   instanceBufAgent;

    auto backbuffer = std::make_shared<Image>(graphicToolSet.COMMON_PIXEL_WIDTH, graphicToolSet.COMMON_PIXEL_HEIGHT);
    auto shadowMap  = std::make_shared<Texture>();
    shadowMap->Init(graphicToolSet.COMMON_PIXEL_WIDTH, graphicToolSet.COMMON_PIXEL_HEIGHT);

    std::wstring pictureIndex = L"015";
    PSO_backbuffer->m_vertexShader = graphicToolSet.GetVertexShaderWithVSOut(instanceBufAgent, renderingBuffer.cameraBuffer);
    PSO_backbuffer->m_pixelShader  = graphicToolSet.GetPixelShaderForShadowEffect(instanceBufAgent, renderingBuffer.cameraBuffer, renderingBuffer.lightCameraBuffer, shadowMap);
    PSO_shadowMap-> m_vertexShader = graphicToolSet.GetVertexShaderWithVSOut(instanceBufAgent, renderingBuffer.lightCameraBuffer);// notice, lightCameraBuffer is used.
    PSO_shadowMap-> m_pixelShader  = graphicToolSet.GetPixelShaderForShadowMap(instanceBufAgent, renderingBuffer.lightCameraBuffer);

    std::wstring geometryName;
    const auto& mesh = renderingBuffer.prebuildMeshData[CommonRenderingBuffer::M_GEOSPHERE];    geometryName = L"geoSphere";

    PSO_backbuffer->m_primitiveType = PrimitiveType::TRIANGLE_LIST;
    PSO_backbuffer->m_cullFace      = CullFace::CLOCK_WISE;
    PSO_backbuffer->m_fillMode      = FillMode::SOLIDE;
    PSO_shadowMap ->m_primitiveType = PrimitiveType::TRIANGLE_LIST;
    PSO_shadowMap ->m_cullFace      = CullFace::CLOCK_WISE;
    PSO_shadowMap ->m_fillMode      = FillMode::SOLIDE;

    // rendering depth map from the view of one spot light.
    pipline->SetPSO(PSO_shadowMap);
    pipline->SetBackBuffer(shadowMap);
    for (int i = 0; i < 3; ++i)
    {
        instanceBufAgent = renderingBuffer.instanceBuffers[i];
        {
            COUNT_DETAIL_TIME;
            //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
            pipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
        }
    }

    // rendering to back buffer.
    pipline->SetPSO(PSO_backbuffer);
    pipline->SetBackBuffer(backbuffer);
    for (int i = 0; i < 3; ++i)
    {
        instanceBufAgent = renderingBuffer.instanceBuffers[i];
        {
            COUNT_DETAIL_TIME;
            //DebugGuard<DEBUG_CLIENT_CONF_TRIANGL> openDebugMode;
            pipline->DrawInstance(mesh.indices, mesh.vertexBuffer.get());
        }
    }

    std::wstring pictureNameWithNoExt = L"shadowMap_backbuffer_" + pictureIndex + L"_" + geometryName;
    SaveAndShow(*backbuffer, pictureNameWithNoExt);

    std::wstring shadowMapNameWithNoExt = L"shadowMap_shadowMap_" + pictureIndex + L"_" + geometryName;
    SaveAndShow(*shadowMap, shadowMapNameWithNoExt);
}