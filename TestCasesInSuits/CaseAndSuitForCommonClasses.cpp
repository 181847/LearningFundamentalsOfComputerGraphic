#include "CaseAndSuitForCommonClasses.h"

void CASE_NAME_IN_COMMON_CLASSES(BasicImage)::Run()
{
    const Types::U32 WIDTH(pEnvironment->COMMON_PIXEL_WIDTH), HEIGHT(pEnvironment->COMMON_PIXEL_HEIGHT);
    CommonClass::Image testImage(WIDTH, HEIGHT);

    RGBA pixelSetterT;

    pixelSetterT.SetChannel<RGBA::B>(0.5f);

    for (int x = 0; x < WIDTH; ++x)
    {
        for (int y = 0; y < HEIGHT; ++y)
        {
            pixelSetterT.SetChannel<RGBA::R>(x / 512.0f);
            pixelSetterT.SetChannel<RGBA::G>(y / 512.0f);
            pixelSetterT.SetChannel<RGBA::A>((x + y) / 1024.0f);

            testImage.SetPixel(x, y, pixelSetterT);
        }
    }

    testImage.SaveTo(OUTPUT_PATH + SUB_FOLDER + L"ThisImageIsForTest.png");

    ImageWindow imgWnd(&testImage, L"basic image");
    imgWnd.BlockShow();
}

void CASE_NAME_IN_COMMON_CLASSES(RGBAlphaSetting)::Run()
{
    const Types::U32 WIDTH(512), HEIGHT(512);
    CommonClass::Image testImage(WIDTH, HEIGHT);

    RGBA pixelSetterT;
    RGB pixelSetterRGB;
    Types::F32 alphaSetter = 1.0f;

    pixelSetterT.SetChannel<RGBA::B>(0.5f);
    pixelSetterRGB.SetChannel<RGB::B>(0.5f);

    for (int x = 0; x < WIDTH; ++x)
    {
        for (int y = 0; y < HEIGHT; ++y)
        {

            pixelSetterRGB.SetChannel<RGBA::R>(x / 512.0f);
            pixelSetterRGB.SetChannel<RGBA::G>(y / 512.0f);
            alphaSetter = ((x + y) / 1024.0f);

            testImage.SetPixel(x, y, pixelSetterRGB);
            testImage.SetAlpha(x, y, alphaSetter);
        }
    }

    testImage.SaveTo(OUTPUT_PATH + SUB_FOLDER + L"ThisImageIsForWriteRGB_alpha.png");

    BlockShowImg(&testImage, L"set color by RGB and Alpha");
}

void CASE_NAME_IN_COMMON_CLASSES(FilmCaptureLight)::Run()
{
    const Types::U32 WIDTH(512), HEIGHT(512);

    Film tfilm(WIDTH, HEIGHT, -1.0f, 1.0f, -1.0f, 1.0f);

    RGBA pixelSetterT;
    pixelSetterT.SetChannel<RGBA::B>(0.5f);

    for (int x = 0; x < WIDTH; ++x)
    {
        for (int y = 0; y < HEIGHT; ++y)
        {
            pixelSetterT.SetChannel<RGBA::R>(x / 512.0f);
            pixelSetterT.SetChannel<RGBA::G>(y / 512.0f);
            pixelSetterT.SetChannel<RGBA::A>((x + y) / 1024.0f);

            tfilm.SetPixel(x, y, pixelSetterT);
        }
    }

    tfilm.SaveTo(OUTPUT_PATH + SUB_FOLDER + L"ThisImageIsForFilm.png");

    BlockShowImg(&tfilm, L"film capture light");
}

void CASE_NAME_IN_COMMON_CLASSES(CameraBasis)::Run()
{
    /*!
        \brief help to check the vector's length is almost 1 unit.
    */
    auto lengthAlmostEqual_1_Unit = [](const vector3& a)->bool
    {
        return MathTool::almost_equal(Length(a), 1.0f, 8);
    };

    /*!
        \brief using cross product check two vectors is almost perpendicular to each other.
        Warning: the result of cross product by have a huge differ from 0.0f,
                so the almost_equal will not work properly,
                here we just specify a range (-1e-7f, 1e-7f).
    */
    auto almostPerpendicular = [](const vector3& a, const vector3& b)->bool
    {
        Types::F32 dpValue = dotProd(a, b);
        //std::printf("## check perpendicular vector3 %f.\n", dpValue);

        return (-1e-7f) < dpValue && dpValue < (1e-7f);
    };

    for (int i = 0; i < 20; ++i)
    {
        vector3 origin = GetRandomVector3(true);
        vector3 target = GetRandomVector3(false);
        vector3 dummyLookUp = Normalize(GetRandomVector3(false));

        // if the position and target is overlaped, skip this turn.
        if (origin == target)
        {
            continue;
        }

        OrthographicCamera orthoCamera(origin, target, dummyLookUp);
            
        // check basis is all unit vector
        TEST_ASSERT(lengthAlmostEqual_1_Unit(orthoCamera.m_w));
        TEST_ASSERT(lengthAlmostEqual_1_Unit(orthoCamera.m_u));
        TEST_ASSERT(lengthAlmostEqual_1_Unit(orthoCamera.m_v));

        // check three basises is perpendicular to each other
        TEST_ASSERT(almostPerpendicular(orthoCamera.m_w, orthoCamera.m_u));
        TEST_ASSERT(almostPerpendicular(orthoCamera.m_w, orthoCamera.m_v));
        TEST_ASSERT(almostPerpendicular(orthoCamera.m_u, orthoCamera.m_v));
    }
}

