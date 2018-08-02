#include "CaseAndSuitForRasterisation.h"

void CASE_NAME_IN_RASTERISATION(WuXiaolinLines)::Run()
{
    RasterizeImage defaultImg(
        pEnvironment->COMMON_PIXEL_WIDTH, pEnvironment->COMMON_PIXEL_HEIGHT, 
        RGBA::WHITE);

    const Types::F32 
        CENTER_X = defaultImg.GetWidth() / 2.0f,
        CENTER_Y = defaultImg.GetHeight() / 2.0f;

    {
        COUNT_DETAIL_TIME;

        SphereRay([&defaultImg](HELP_SPHERE_RAY_LAMBDA_PARAMETERS) -> void {
            defaultImg.DrawWuXiaolinLine(x0, y0, x1, y1);
        }, CENTER_X, CENTER_Y);
    }// end time counter

    defaultImg.SaveTo(OUTPUT_PATH + SUB_FOLDER + L"sphereRayWuXiaolinLines.png");

    BlockShowImg(&defaultImg, L"draw lines using WuXiaolin method");
}

void CASE_NAME_IN_RASTERISATION(BresenhamLine)::Run()
{

    RasterizeImage defaultImg(
        pEnvironment->COMMON_PIXEL_WIDTH, pEnvironment->COMMON_PIXEL_HEIGHT, 
        RGBA::WHITE);

    const Types::F32 
        CENTER_X = defaultImg.GetWidth() / 2.0f,
        CENTER_Y = defaultImg.GetHeight() / 2.0f;

    {
        COUNT_DETAIL_TIME;

        SphereRay([&defaultImg](HELP_SPHERE_RAY_LAMBDA_PARAMETERS) -> void {
            defaultImg.DrawBresenhamLine(

                static_cast<Types::U32>(x0),
                static_cast<Types::U32>(y0),
                static_cast<Types::U32>(x1),
                static_cast<Types::U32>(y1));
        }, CENTER_X, CENTER_Y);
    }// end time counter

    defaultImg.SaveTo(OUTPUT_PATH + SUB_FOLDER + L"sphereRayBresenhamLines.png");

    BlockShowImg(&defaultImg, L"Bresenham line method");
}

void CASE_NAME_IN_RASTERISATION(2DLineClip)::Run()
{
    RasterizeImage defaultImg(pEnvironment->COMMON_PIXEL_WIDTH, pEnvironment->COMMON_PIXEL_HEIGHT, RGBA::WHITE);

    const Types::F32 CENTER_X = 0.5,
        CENTER_Y = 0.5;

    {
        COUNT_DETAIL_TIME;

        defaultImg.SetScissor(ScissorRect(0, 300, 300, 511));

        SphereRay([&defaultImg](HELP_SPHERE_RAY_LAMBDA_PARAMETERS) -> void {
            defaultImg.DrawLine(x0, y0, x1, y1, RGB::GREEN);
        }, CENTER_X, CENTER_Y, 0.1f, 0.1f, 6);


        defaultImg.SetScissor(ScissorRect(300, 511, 0, 511));

        SphereRay([&defaultImg](HELP_SPHERE_RAY_LAMBDA_PARAMETERS) -> void {
            defaultImg.DrawLine(x0, y0, x1, y1, RGB::RED);
        }, CENTER_X, CENTER_Y, 0.3f, 0.2f, 6);


        defaultImg.SetScissor(ScissorRect(0, 300, 0, 300));

        SphereRay([&defaultImg](HELP_SPHERE_RAY_LAMBDA_PARAMETERS) -> void {
            defaultImg.DrawLine(x0, y0, x1, y1, RGB::BLUE);
        }, CENTER_X, CENTER_Y, 0.05f, 0.1f, 8);
    }// end time counter

    defaultImg.SaveTo(OUTPUT_PATH + SUB_FOLDER + L"2d_line_clipping.png");

    BlockShowImg(&defaultImg, L"2d line clipping");
}

