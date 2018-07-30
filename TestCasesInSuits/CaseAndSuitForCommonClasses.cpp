#include "CaseAndSuitForCommonClasses.h"

void CASE_NAME(BasicImage)::Run()
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

void CASE_NAME(RGBAlphaSetting)::Run()
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

void CASE_NAME(FilmCaptureLight)::Run()
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