void CASE_NAME_IN_COMMON_CLASSES(CameraCaptureLight)::Run()
{
    using namespace MathTool;

    /*!
        \brief help to check the vector's length is almost 1 unit.
    */
    auto lengthAlmostEqual_1_Unit = [](const vector3& a)->bool
    {
        return almost_equal(Length(a), 1.0f, 8);
    };

    /*!
        \brief using cross product check two vectors is almost perpendicular to each other.
        Warning: the result of cross product by have a huge differ from 0.0f,
                so the almost_equal will not work properly,
                here we just specify a range (-1e-7f, 1e-7f).
    */
    auto almostPerpendicular = [](const vector3& a, const vector3& b)->bool
    {
        Types::F32 dpValue = dotProd(a, b);
        //std::printf("## check perpendicular vector3 %f.\n", dpValue);

        return (-1e-7f) < dpValue && dpValue < (1e-7f);
    };

    vector3 origin(0.0f, 0.0f, 0.0f);
    vector3 target(1.0f, 0.0f, 0.0f);
    vector3 dummyLookUp(0.0f, 1.0f, 0.0f);

    const Types::U32 WIDTH(512), HEIGHT(512);
    OrthographicCamera orthoCamera(origin, target, dummyLookUp);
    orthoCamera.SetFilm(std::make_unique<Film>(WIDTH, HEIGHT, -1.0f, 1.0f, -1.0f, 1.0f));

    RGBA pixelSetterT;
    pixelSetterT.SetChannel<RGBA::B>(0.5f);
    RGB pixelSetterRGB;
    pixelSetterRGB.SetChannel<RGBA::B>(0.5f);
    Types::F32 alphaSetter = 1.0f;

    for (int x = 0; x < WIDTH; ++x)
    {
        for (int y = 0; y < HEIGHT; ++y)
        {
            if (x < WIDTH / 2)
            {
                pixelSetterT.SetChannel<RGBA::R>(x / 512.0f);
                pixelSetterT.SetChannel<RGBA::G>(y / 512.0f);
                pixelSetterT.SetChannel<RGBA::A>((x + y) / 1024.0f);
                orthoCamera.IncomeLight(x, y, pixelSetterT);
            }
            else
            {
                pixelSetterRGB.SetChannel<RGBA::R>(x / 512.0f);
                pixelSetterRGB.SetChannel<RGBA::G>(y / 512.0f);
                alphaSetter = ((x + y) / 1024.0f);

                orthoCamera.IncomeLight(x, y, pixelSetterRGB);
                orthoCamera.IncomeAlpha(x, y, alphaSetter);
            }
        }
    }

    orthoCamera.m_film->SaveTo(OUTPUT_PATH + SUB_FOLDER + L"ThisImageIsForOrthoCamera.png");

    BlockShowImg(orthoCamera.m_film.get(), L"orthographic camera capture light");
}

void CASE_NAME_IN_COMMON_CLASSES(RayConstruct)::Run()
{
    for (int i = 0; i < 20; ++i)
    {
        Ray tray(GetRandomVector3(), GetRandomVector3(false));

        TEST_ASSERT(MathTool::almost_equal(Length(tray.m_direction), 1.0f, 8));
    }
}