void CASE_NAME_IN_RASTERISATION(FlagBottomTriangle)::Run()
{
    RasterizeImage defaultImg(pEnvironment->COMMON_PIXEL_WIDTH, pEnvironment->COMMON_PIXEL_HEIGHT, RGBA::WHITE);

    const Types::F32 CENTER_X = defaultImg.GetWidth() / 2.0f,
        CENTER_Y = defaultImg.GetHeight() / 2.0f;

    {
        COUNT_DETAIL_TIME;

        defaultImg.DrawTri_flatBottom(
            256.0f, 500.0f,
            100.0f, 300.0f,
            300.0f, 300.0f,
            RGB::BLUE);

        defaultImg.DrawTri_flatTop(
            256.0f, 100.0f,
            100.0f, 300.0f,
            300.0f, 300.0f,
            RGB::RED);

    }// end time counter

    defaultImg.SaveTo(OUTPUT_PATH + SUB_FOLDER + L"triangleFlatBottom.png");

    BlockShowImg(&defaultImg, L"triangle flat bottom");
}

void CASE_NAME_IN_RASTERISATION(GeneralTriangle)::Run()
{
    RasterizeImage defaultImg(pEnvironment->COMMON_PIXEL_WIDTH, pEnvironment->COMMON_PIXEL_HEIGHT, RGBA::WHITE);

    const Types::F32 CENTER_X = defaultImg.GetWidth() / 2.0f,
        CENTER_Y = defaultImg.GetHeight() / 2.0f;

    {
        COUNT_DETAIL_TIME;

        defaultImg.DrawTriangle(
            256.0f, 500.0f,
            100.0f, 300.0f,
            300.0f, 100.0f,
            RGB::BLUE);

    }// end time counter

    defaultImg.SaveTo(OUTPUT_PATH + SUB_FOLDER + L"generalTriangle.png");

    BlockShowImg(&defaultImg, L"general triangle");
}

void CASE_NAME_IN_RASTERISATION(LineInPipline)::Run()
{
    // skip this test due to the bug of clipping line function.
    //return 0;
    // temp struct for line drawing.
    struct SimplePoint
    {
    public:
        hvector m_position;
        SimplePoint(const hvector& pos)
            :m_position(pos)
        {
            // empty
        }
    };
    static_assert(sizeof(SimplePoint) == 16, "size of SimplePoint is wrong");

    // create and config pipline state object
    auto pso = std::make_unique<PiplineStateObject>();
    pso->m_primitiveType = PrimitiveType::LINE_LIST;
    pso->m_vertexLayout.vertexShaderInputSize = sizeof(hvector);
    pso->m_vertexLayout.pixelShaderInputSize  = sizeof(hvector);
        
    pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {

        RGBA color;
        color.m_chas.m_r = pVertex->m_posH.m_x * 1.0f / 512;
        color.m_chas.m_g = pVertex->m_posH.m_y * 1.0f / 512;

        return color;
    };

    pso->m_vertexShader = [](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pdestV)->void {
        memcpy(pdestV, pSrcVertex, sizeof(SimplePoint));
    };

    Viewport viewport;
    viewport.left = 0;
    viewport.right = pEnvironment->COMMON_PIXEL_WIDTH - 1;
    viewport.bottom = 0;
    viewport.top = pEnvironment->COMMON_PIXEL_HEIGHT - 1;
    pso->SetViewport(viewport);

    // create and set a pipline.
    Pipline pipline;
    pipline.SetPSO(std::move(pso));

    // set a backbuffer
    pipline.SetBackBuffer(std::make_unique<RasterizeImage>(
        pEnvironment->COMMON_PIXEL_WIDTH,
        pEnvironment->COMMON_PIXEL_HEIGHT,
        RGBA::WHITE));

    std::vector<SimplePoint> points;
    std::vector<unsigned int> indices;
    unsigned int numIndices = 0;

    // create line segments in sphere ray.
    SphereRay([&numIndices, &points, &indices](HELP_SPHERE_RAY_LAMBDA_PARAMETERS)->void {

        // add start vertex and its index
        points.push_back(SimplePoint(hvector(x0, y0)));
        indices.push_back(numIndices++);

        // add end vertex and its index
        points.push_back(SimplePoint(hvector(x1, y1)));
        indices.push_back(numIndices++);
    }, 
        0.0f, 0.0f, // center location
        0.2f,       // segment length
        0.1f,       // start radius
        4);         // num rounds

    auto vertexBuffer = std::make_unique<F32Buffer>(points.size() * 4 * sizeof(Types::F32));
    memcpy(vertexBuffer->GetBuffer(), points.data(), vertexBuffer->GetSizeOfByte());

    {
        COUNT_DETAIL_TIME;
        pipline.DrawInstance(indices, vertexBuffer.get());
    }

    SaveAndShowPiplineBackbuffer(pipline, L"drawLines");
}

