#pragma once

#include "CommonHeaders.h"

// help to get case name
#define CASE_NAME_IN_STB_IMG(RestClassName) CaseFor##RestClassName

// help to declare class body for case class
#define DECLARE_CASE_IN_STB_IMG_FOR(RestClassName, CaseName) \
class CASE_NAME_IN_STB_IMG(RestClassName) : public CaseForPipline\
{\
public:\
    CASE_NAME_IN_STB_IMG(RestClassName)() :CaseForPipline(CaseName) {}\
    virtual void Run() override;\
    virtual std::wstring GetStoragePath() const override {return Super::GetStoragePath() + SUB_FOLDER;}\
    const std::string LOAD_IMAGE_PATH = "../OutputTestImage/RayRender/";\
protected:\
    const std::wstring SUB_FOLDER = L"StbImg/";\
}

//DECLARE_CASE_IN_STB_IMG_FOR(BasicTest, "load image using stb_image.h");

DECLARE_CASE_IN_STB_IMG_FOR(TextureLoad, "load texture by file");

DECLARE_CASE_IN_STB_IMG_FOR(TextureSample, "load texture by file");

DECLARE_CASE_IN_STB_IMG_FOR(PerlinNoiseTexture, "perlin noise texture");

DECLARE_CASE_IN_STB_IMG_FOR(NoiseVecTexture, "perlin noise texture");

using SuitForStbImage = SuitForPipline<
    //CASE_NAME_IN_STB_IMG(TextureLoad),
    CASE_NAME_IN_STB_IMG(TextureSample),
    CASE_NAME_IN_STB_IMG(PerlinNoiseTexture),
    CASE_NAME_IN_STB_IMG(NoiseVecTexture)
>;