void CASE_NAME_IN_COMMON_CLASSES(RayFromOrthoCamera)::Run()
{
    using namespace MathTool;

    vector3 camOrigin = GetRandomVector3();
    vector3 camLookAt = GetRandomVector3();
    vector3 camLookUp = Normalize(GetRandomVector3(false));
    OrthographicCamera orthoCamera(camOrigin, camLookAt, camLookUp);
    orthoCamera.SetFilm(std::make_unique<Film>(
        pEnvironment->COMMON_PIXEL_WIDTH, 
        pEnvironment->COMMON_PIXEL_HEIGHT,
        pEnvironment->COMMON_RENDER_LEFT, 
        pEnvironment->COMMON_RENDER_RIGHT, 
        pEnvironment->COMMON_RENDER_BOTTOM, 
        pEnvironment->COMMON_RENDER_TOP));

    for (unsigned int i = 0; i < orthoCamera.m_film->m_width; ++i)
    {

        for (unsigned int j = 0; j < orthoCamera.m_film->m_height; ++j)
        {
            Ray ray = orthoCamera.GetRay(i, j);

            TEST_ASSERT(AlmostPerpendicular(ray.m_origin - orthoCamera.m_origin, ray.m_direction, +1e-4f));
            TEST_ASSERT(AlmostEqual(ray.m_direction, -orthoCamera.m_w));
        }
    }

}

void CASE_NAME_IN_COMMON_CLASSES(CameraPixelLoc)::Run()
{
    vector3 origin(0.0f, 0.0f, 0.0f);
    vector3 target(1.0f, 0.0f, 0.0f);
    vector3 dummyLookUp(0.0f, 1.0f, 0.0f);

    const Types::U32 WIDTH(512), HEIGHT(512);
    OrthographicCamera orthoCamera(origin, target, dummyLookUp);
    orthoCamera.SetFilm(std::make_unique<Film>(WIDTH, HEIGHT, -1.0f, 1.0f, -1.0f, 1.0f));

    RGBA blackT(0.0f, 0.0f, 0.0f);
    RGBA whiteT(1.0f, 1.0f, 1.0f);
    RGBA redT(1.0f, 0.0f, 0.0f);
    RGBA greenT(0.0f, 1.0f, 0.0f);
    RGBA blueT(0.0f, 0.0f, 1.0f);


    // background as black
    for (int x = 0; x < WIDTH; ++x)
    {
        for (int y = 0; y < HEIGHT; ++y)
        {
            orthoCamera.IncomeLight(x, y, blackT);
        }
    }
    // counterclockwise from left top corner: 
    // white ------------------------> red 
    //  A                               |
    //  |                               |
    //  |           black               |
    //  |                               |
    //  |                               |
    //  |                               V
    //  blue <------------------------ green
    orthoCamera.IncomeLight(0, HEIGHT - 1, whiteT);
    orthoCamera.IncomeLight(WIDTH - 1, HEIGHT - 1, redT);
    orthoCamera.IncomeLight(WIDTH - 1, 0, greenT);
    orthoCamera.IncomeLight(0, 0, blueT);

    std::printf(
        "A Image with four single pixel in his corner:\n"
        "white ------------------------> red \n"
        " A                               | \n"
        " |                               | \n"
        " |           black               | \n"
        " |                               | \n"
        " |                               | \n"
        " |                               V \n"
        " blue <------------------------ green \n");

    orthoCamera.m_film->SaveTo(OUTPUT_PATH + SUB_FOLDER + L"ThisImageIsForOrthoCameraPixelLoc.png");

    BlockShowImg(orthoCamera.m_film.get(), L"pixel location");
}

void CASE_NAME_IN_COMMON_CLASSES(OrthoCameraAndSphere)::Run()
{
    Sphere tsph(vector3(-1.0f, 2.0f, 2.0f), 0.9f);

    RGB hitPixel(1.0f, 1.0f, 1.0f);
    RGB missSphPixel(0.0f, 0.0f, 0.0f);
    RGB missAABBPixel(0.0f, 0.5f, 0.0f);

    vector3 camPosition = vector3(2.0f, 1.0f, 3.0f);
    vector3 camTarget = vector3(0.0f, 0.0f, 0.0f);
    vector3 camLookUp = vector3(0.0f, 1.0f, 0.0f);

    OrthographicCamera orthoCamera(camPosition, camTarget, camLookUp);
    orthoCamera.SetFilm(std::make_unique<Film>(
        pEnvironment->COMMON_PIXEL_WIDTH,
        pEnvironment->COMMON_PIXEL_HEIGHT,
        pEnvironment->COMMON_RENDER_LEFT,
        pEnvironment->COMMON_RENDER_RIGHT,
        pEnvironment->COMMON_RENDER_BOTTOM,
        pEnvironment->COMMON_RENDER_TOP));

    HitRecord hitRec;

    for (unsigned int i = 0; i < orthoCamera.m_film->m_width; ++i)
    {
        for (unsigned int j = 0; j < orthoCamera.m_film->m_height; ++j)
        {
            Ray ray = orthoCamera.GetRay(i, j);

            //BREAK_POINT_IF(i == 400 && j == 256);

            // try bounding box first
            if (tsph.BoundingBox().Hit(ray, 0.0f, 1000.0f, &hitRec))
            {

                // try sphere
                if (tsph.Hit(ray, 0.0f, 1000.0f, &hitRec))
                {
                    hitPixel.SetChannel<RGB::R>((hitRec.m_hitT / 3.8f));
                    hitPixel.SetChannel<RGB::G>((hitRec.m_hitT / 3.8f));
                    hitPixel.SetChannel<RGB::B>((hitRec.m_hitT / 3.8f));

                    orthoCamera.IncomeLight(i, j, hitPixel);
                }
                else
                {
                    orthoCamera.IncomeLight(i, j, missSphPixel);
                }
            }
            else
            {
                orthoCamera.IncomeLight(i, j, missAABBPixel);
            }
        }
    }

    orthoCamera.m_film->SaveTo(OUTPUT_PATH + SUB_FOLDER + L"ThisImageIsForOrthoCameraRenderSphere.png");

    BlockShowImg(orthoCamera.m_film.get(), L"sphere in ortho camera");
}