void CASE_NAME_IN_RASTERISATION(LineClipping)::Run()
{
    // skip this test due to the bug of clipping line function.
    //std::cout << "next test will be skipped." << std::endl;
    //return 0;
        

    // temp struct for line drawing.
    struct SimplePoint
    {
    public:
        hvector m_position;
        SimplePoint(const hvector& pos)
            :m_position(pos)
        {
            // empty
        }
    };
    // create and config pipline state object
    auto pso = std::make_unique<PiplineStateObject>();
    pso->m_primitiveType = PrimitiveType::LINE_LIST;
    pso->m_vertexLayout.vertexShaderInputSize = sizeof(hvector);
    pso->m_vertexLayout.pixelShaderInputSize  = sizeof(hvector);
        
    pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {

        RGBA color;
        color.m_chas.m_r = pVertex->m_posH.m_x * 1.0f / 512;
        color.m_chas.m_g = pVertex->m_posH.m_y * 1.0f / 512;

        return color;
    };

    pso->m_vertexShader = [](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pdestV)->void {
        memcpy(pdestV, pSrcVertex, sizeof(SimplePoint));
    };

    Viewport viewport;
    viewport.left = 0;
    viewport.right = pEnvironment->COMMON_PIXEL_WIDTH - 1;
    viewport.bottom = 0;
    viewport.top = pEnvironment->COMMON_PIXEL_HEIGHT - 1;
    pso->SetViewport(viewport);

    // create and set a pipline.
    Pipline pipline;
    pipline.SetPSO(std::move(pso));

    // set a backbuffer
    pipline.SetBackBuffer(std::make_unique<RasterizeImage>(
        pEnvironment->COMMON_PIXEL_WIDTH,
        pEnvironment->COMMON_PIXEL_HEIGHT,
        RGBA::WHITE));

    std::vector<SimplePoint> points;
    std::vector<unsigned int> indices;
    unsigned int numIndices = 0;

    // create line segments in sphere ray.
    SphereRay([&numIndices, &points, &indices](HELP_SPHERE_RAY_LAMBDA_PARAMETERS)->void {

        // add start vertex and its index
        points.push_back(SimplePoint(hvector(x0, y0)));
        indices.push_back(numIndices++);

        // add end vertex and its index
        points.push_back(SimplePoint(hvector(x1, y1)));
        indices.push_back(numIndices++);
    }, 
        0.8f, 0.0f, // center location
        0.3f,       // segment length
        0.1f,       // start radius
        12);         // num rounds

    auto vertexBuffer = std::make_unique<F32Buffer>(points.size() * 4 * sizeof(Types::F32));
    memcpy(vertexBuffer->GetBuffer(), points.data(), vertexBuffer->GetSizeOfByte());

    {
        COUNT_DETAIL_TIME;
        pipline.DrawInstance(indices, vertexBuffer.get());
    }

    SaveAndShowPiplineBackbuffer(pipline, L"lineClippingTest");
}

