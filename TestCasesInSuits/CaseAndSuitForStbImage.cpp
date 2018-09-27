#include "CaseAndSuitForStbImage.h"

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

            u = u * 3.0f - 1.0f;
            v = v * 2.0f;

            samplingResult.SetPixel(x, y, tex.Sample(u, v));
        }
    }

    BlockShowImg(&samplingResult, L"sampling image result");
    samplingResult.SaveTo(GetSafeStoragePath() + L"resamplingImage.png");
}

void CASE_NAME_IN_STB_IMG(PerlinNoiseTexture)::Run()
{
    using namespace Types;
    Image img(512, 512);

    {
        COUNT_DETAIL_TIME;
        for (U32 x = 0; x < 512; ++x)
        {
            for (U32 y = 0; y < 512; ++y)
            {
                vector3 p(x * 1.0f, y / 1.0f, 0.0f);
                /*F32 noiseIntensity =
                    std::sin(
                        p.m_x + std::abs(
                            Texture::PerlinNoise(p) +
                            0.5f   * Texture::PerlinNoise(2.0f * p) +
                            0.25f  * Texture::PerlinNoise(4.0f * p) +
                            0.125f * Texture::PerlinNoise(8.0f * p)));*/
                p = p * (1.0f / 64.0f);
                
                F32 noiseIntensity = Texture::PerlinNoise(p);
                //if (noiseIntensity > 0.0f)
                {
                    noiseIntensity *= 20.0f;
                    noiseIntensity = noiseIntensity - (int)noiseIntensity;

                    const F32 colorScale = (noiseIntensity + 1.0f) * 0.5f;

                    img.SetPixel(x, y, vector3::WHITE * colorScale);
                }
            }
        }
    }

    this->BlockShowImg(&img, L"noise texture");
    img.SaveTo(GetSafeStoragePath() + L"perlinNoise_04.png");
}

void CASE_NAME_IN_STB_IMG(NoiseVecTexture)::Run()
{
    using namespace Types;
    Image img(512, 512);

    {
        COUNT_DETAIL_TIME;
        for (U32 x = 0; x < 512; ++x)
        {
            for (U32 y = 0; y < 512; ++y)
            {
                vector3 p(x * 1.0f, y / 1.0f, 0.0f);
                /*F32 noiseIntensity =
                    std::sin(
                        p.m_x + std::abs(
                            Texture::PerlinNoise(p) +
                            0.5f   * Texture::PerlinNoise(2.0f * p) +
                            0.25f  * Texture::PerlinNoise(4.0f * p) +
                            0.125f * Texture::PerlinNoise(8.0f * p)));*/
                p = p * (1.0f / 32.0f);

                vector3 noiseVec = Texture::NoiseVector3(p);// + 0.5f * Texture::NoiseVector3(2.0f * p) + 0.25f * Texture::NoiseVector3(4.0f * p) + 0.125f * Texture::NoiseVector3(8.0f * p);
                noiseVec = 0.5f * (noiseVec + vector3::UNIT);
                vector3 noiseColor(noiseVec.m_x, noiseVec.m_y, noiseVec.m_z);
                img.SetPixel(x, y, noiseColor);
            }
        }
    }

    this->BlockShowImg(&img, L"noise vector3 texture");
    img.SaveTo(GetSafeStoragePath() + L"noiseVector3_06.png");
}
