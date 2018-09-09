#include "CaseAndSuitForStbImage.h"

//void CASE_NAME_IN_STB_IMG(BasicTest)::Run()
//{
//    int x, y, n = 4;
//    //std::string imagePath = LOAD_IMAGE_PATH + "InsideBox29.png";
//    std::string imagePath = LOAD_IMAGE_PATH + "OldImgs/RenderReflect/ThisIsForSceneRayColor06.png";
//    unsigned char *data = stbi_load(imagePath.c_str(), &x, &y, &n, 0);
//
//    if (!data)
//    {
//        printf("Error!, cannot load image: %s\n", imagePath.c_str());
//        return;
//    }
//
//    Image restoreImage(x, y);
//    unsigned char * restoreAddr = restoreImage.GetRawData();
//
//    memcpy(restoreAddr, data, x * y * 4);
//    
//    BlockShowImg(&restoreImage, L"restored image");
//
//    stbi_image_free(data);
//}

void CASE_NAME_IN_STB_IMG(TextureLoad)::Run()
{
    Texture tex;

    tex.LoadFile("d:\\picture\\Maria\\sideShowToy\\bloodborne-lady-maria-of-the-astral-clocktower-statue-prime1-studio-902974-04.jpg");
    BlockShowImg(&tex, L"test texture");
    tex.LoadFile("d:\\picture\\Maria\\Âí¼×\\images.jpg");
    BlockShowImg(&tex, L"test texture");
    tex.LoadFile("d:\\picture\\ThisImageIsForPerspectiveCameraRenderSphere.png");
    BlockShowImg(&tex, L"test texture");

    tex.SaveTo(this->GetSafeStoragePath() + L"stb_reoutput.png");
}

void CASE_NAME_IN_STB_IMG(TextureSample)::Run()
{
    const Types::U32 CWIDTH(512), CHEIGHT(512);
    Image samplingResult(CWIDTH, CHEIGHT);

    Texture tex;
    tex.LoadFile("d:\\picture\\Maria\\sideShowToy\\bloodborne-lady-maria-of-the-astral-clocktower-statue-prime1-studio-902974-04.jpg");
    BlockShowImg(&tex, L"loaded texture");

    for (Types::U32 x = 0; x < CWIDTH; ++x)
    {
        for (Types::U32 y = 0; y < CHEIGHT; ++y)
        {
            Types::F32 u(x * 1.0f / CWIDTH), v(y * 1.0f / CHEIGHT);

            samplingResult.SetPixel(x, y, tex.Sample(u, v));
        }
    }

    BlockShowImg(&samplingResult, L"sampling image result");
    samplingResult.SaveTo(GetSafeStoragePath() + L"resamplingImage.png");
}