void CASE_NAME_IN_RASTERISATION(VertexTransform)::Run()
{
    // skip this test due to the bug of clipping line function.
    //std::cout << "next test will be skipped." << std::endl;
    //return 0;

    // temp struct for line drawing.
    struct SimplePoint
    {
    public:
        hvector m_position;
        SimplePoint(const hvector& pos)
            :m_position(pos)
        {
            // empty
        }
    };
    // create and config pipline state object
    auto pso = std::make_unique<PiplineStateObject>();
    pso->m_primitiveType = PrimitiveType::LINE_LIST;
    pso->m_vertexLayout.vertexShaderInputSize = sizeof(hvector);
    pso->m_vertexLayout.pixelShaderInputSize  = sizeof(hvector);
        
    pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {

        RGBA color(RGBA::WHITE);
        //color.SetChannel<RGBA::R>(pVertex->m_posH.m_x * 1.0f / 512);
        //color.SetChannel<RGBA::G>(pVertex->m_posH.m_y * 1.0f / 512);

        Types::F32 chanV = (pVertex->m_posH.m_z + 1.0f) / 2.0f;
        const Types::F32 gamma = 4.0f;
        chanV = std::powf(chanV, gamma);
        chanV = 1.0f - chanV;
        color.SetChannel<RGBA::B>(chanV);
        color.SetChannel<RGBA::R>(chanV);
        color.SetChannel<RGBA::G>(chanV);

        return color;
    };

    vector3 localU(1.0f, 0.0f, 1.0f);
    vector3 localV(0.0f, 1.0f, 0.0f);
    vector3 coordPos(0.0f, 0.0f, 0.0f);
    CoordinateFrame coordFrm(localU, localV, coordPos);
    Transform toWorld = coordFrm.GetTransformLocalToWorld();

    pso->m_vertexShader = [&toWorld](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pDestV)->void {
        const hvector* pSrcH = reinterpret_cast<const hvector*>(pSrcVertex);
        hvector* pDestH = reinterpret_cast<hvector*>(pDestV);
            
        *pDestH = toWorld * (*pSrcH);
    };

    Viewport viewport;
    viewport.left = 0;
    viewport.right = pEnvironment->COMMON_PIXEL_WIDTH - 1;
    viewport.bottom = 0;
    viewport.top = pEnvironment->COMMON_PIXEL_HEIGHT - 1;
    pso->SetViewport(viewport);

    // create and set a pipline.
    Pipline pipline;
    pipline.SetPSO(std::move(pso));

    // set a backbuffer
    pipline.SetBackBuffer(std::make_unique<RasterizeImage>(
        pEnvironment->COMMON_PIXEL_WIDTH,
        pEnvironment->COMMON_PIXEL_HEIGHT,
        RGBA::WHITE));

    std::vector<SimplePoint> points;
    std::vector<unsigned int> indices;
    unsigned int numIndices = 0;

    // create line segments in sphere ray.
    SphereRay([&numIndices, &points, &indices](HELP_SPHERE_RAY_LAMBDA_PARAMETERS)->void {

        // add start vertex and its index
        points.push_back(SimplePoint(hvector(x0, y0)));
        indices.push_back(numIndices++);

        // add end vertex and its index
        points.push_back(SimplePoint(hvector(x1, y1)));
        indices.push_back(numIndices++);
    }, 
        0.8f, 0.0f, // center location
        0.3f,       // segment length
        0.1f,       // start radius
        12);         // num rounds

    auto vertexBuffer = std::make_unique<F32Buffer>(points.size() * 4 * sizeof(Types::F32));
    memcpy(vertexBuffer->GetBuffer(), points.data(), vertexBuffer->GetSizeOfByte());

    {
        COUNT_DETAIL_TIME;
        pipline.DrawInstance(indices, vertexBuffer.get());
    }

    SaveAndShowPiplineBackbuffer(pipline, L"lineVertexShaderProcess06");
}

