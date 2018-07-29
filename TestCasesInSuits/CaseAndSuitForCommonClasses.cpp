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

    testImage.SaveTo(L"OutputTestImage\\ThisImageIsForTest.png");

    ImageWindow imgWnd(&testImage, L"basic image");
}