void CASE_NAME_IN_COMMON_CLASSES(ConstColors)::Run()
{
    using namespace Types;

    const U32 WIDTH(512), HEIGHT(512);
    Image img(WIDTH, HEIGHT);

    std::array<RGBA, 8> colorsT = {
        RGBA::RED,
        RGBA::GREEN,
        RGBA::BLUE,
        RGBA::YELLOW,
        RGBA::CYAN,
        RGBA::MAGENTA,
        RGBA::WHITE,
        RGBA::BLACK,
    };

    std::array<RGB, 8> colorsRGB = {
        RGB::RED,
        RGB::GREEN,
        RGB::BLUE,
        RGB::YELLOW,
        RGB::CYAN,
        RGB::MAGENTA,
        RGB::WHITE,
        RGB::BLACK,
    };

    int i = 0;
    int colorIndex = 0;
    for (RGBA& oneColor : colorsT)
    {
        for (int count = 0; count < 64; ++count)
        {
            for (int row = 0; row < HEIGHT; ++row)
            {
                if (row < (HEIGHT / 2))
                {
                    img.SetPixel(i + count, row, oneColor);
                }
                else
                {
                    img.SetPixel(i + count, row, colorsRGB[colorIndex]);
                    img.SetAlpha(i + count, row, RGB::HALF_CHANNEL_VALUE);
                }
            }
        }
        i += 64;
        ++colorIndex;
    }

    img.SaveTo(OUTPUT_PATH + SUB_FOLDER + L"ConstantColorCheck.png");

    BlockShowImg(&img, L"constant colors");
}

void CASE_NAME_IN_COMMON_CLASSES(PerspectiveCameraAndSphere)::Run()
{
    Sphere tsph(vector3(0.0f, 0.0f, 0.0f), 1.0f);

    RGB hitPixel(1.0f, 1.0f, 1.0f);
    RGB missSphPixel(0.0f, 0.0f, 0.0f);
    RGB missAABBPixel(0.0f, 0.5f, 0.0f);

    vector3 camPosition = vector3(1.8f, 1.8f, 1.8f);
    vector3 camTarget = vector3(0.0f, 0.0f, 0.0f);
    vector3 camLookUp = vector3(0.0f, 1.0f, 0.0f);

    Types::F32 focalLength = 0.5f;

    PerspectiveCamera perspectCamera(focalLength, camPosition, camTarget, camLookUp);
    perspectCamera.SetFilm(std::make_unique<Film>(
        pEnvironment->COMMON_PIXEL_WIDTH, pEnvironment->COMMON_PIXEL_HEIGHT,
        -0.5f, +0.5f,
        -0.5f, +0.5f));

    HitRecord hitRec;

    for (unsigned int i = 0; i < perspectCamera.m_film->m_width; ++i)
    {
        for (unsigned int j = 0; j < perspectCamera.m_film->m_height; ++j)
        {
            Ray ray = perspectCamera.GetRay(i, j);

            //BREAK_POINT_IF(i == 256 && j == 256);

            // try bounding box first
            if (tsph.BoundingBox().Hit(ray, 0.0f, 1000.0f, &hitRec))
            {

                // try sphere
                if (tsph.Hit(ray, 0.0f, 1000.0f, &hitRec))
                {
                    hitPixel.SetChannel<RGB::R>((hitRec.m_hitT / 3.8f));
                    hitPixel.SetChannel<RGB::G>((hitRec.m_hitT / 3.8f));
                    hitPixel.SetChannel<RGB::B>((hitRec.m_hitT / 3.8f));

                    perspectCamera.IncomeLight(i, j, hitPixel);
                }
                else
                {
                    perspectCamera.IncomeLight(i, j, missSphPixel);
                }
            }
            else
            {
                perspectCamera.IncomeLight(i, j, missAABBPixel);
            }
        }
    }

    perspectCamera.m_film->SaveTo(OUTPUT_PATH + SUB_FOLDER + L"ThisImageIsForPerspectiveCameraRenderSphere.png");
    
    BlockShowImg(perspectCamera.m_film.get(), L"sphere in perspective camera");
}