void CASE_NAME_IN_RASTERISATION(OrthoTransform)::Run()
{
    // skip this test due to the bug of clipping line function.
    //std::cout << "next test will be skipped." << std::endl;
    //return 0;

    // temp struct for line drawing.
    struct SimplePoint
    {
    public:
        hvector m_position;
        SimplePoint(const hvector& pos)
            :m_position(pos)
        {
            // empty
        }
    };
    // create and config pipline state object
    auto pso = std::make_unique<PiplineStateObject>();
    pso->m_primitiveType = PrimitiveType::LINE_LIST;
    pso->m_vertexLayout.vertexShaderInputSize = sizeof(hvector);
    pso->m_vertexLayout.pixelShaderInputSize  = sizeof(hvector);
        
    pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {

        return RGBA::BLACK;
    };

    // using MARCO BEFORE TO select two output of images,
    // BEFORE represent the regular sphere ray
    // else represent the distortion sphere ray which will be stretched.
//#define BEFORE

#ifdef BEFORE
    const Types::F32 LEFT(-1.0f), RIGHT(1.0f), BOTTOM(-1.0f), TOP(1.0f), NEAR(+1.0f), FAR(-1.0f);
#else
    const Types::F32 LEFT(-1.5f), RIGHT(-0.5f), BOTTOM(-1.0f), TOP(1.0f), NEAR(+1.0f), FAR(-1.0f);
#endif// BEFORE

    Transform orthTrans = Transform::OrthographicTransOG(LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR);

    pso->m_vertexShader = [& orthTrans](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pDestV)->void {
        const hvector* pSrcH = reinterpret_cast<const hvector*>(pSrcVertex);
        hvector* pDestH = reinterpret_cast<hvector*>(pDestV);
            
        *pDestH = orthTrans * (*pSrcH);
    };

    Viewport viewport;
    viewport.left = 0;
    viewport.right = pEnvironment->COMMON_PIXEL_WIDTH - 1;
    viewport.bottom = 0;
    viewport.top = pEnvironment->COMMON_PIXEL_HEIGHT - 1;
    pso->SetViewport(viewport);

    // create and set a pipline.
    Pipline pipline;
    pipline.SetPSO(std::move(pso));

    // set a backbuffer
    pipline.SetBackBuffer(std::make_unique<RasterizeImage>(
        pEnvironment->COMMON_PIXEL_WIDTH,
        pEnvironment->COMMON_PIXEL_HEIGHT,
        RGBA::WHITE));

    std::vector<SimplePoint> points;
    std::vector<unsigned int> indices;
    unsigned int numIndices = 0;

    // create line segments in sphere ray.
    SphereRay([&numIndices, &points, &indices](HELP_SPHERE_RAY_LAMBDA_PARAMETERS)->void {

        // add start vertex and its index
        points.push_back(SimplePoint(hvector(x0, y0)));
        indices.push_back(numIndices++);

        // add end vertex and its index
        points.push_back(SimplePoint(hvector(x1, y1)));
        indices.push_back(numIndices++);
    }, 
        0.0f, 0.0f, // center location
        0.3f,       // segment length
        0.1f,       // start radius
        3);         // num rounds

    auto vertexBuffer = std::make_unique<F32Buffer>(points.size() * 4 * sizeof(Types::F32));
    memcpy(vertexBuffer->GetBuffer(), points.data(), vertexBuffer->GetSizeOfByte());

    {
        COUNT_DETAIL_TIME;
        pipline.DrawInstance(indices, vertexBuffer.get());
    }

#ifdef BEFORE
    pipline.m_backBuffer->SaveTo(OUTPUT_PATH + SUB_FOLDER + L"orthographicViewTransformationBefore.png");
#else
    pipline.m_backBuffer->SaveTo(OUTPUT_PATH + SUB_FOLDER + L"orthographicViewTransformationAfter.png");
#endif

    SaveAndShowPiplineBackbuffer(pipline, L"orthographicViewTransformation");
}

