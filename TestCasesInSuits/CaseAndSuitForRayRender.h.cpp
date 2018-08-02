#include "CaseAndSuitForRayRender.h"

void CASE_NAME_IN_RAY_RENDER(InsideBoxesAndSphere)::Run()
{
    const bool SKIP_THIS_TEST = false;

    if (SKIP_THIS_TEST)
    {
        std::cout << "test skipped";
        CountOneError();
        return;
    }

    /*!
    `    \brief set scene and light.
    */
    Scene scene;

    /*!
        \brief make up materials.
    */
    Material sphereMat      (RGB(0.5f, 0.2f, 1.0f),              8,  16.0f);
    Material triMat         (RGB::YELLOW,           8,  1.0f);
    Material polyMat        (RGB(0.3f, 0.5f, 0.9f), 8,  1.0f);
    Material polyMat_left   (RGB::RED,              8,  1.0f);
    Material polyMat_right  (RGB::GREEN,            8,  1.0f);
    Material polyMat_top    (RGB::WHITE * 0.6f,            8,  3.0f);
    Material polyMat_bottom (RGB::WHITE * 0.6f,            8,  3.0f);
    Material polyMat_back   (RGB::WHITE * 0.6f,            8,  3.0f);
    Material box_1_Mat      (RGB::WHITE * 0.8f,            8,  2.0f);
    Material box_2_Mat      (RGB::WHITE * 0.8f,            8,  2.0f);


    vector3 pointLightPosition(0.0f, 5.0f, 0.0f);
    RGB pointLightColor = RGB::WHITE;
    auto pointLight1 = std::make_unique<Light>(pointLightPosition, pointLightColor * 0.5f);
    auto pointLight2 = std::make_unique<Light>(vector3(2.2f, 2.7f, 0.0f), pointLightColor * 0.2f);
    auto pointLight3 = std::make_unique<Light>(vector3(-2.2f, 2.7f, 0.0f), pointLightColor * 0.2f);

    scene.Add(std::move(pointLight1));
    scene.Add(std::move(pointLight2));
    scene.Add(std::move(pointLight3));

    /*!
        \brief set a sphere to render.
    */
    auto tsph = std::make_unique<Sphere>(vector3(-1.0617f, 0.8190f, 1.2368f), 0.8f);
    tsph->m_material = sphereMat;

#pragma region build the big open box

    Types::F32 outBoxLeft = -2.5f;
    Types::F32 outBoxRight = +2.5f;
    Types::F32 outBoxTop = 5.470f;
    Types::F32 outBoxBack = -2.813f;
    Types::F32 outBoxFront = 2.813f;
    Types::F32 outBoxBottom = 0.100f;

    std::array<vector3, 8> outBoxPoints = {
        vector3(outBoxLeft, outBoxTop, outBoxFront),
        vector3(outBoxLeft, outBoxTop, outBoxBack),
        vector3(outBoxRight, outBoxTop, outBoxBack),
        vector3(outBoxRight, outBoxTop, outBoxFront),
        vector3(outBoxLeft, outBoxBottom, outBoxFront),
        vector3(outBoxLeft, outBoxBottom, outBoxBack),
        vector3(outBoxRight, outBoxBottom, outBoxBack),
        vector3(outBoxRight, outBoxBottom, outBoxFront)
    };
        
    auto leftPoly = CreatQuadPoly(
        outBoxPoints[PointsOrder::TOP_LEFT_FRONT],
        outBoxPoints[PointsOrder::TOP_LEFT_BACK],
        outBoxPoints[PointsOrder::BOTTOM_LEFT_BACK],
        outBoxPoints[PointsOrder::BOTTOM_LEFT_FRONT]
    );
    auto rightPoly = CreatQuadPoly(
        outBoxPoints[PointsOrder::TOP_RIGHT_BACK],
        outBoxPoints[PointsOrder::TOP_RIGHT_FRONT],
        outBoxPoints[PointsOrder::BOTTOM_RIGHT_FRONT],
        outBoxPoints[PointsOrder::BOTTOM_RIGHT_BACK]
    );
    auto bottomPoly = CreatQuadPoly(
        outBoxPoints[PointsOrder::BOTTOM_LEFT_FRONT],
        outBoxPoints[PointsOrder::BOTTOM_LEFT_BACK],
        outBoxPoints[PointsOrder::BOTTOM_RIGHT_BACK],
        outBoxPoints[PointsOrder::BOTTOM_RIGHT_FRONT]
    );
    auto topPoly = CreatQuadPoly(
        outBoxPoints[PointsOrder::TOP_LEFT_FRONT],
        outBoxPoints[PointsOrder::TOP_RIGHT_FRONT],
        outBoxPoints[PointsOrder::TOP_RIGHT_BACK],
        outBoxPoints[PointsOrder::TOP_LEFT_BACK]
    );
    auto backPoly = CreatQuadPoly(
        outBoxPoints[PointsOrder::TOP_LEFT_BACK],
        outBoxPoints[PointsOrder::TOP_RIGHT_BACK],
        outBoxPoints[PointsOrder::BOTTOM_RIGHT_BACK],
        outBoxPoints[PointsOrder::BOTTOM_LEFT_BACK]
    );

    leftPoly    ->m_material = polyMat_left;
    rightPoly   ->m_material = polyMat_right;
    bottomPoly  ->m_material = polyMat_bottom;
    topPoly     ->m_material = polyMat_top;
    backPoly    ->m_material = polyMat_back;

    scene.Add(std::move(leftPoly));
    scene.Add(std::move(rightPoly));
    scene.Add(std::move(bottomPoly));
    scene.Add(std::move(topPoly));
    scene.Add(std::move(backPoly));

#pragma endregion

#pragma region build inner box 1
    std::array<vector3, 8> innerBox_1_Points = {
        vector3(-1.689f, +3.286f, -0.560f),
        vector3(-2.187f, +3.286f, -2.007f),
        vector3(-0.503f, +3.286f, -2.587f),
        vector3(-0.005f, +3.286f, -1.140f),
        vector3(-1.689f, +0.112f, -0.560f),
        vector3(-2.187f, +0.112f, -2.007f),
        vector3(-0.503f, +0.112f, -2.587f),
        vector3(-0.005f, +0.112f, -1.140f)};

    auto innerBox1 = CreatBox(innerBox_1_Points);

    for (auto & polyFace : innerBox1)
    {
        polyFace->m_material = box_1_Mat;
    }

    /*!
        \brief add each poly seperately for debuging.
    */
    // TOP
    scene.Add(std::move(innerBox1[0]));
        
    // BOTTOM
    scene.Add(std::move(innerBox1[1]));

    // LEFT
    scene.Add(std::move(innerBox1[2]));

    // RIGHT
    scene.Add(std::move(innerBox1[3]));

    // FRONT
    scene.Add(std::move(innerBox1[4]));

    // BACK
    scene.Add(std::move(innerBox1[5]));

#pragma endregion

#pragma region build inner box 2
    std::array<vector3, 8> innerBox_2_Points = {
        vector3(+0.612f, +2.001f, -0.716f),
        vector3(+1.077f, +2.001f, -1.671f),
        vector3(+2.184f, +2.001f, -1.131f),
        vector3(+1.718f, +2.001f, -0.177f),
        vector3(+0.612f, +0.112f, -0.716f),
        vector3(+1.077f, +0.112f, -1.671f),
        vector3(+2.184f, +0.112f, -1.131f),
        vector3(+1.718f, +0.112f, -0.177f)};

    auto innerBox2 = CreatBox(innerBox_2_Points);

    for (auto & polyFace : innerBox2)
    {
        polyFace->m_material = box_2_Mat;
    }

    /*!
        \brief add each poly seperately for debuging.
    */
    // TOP
    scene.Add(std::move(innerBox2[0]));
        
    // BOTTOM
    scene.Add(std::move(innerBox2[1]));

    // LEFT
    scene.Add(std::move(innerBox2[2]));

    // RIGHT
    scene.Add(std::move(innerBox2[3]));

    // FRONT
    scene.Add(std::move(innerBox2[4]));

    // BACK
    scene.Add(std::move(innerBox2[5]));

#pragma endregion

    scene.Add(std::move(tsph));

    /*!
        \brief config a camera.
    */
    //vector3 camPosition = vector3(0.0f, 2.618f, 9.564f);        // original camera position
    //vector3 camPosition = vector3(-0.2f, 5.5f, 3.0f);
    //vector3 camPosition = vector3( -3.8f, 1.0f, 1.8f);
    vector3 camPosition = vector3(0.0f, 3.6f, 8.0f);
    vector3 camTarget = vector3(0.0f, 2.145f, 0.0f);
    vector3 camLookUp = vector3(0.0f, 1.0f, 0.0f);
    Types::F32 focalLength = 1.0f;
    PerspectiveCamera camera(focalLength, camPosition, camTarget, camLookUp);

    camera.SetFilm(std::make_unique<Film>(
        pEnvironment->COMMON_PIXEL_WIDTH, pEnvironment->COMMON_PIXEL_HEIGHT,
        -0.5f, +0.5f,
        -0.5f, +0.5f));

    /*!
        \brief help to trace the rendering progress.
    */
    const unsigned int PIXEL_WIDTH = camera.m_film->GetWidth();
    const unsigned int PIXEL_HEIGHT = camera.m_film->GetHeight();
    const unsigned int NUM_ALL_PIXEL = PIXEL_WIDTH * PIXEL_HEIGHT;

    HitRecord hitRec, shadowHitRec;
    Ray viewRay;
    for (unsigned int i = 0; i < PIXEL_WIDTH; ++i)
    {
        for (unsigned int j = 0; j < PIXEL_HEIGHT; ++j)
        {
            // print the progress
            const unsigned int CURR_COUNT_PIXE = j + i * PIXEL_HEIGHT + 1;
            if (CURR_COUNT_PIXE % 2500 == 0)
            {
                ShowProgress(CURR_COUNT_PIXE * 1.0f / NUM_ALL_PIXEL);
            }

            //BREAK_POINT_IF(i == 90 && j == 511 - 298);

            viewRay = camera.GetRay(i, j);

            //BREAK_POINT_IF(i == 93 && j == 511 - 76);
            camera.IncomeLight(i, j, scene.RayColor(viewRay, 0.0f, 1000.0f));
        }
    }

    camera.m_film->SaveTo(OUTPUT_PATH + SUB_FOLDER + L"InsideBox29.png");

    ImageWindow imgWnd(camera.m_film.get(), L"InsideBox29.png");
    imgWnd.BlockShow();
}