void CASE_NAME_IN_COMMON_CLASSES(TriangleAndRay)::Run()
{
    Triangle tri(
        vector3(1.0f, 0.0f, 0.0f),
        vector3(-1.0f, 0.0f, 0.0f),
        vector3(0.0f, 2.0f, 1.0f));

    RGB hitPixel(1.0f, 1.0f, 1.0f);
    RGB missSphPixel(0.0f, 0.0f, 0.0f);
    RGB missAABBPixel(0.0f, 0.5f, 0.0f);
    RGB positivePixel(RGB::RED);
    RGB negativePixel(RGB::GREEN);

    vector3 camPosition = vector3(3.0f, 3.0f, 3.0f);
    vector3 camTarget = vector3(0.0f, 0.0f, 0.0f);
    vector3 camLookUp = vector3(0.0f, 1.0f, 0.0f);

    Types::F32 focalLength = 0.5f;
    PerspectiveCamera camera(focalLength, camPosition, camTarget, camLookUp);
    camera.SetFilm(std::make_unique<Film>(
        pEnvironment->COMMON_PIXEL_WIDTH, pEnvironment->COMMON_PIXEL_HEIGHT,
        -0.5f, +0.5f,
        -0.5f, +0.5f));

    HitRecord hitRec;

    for (unsigned int i = 0; i < camera.m_film->m_width; ++i)
    {
        for (unsigned int j = 0; j < camera.m_film->m_height; ++j)
        {
            Ray ray = camera.GetRay(i, j);

            //BREAK_POINT_IF(i == 256 && j == 256);

            // try bounding box first
            if (tri.BoundingBox().Hit(ray, 0.0f, 1000.0f, &hitRec))
            {

                // try triangle
                if (tri.Hit(ray, 0.0f, 1000.0f, &hitRec))
                {
                    if (!hitRec.m_isBackFace)
                    {
                        camera.IncomeLight(i, j, positivePixel);
                    }
                    else
                    {
                        //PUT_BREAK_POINT;
                        camera.IncomeLight(i, j, negativePixel);
                    }
                }
                else
                {
                    camera.IncomeLight(i, j, missSphPixel);
                }
            }
            else
            {
                camera.IncomeLight(i, j, missAABBPixel);
            }
        }
    }

    camera.m_film->SaveTo(OUTPUT_PATH + SUB_FOLDER + L"ThisImageIsForOrthoCameraRenderTriangle.png");

    BlockShowImg(camera.m_film.get(), L"triangle and ray");
}

void CASE_NAME_IN_COMMON_CLASSES(SceneAndRay)::Run()
{
    Scene testScene;

    auto sph1 = std::make_unique<Sphere>(vector3(0.0f, 0.0f, 0.0f), 1.0f);
    auto sph2 = std::make_unique<Sphere>(vector3(3.0f, 2.0f, 1.0f), 0.6f);

    const Types::F32 borderLength = 20.0f;
    auto tri1 = std::make_unique<Triangle>(
        vector3(-borderLength, -2.0f, -borderLength),
        vector3(-borderLength, -2.0f, borderLength),
        vector3(+borderLength, -2.0f, borderLength));
    auto tri2 = std::make_unique<Triangle>(
        vector3(-borderLength, -2.0f, -borderLength),
        vector3(+borderLength, -2.0f, +borderLength),
        vector3(+borderLength, -2.0f, -borderLength));

    auto poly = std::make_unique<Polygon>(
        vector3(-borderLength, -borderLength, -2.0f),
        vector3(-borderLength, borderLength, -2.0f),
        vector3(+borderLength, borderLength, -2.0f)
        );

    testScene.Add(std::move(sph1));
    testScene.Add(std::move(sph2));
    testScene.Add(std::move(tri1));
    testScene.Add(std::move(tri2));
    testScene.Add(std::move(poly));

    RGB backgroundColor(RGB::BLACK);
    RGB hitColor(RGB::RED);

    vector3 camPosition = vector3(3.0f, 4.0f, 2.0f);
    vector3 camTarget = vector3(0.0f, 0.0f, 0.0f);
    vector3 camLookUp = vector3(0.0f, 1.0f, 0.0f);

    Types::F32 focalLength = 0.5f;
    PerspectiveCamera camera(focalLength, camPosition, camTarget, camLookUp);
    camera.SetFilm(std::make_unique<Film>(
        pEnvironment->COMMON_PIXEL_WIDTH, pEnvironment->COMMON_PIXEL_HEIGHT,
        -0.5f, +0.5f,
        -0.5f, +0.5f));

    HitRecord hitRec;

    for (unsigned int i = 0; i < camera.m_film->m_width; ++i)
    {
        for (unsigned int j = 0; j < camera.m_film->m_height; ++j)
        {
            Ray ray = camera.GetRay(i, j);

            //BREAK_POINT_IF(i == 256 && j == 256);

            // try triangle
            if (testScene.Hit(ray, 0.0f, 1000.0f, &hitRec))
            {
                hitColor.SetChannel<RGB::R>(hitRec.m_hitT / 8.0f);

                camera.IncomeLight(i, j, hitColor);
            }
            else
            {
                camera.IncomeLight(i, j, backgroundColor);
            }
        }
    }

    camera.m_film->SaveTo(OUTPUT_PATH + SUB_FOLDER + L"ThisImageIsForSceneHit.png");

    BlockShowImg(camera.m_film.get(), L"scene and ray");
}