void CASE_NAME_IN_RASTERISATION(PerspectTransform)::Run()
{
    // skip this test due to the bug of clipping line function.
    //std::cout << "next test will be skipped." << std::endl;
    //return 0;

    // temp struct for line drawing.
    struct SimplePoint
    {
    public:
        hvector m_position;
        SimplePoint(const hvector& pos)
            :m_position(pos)
        {
            // empty
        }
    };
    // create and config pipline state object
    auto pso = std::make_unique<PiplineStateObject>();
    pso->m_primitiveType = PrimitiveType::LINE_LIST;
    pso->m_vertexLayout.vertexShaderInputSize = sizeof(hvector);
    pso->m_vertexLayout.pixelShaderInputSize = sizeof(hvector);

    pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
        const Types::F32 depth = (pVertex->m_posH.m_z + 1.0f) * 0.5f;
        RGBA color(depth, depth, depth, 1.0f);
        return color;
    };

    const Types::F32 LEFT(-1.0f), RIGHT(1.0f), BOTTOM(-1.0f), TOP(1.0f), NEAR(-1.0f), FAR(-4.0f);

    // rotate the line a little.
    Transform rotateY = Transform::RotationY(Types::Constant::PI_F / 3.0f);
    Transform rotateZ = Transform::RotationZ(Types::Constant::PI_F / 3.0f);

    Transform moveIntoScree = Transform::Translation(0.0f, 0.0f, -2.0f);

    // perspective tranformation
    Transform perTrans = Transform::PerspectiveOG(LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR);

    Transform pushInto = Transform::Translation(0.0f, 0.0f, -2.0f);

    Transform mat = perTrans * pushInto * rotateZ * rotateY;

    pso->m_vertexShader = [&mat](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pDestV)->void {
        const hvector* pSrcH = reinterpret_cast<const hvector*>(pSrcVertex);
        hvector* pDestH = reinterpret_cast<hvector*>(pDestV);

        *pDestH = mat * (*pSrcH);
    };

    Viewport viewport;
    viewport.left = 0;
    viewport.right = pEnvironment->COMMON_PIXEL_WIDTH - 1;
    viewport.bottom = 0;
    viewport.top = pEnvironment->COMMON_PIXEL_HEIGHT - 1;
    pso->SetViewport(viewport);

    // create and set a pipline.
    Pipline pipline;
    pipline.SetPSO(std::move(pso));

    // set a backbuffer
    pipline.SetBackBuffer(std::make_unique<RasterizeImage>(
        pEnvironment->COMMON_PIXEL_WIDTH,
        pEnvironment->COMMON_PIXEL_HEIGHT,
        RGBA::WHITE));

    std::vector<SimplePoint> points;
    std::vector<unsigned int> indices;
    unsigned int numIndices = 0;

    // create line segments in sphere ray.
    SphereRay([&numIndices, &points, &indices](HELP_SPHERE_RAY_LAMBDA_PARAMETERS)->void {

        // add start vertex and its index
        points.push_back(SimplePoint(hvector(x0, y0, 0.0f)));
        indices.push_back(numIndices++);

        // add end vertex and its index
        points.push_back(SimplePoint(hvector(x1, y1, 0.0f)));
        indices.push_back(numIndices++);
    },
        0.0f, 0.0f, // center location
        0.3f,       // segment length
        0.1f,       // start radius
        12);         // num rounds

    auto vertexBuffer = std::make_unique<F32Buffer>(points.size() * 4 * sizeof(Types::F32));
    memcpy(vertexBuffer->GetBuffer(), points.data(), vertexBuffer->GetSizeOfByte());


    {
        COUNT_DETAIL_TIME;

        DebugGuard<DEBUG_CLIENT_CONF_LINE_CLIP_ERROR_ANALYSIS> guard;
        pipline.DrawInstance(indices, vertexBuffer.get());
    }

    SaveAndShowPiplineBackbuffer(pipline, L"perspectiveViewTransformation");
}