void CASE_NAME_IN_COMMON_CLASSES(PolygoneAndRay)::Run()
{
    const Types::F32 borderLength = 3.0f;

    auto poly = std::make_unique<Polygon>(
        vector3(-borderLength, -borderLength, +4.0f),
        vector3(-borderLength, borderLength, -4.0f),
        vector3(+borderLength, borderLength, -4.0f)
        );

    poly->AddPoint(vector3(borderLength * 0.9f, -borderLength * 3.0f, -2.0f));
    poly->AddPoint(vector3(borderLength * 0.1f, -borderLength * 4.0f, -2.0f));

    RGB backgroundColor(RGB::GREEN);
    backgroundColor = backgroundColor * (0.5f); // make color darker
    RGB hitBBoxColor(RGB::BLACK);
    RGB hitColor(RGB::RED);

    vector3 camPosition = vector3(-4.0f, 8.0f, 8.0f);
    vector3 camTarget = vector3(0.0f, 0.0f, 0.0f);
    vector3 camLookUp = vector3(0.0f, 1.0f, 0.0f);

    Types::F32 focalLength = 0.5f;
    PerspectiveCamera camera(focalLength, camPosition, camTarget, camLookUp);
    camera.SetFilm(std::make_unique<Film>(
        pEnvironment->COMMON_PIXEL_WIDTH, pEnvironment->COMMON_PIXEL_HEIGHT,
        -0.5f, +0.5f,
        -0.5f, +0.5f));

    HitRecord hitRec;

    for (unsigned int i = 0; i < camera.m_film->m_width; ++i)
    {
        for (unsigned int j = 0; j < camera.m_film->m_height; ++j)
        {
            Ray ray = camera.GetRay(i, j);

            //BREAK_POINT_IF(i == 278 && j == 192);
            //BREAK_POINT_IF(i == 185 && j == 195);

            // try triangle
            if (poly->BoundingBox().Hit(ray, 0.0f, 1000.f, &hitRec))
            {
                if (poly->Hit(ray, 0.0f, 1000.0f, &hitRec))
                {
                    hitColor.SetChannel<RGB::R>(hitRec.m_hitT / 4.0f);

                    camera.IncomeLight(i, j, hitColor);
                }
                else
                {
                    camera.IncomeLight(i, j, hitBBoxColor);
                }
            }
            else
            {
                camera.IncomeLight(i, j, backgroundColor);
            }

        }
    }

    camera.m_film->SaveTo(OUTPUT_PATH + SUB_FOLDER + L"ThisImageIsForHitPolygon.png");

    BlockShowImg(camera.m_film.get(), L"polygon and ray");
}

void CASE_NAME_IN_COMMON_CLASSES(PointLight)::Run()
{
    /*!
    `    \brief set scene and light.
    */
    Scene scene;
    RGB backgroundColor(RGB::BLACK);

    /*!
        \brief make up materials.
    */
    Material sphereMat;
    Material triMat;
    Material polyMat;

    sphereMat.m_kDiffuse        = RGB::RED;
    sphereMat.m_shinness        = 12.0f;
    sphereMat.SetRFresnel0(8);
    triMat.m_kDiffuse           = RGB::GREEN;
    triMat.m_shinness           = 1.0f;
    triMat.SetRFresnel0(8);
    polyMat.m_kDiffuse          = RGB(0.3f, 0.5f, 0.9f);
    polyMat.m_shinness          = 4.0f;
    polyMat.SetRFresnel0(8);

    vector3 pointLightPosition(-3.0f, 4.0f, 3.0f);
    RGB pointLightColor = RGB::WHITE;
    Light pointLight(pointLightPosition * 5.0f, pointLightColor);

    /*!
        \brief set a sphere to render.
    */
    auto tsph = std::make_unique<Sphere>(vector3(0.0f, 0.0f, 0.0f), 1.0f);
    tsph->m_material = sphereMat;
    auto tsph2 = std::make_unique<Sphere>(vector3(-1.0f, 2.0f, 0.5f), 1.0f);
    tsph2->m_material = sphereMat;

    /*!
        \brief render triangles
    */
    const Types::F32 borderLength = 20.0f;
    auto tri1 = std::make_unique<Triangle>(
        vector3(-borderLength, -2.0f, +borderLength),
        vector3(-borderLength, -2.0f, -borderLength),
        vector3(+borderLength, -2.0f, borderLength));
    auto tri2 = std::make_unique<Triangle>(
        vector3(+borderLength, -2.0f, +borderLength),
        vector3(-borderLength, -2.0f, -borderLength),
        vector3(+borderLength, -2.0f, -borderLength));
    tri1->m_material = triMat;
    tri2->m_material = triMat;

    /*!
        \brief render polygon
    */
    auto poly = std::make_unique<Polygon>(
        vector3(-borderLength, -borderLength, -2.0f),
        vector3(-borderLength, borderLength, -2.0f),
        vector3(+borderLength, borderLength, -2.0f)
        );
    poly->AddPoint(vector3(borderLength * 0.9f, -borderLength * 3.0f, -2.0f));
    poly->AddPoint(vector3(borderLength * 0.1f, -borderLength * 4.0f, -2.0f));
    poly->m_material = polyMat;


    scene.Add(std::move(tsph));
    scene.Add(std::move(tsph2));
    scene.Add(std::move(tri1));
    scene.Add(std::move(tri2));
    scene.Add(std::move(poly));

    /*!
        \brief config a camera.
    */
    //vector3 camPosition = vector3(1.8f, 1.8f, 1.8f);
    vector3 camPosition = 2.0f * vector3(1.8f, 1.8f, 1.8f);
    //vector3 camPosition = vector3( -3.8f, 1.0f, 1.8f);
    vector3 camTarget = vector3(0.0f, 0.0f, 0.0f);
    vector3 camLookUp = vector3(0.0f, 1.0f, 0.0f);
    Types::F32 focalLength = 0.5f;
    PerspectiveCamera camera(focalLength, camPosition, camTarget, camLookUp);

    camera.SetFilm(std::make_unique<Film>(
        pEnvironment->COMMON_PIXEL_WIDTH, pEnvironment->COMMON_PIXEL_HEIGHT,
        -0.5f, +0.5f,
        -0.5f, +0.5f));

    HitRecord hitRec, shadowHitRec;
    Ray viewRay;
    for (unsigned int i = 0; i < camera.m_film->m_width; ++i)
    {
        for (unsigned int j = 0; j < camera.m_film->m_height; ++j)
        {
            viewRay = camera.GetRay(i, j);

            if ((i % 10) == 0 && (j % 10) == 0)
            {
                ShowProgress((j + i * camera.m_film->m_width) * 1.0f / (camera.m_film->m_width * camera.m_film->m_height));
            }
                
            //BREAK_POINT_IF(i == 224 && j == 511 - 231);

            if (scene.Hit(viewRay, 0.0f, 1000.0f, &hitRec))
            {
                RGB color;

                color = hitRec.m_material.m_kDiffuse * scene.m_ambient;

                Types::F32 toLightDist = 0.0f;
                vector3 toLight = pointLight.ToMeFrom(hitRec.m_hitPoint, &toLightDist);

                Ray shadowRayTest = Ray(hitRec.m_hitPoint, toLight);

                if ( ! scene.Hit(shadowRayTest, 0.0f, toLightDist, &shadowHitRec))
                {
                    vector3 toEye = -viewRay.m_direction;
                    vector3 halfVec = Normalize(toEye + toLight);

                    RGB lightStrength = pointLight.m_color * std::max(0.0f, hitRec.m_normal * toLight);

                    const Types::F32 m = hitRec.m_material.m_shinness;

                    Types::F32 shinnessSthrength = (m + 8) / 8 * std::powf(halfVec * hitRec.m_normal, m);

                    color = color + lightStrength * (hitRec.m_material.m_kDiffuse + hitRec.m_material.RFresnel(toEye * hitRec.m_normal) * shinnessSthrength);
                }

                camera.IncomeLight(i, j, color);
            }
            else
            {
                camera.IncomeLight(i, j, backgroundColor);
            }
        }
    }

    camera.m_film->SaveTo(OUTPUT_PATH + SUB_FOLDER + L"ThisImageIsForTempPointLight15.png");

    BlockShowImg(camera.m_film.get(), L"there is a light");
}