void CASE_NAME_IN_RASTERISATION(LineClippingErrorAnalysis)::Run()
{
    // temp struct for line drawing.
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

    static_assert(sizeof(SimplePoint) == 2 * sizeof(hvector), "SimplePoint size is wrong");
    // create and config pipline state object
    auto pso = std::make_unique<PiplineStateObject>();
    pso->m_primitiveType = PrimitiveType::LINE_LIST;
    pso->m_vertexLayout.vertexShaderInputSize = sizeof(SimplePoint);
    pso->m_vertexLayout.pixelShaderInputSize = sizeof(SimplePoint);

    pso->m_pixelShader = [](const ScreenSpaceVertexTemplate* pVertex)->RGBA {
        const Types::F32   depth = (pVertex->m_posH.m_z + 1.0f) * 0.5f;
        const SimplePoint* pPoint = reinterpret_cast<const SimplePoint*>(pVertex);
        const Types::F32   red = pPoint->m_rayIndex.m_z;

        const Types::U32   roundIndex = static_cast<const Types::U32>(pPoint->m_rayIndex.m_x);
        const Types::U32   lineIndex = static_cast<const Types::U32>(pPoint->m_rayIndex.m_y);
        //const Types::F32   isTheOne   = 22 < lineIndex && lineIndex < 34 ? 1.0f : 0.0f;
        const Types::F32   isTheOne = roundIndex == 7 && 34 <= lineIndex && lineIndex <= 34 ? 1.0f : 0.0f;

        RGBA               color(isTheOne, 0.0f, 0.0f, 1.0f);
        return color;
    };

    const Types::F32 LEFT(-1.0f), RIGHT(1.0f), BOTTOM(-1.0f), TOP(1.0f), NEAR(-0.5f), FAR(-4.0f);

    // rotate the line a little.
    Transform rotateY = Transform::RotationY(Types::Constant::PI_F / 3.0f);
    Transform rotateZ = Transform::RotationZ(Types::Constant::PI_F / 3.0f);

    Transform moveIntoScree = Transform::Translation(0.0f, 0.0f, -2.0f);

    // perspective tranformation
    Transform perTrans = Transform::PerspectiveOG(LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR);

    Transform pushInto = Transform::Translation(0.0f, 0.0f, -2.0f);

    Transform mat = perTrans * pushInto * rotateZ * rotateY;

    Transform toView = pushInto * rotateZ * rotateY;

    Transform perspect = perTrans;

    pso->m_vertexShader = [&mat, &toView, &perspect](const unsigned char * pSrcVertex, ScreenSpaceVertexTemplate * pDestV)->void {
        const SimplePoint* pSrcH = reinterpret_cast<const SimplePoint*>(pSrcVertex);
        SimplePoint* pDestH = reinterpret_cast<SimplePoint*>(pDestV);

        //DebugClient<DEBUG_CLIENT_CONF_LINE_CLIP_ERROR_ANALYSIS>(pSrcH->m_rayIndex.m_x == 7.0f && pSrcH->m_rayIndex.m_y == 34.0f);

        hvector inViewPos = toView * pSrcH->m_position;

        pDestH->m_position = perspect * inViewPos;
        pDestH->m_rayIndex = pSrcH->m_rayIndex;
    };

    Viewport viewport;
    viewport.left = 0;
    viewport.right = pEnvironment->COMMON_PIXEL_WIDTH - 1;
    viewport.bottom = 0;
    viewport.top = pEnvironment->COMMON_PIXEL_HEIGHT - 1;
    pso->SetViewport(viewport);

    // create and set a pipline.
    Pipline pipline;
    pipline.SetPSO(std::move(pso));

    // set a backbuffer
    pipline.SetBackBuffer(std::make_unique<RasterizeImage>(
        pEnvironment->COMMON_PIXEL_WIDTH,
        pEnvironment->COMMON_PIXEL_HEIGHT,
        RGBA::WHITE));

    std::vector<SimplePoint> points;
    std::vector<unsigned int> indices;
    unsigned int numIndices = 0;

    // create line segments in sphere ray.
    SphereRay([&numIndices, &points, &indices](HELP_SPHERE_RAY_LAMBDA_PARAMETERS)->void {

        const unsigned int theIndexOfOneLine = 64;
        // only get one line for convenience of debugging
        //if (lineIndex == theIndexOfOneLine)
        //{
        // add start vertex and its index
        SimplePoint start(hvector(x0, y0, 0.0f));
        start.m_rayIndex.m_x = roundIndex;
        start.m_rayIndex.m_y = lineIndex;
        start.m_rayIndex.m_z = 0;
        points.push_back(start);
        indices.push_back(numIndices++);

        // add end vertex and its index
        SimplePoint end(hvector(x1, y1, 0.0f));
        end.m_rayIndex.m_x = roundIndex;
        end.m_rayIndex.m_y = lineIndex;
        end.m_rayIndex.m_z = 1;
        points.push_back(end);
        indices.push_back(numIndices++);
        //}
    },
        0.0f, 0.0f,                     // center location
        0.3f,                           // segment length
        0.1f,                           // start radius
        12                              // num rounds
        ); // radio offset

    auto vertexBuffer = std::make_unique<F32Buffer>(points.size() * sizeof(SimplePoint));
    memcpy(vertexBuffer->GetBuffer(), points.data(), vertexBuffer->GetSizeOfByte());

    {
        COUNT_DETAIL_TIME;
        DebugGuard<DEBUG_CLIENT_CONF_LINE_CLIP_ERROR_ANALYSIS> guard;
        pipline.DrawInstance(indices, vertexBuffer.get());
    }

    std::wstring pictureIndex = L"final";

    SaveAndShowPiplineBackbuffer(pipline, L"lineClippingErrAnalysis_fixed_" + pictureIndex);
}