void CASE_NAME_IN_COMMON_CLASSES(RayColorFunction)::Run()
{
    /*!
    `    \brief set scene and light.
    */
    Scene scene;
    RGB backgroundColor(RGB::BLACK);

    /*!
        \brief make up materials.
    */
    Material sphereMat;
    Material triMat;
    Material polyMat;

    sphereMat.m_kDiffuse        = RGB::RED;
    sphereMat.m_shinness        = 12.0f;
    sphereMat.SetRFresnel0(8);
    triMat.m_kDiffuse           = RGB::YELLOW;
    triMat.m_shinness           = 1.0f;
    triMat.SetRFresnel0(8);
    polyMat.m_kDiffuse          = RGB(0.3f, 0.5f, 0.9f);
    polyMat.m_shinness          = 0.5f;
    polyMat.SetRFresnel0(8);

    vector3 pointLightPosition(-3.0f, 4.0f, 3.0f);
    RGB pointLightColor = RGB::WHITE;
    scene.Add(std::make_unique<Light>(pointLightPosition * 5.0f, pointLightColor));


    /*!
        \brief set spheres to render.
    */
    auto tsph = std::make_unique<Sphere>(vector3(0.0f, 0.0f, 0.0f), 1.0f);
    tsph->m_material = sphereMat;
    auto tsph2 = std::make_unique<Sphere>(vector3(-1.0f, 2.0f, 0.5f), 1.0f);
    tsph2->m_material = sphereMat;
    auto tsph3 = std::make_unique<Sphere>(vector3(-1.0f, 0.0f, 2.0f), 1.0f);
    tsph3->m_material = sphereMat;

    /*!
        \brief render triangles
    */
    const Types::F32 borderLength = 20.0f;
    auto tri1 = std::make_unique<Triangle>(
        vector3(-borderLength, -2.0f, +borderLength),
        vector3(-borderLength, -2.0f, -borderLength),
        vector3(+borderLength, -2.0f, borderLength));
    auto tri2 = std::make_unique<Triangle>(
        vector3(+borderLength, -2.0f, +borderLength),
        vector3(-borderLength, -2.0f, -borderLength),
        vector3(+borderLength, -2.0f, -borderLength));
    tri1->m_material = triMat;
    tri2->m_material = triMat;

    /*!
        \brief render polygon
    */
    auto poly = std::make_unique<Polygon>(
        vector3(-borderLength, -borderLength, -2.0f),
        vector3(-borderLength, borderLength, -2.0f),
        vector3(+borderLength, borderLength, -2.0f)
        );
    poly->AddPoint(vector3(borderLength * 0.9f, -borderLength * 3.0f, -2.0f));
    poly->AddPoint(vector3(borderLength * 0.1f, -borderLength * 4.0f, -2.0f));
    poly->m_material = polyMat;


    scene.Add(std::move(tsph));
    scene.Add(std::move(tsph2));
    scene.Add(std::move(tsph3));
    scene.Add(std::move(tri1));
    scene.Add(std::move(tri2));
    scene.Add(std::move(poly));

    /*!
        \brief config a camera.
    */
    vector3 camPosition = vector3(1.8f, 1.8f, 1.8f);
    //vector3 camPosition = 2.0f * vector3(1.8f, 1.8f, 1.8f);
    //vector3 camPosition = vector3( -3.8f, 1.0f, 1.8f);
    vector3 camTarget = vector3(0.0f, 0.0f, 0.0f);
    vector3 camLookUp = vector3(0.0f, 1.0f, 0.0f);
    Types::F32 focalLength = 0.5f;
    PerspectiveCamera camera(focalLength, camPosition, camTarget, camLookUp);

    camera.SetFilm(std::make_unique<Film>(
        pEnvironment->COMMON_PIXEL_WIDTH, pEnvironment->COMMON_PIXEL_HEIGHT,
        -0.5f, +0.5f,
        -0.5f, +0.5f));

    HitRecord hitRec, shadowHitRec;
    Ray viewRay;

    const int WIDTH = camera.m_film->m_width, HEIGHT = camera.m_film->m_height;
    for (unsigned int i = 0; i < WIDTH; ++i)
    {
        for (unsigned int j = 0; j < HEIGHT; ++j)
        {
            viewRay = camera.GetRay(i, j);

            camera.IncomeLight(i, j, scene.RayColor(viewRay, 0.0f, 1000.0f));

            if (j % 10 == 0)
            {
                ShowProgress((i * HEIGHT + j) * 1.0f / (WIDTH * HEIGHT));
            }
        }
    }

    camera.m_film->SaveTo(OUTPUT_PATH + SUB_FOLDER + L"ThisIsForSceneRayColor06.png");

    BlockShowImg(camera.m_film.get(), L"power of Scene::RayColor()");
}
