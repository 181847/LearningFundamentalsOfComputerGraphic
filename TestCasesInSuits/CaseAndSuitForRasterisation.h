#pragma once

#include "CommonHeaders.h"

// help to get case name
#define CASE_NAME_IN_RASTERISATION(RestClassName) CaseFor##RestClassName

// help to declare class body for case class
#define DECLARE_CASE_IN_RASTERISATION_FOR(RestClassName, CaseName) \
class CASE_NAME_IN_RASTERISATION(RestClassName) : public CaseForPipline\
{\
public:\
    CASE_NAME_IN_RASTERISATION(RestClassName)() :CaseForPipline(CaseName) {}\
    virtual void Run() override;\
    virtual std::wstring GetStoragePath() const override {return OUTPUT_PATH + SUB_FOLDER;}\
protected:\
    const std::wstring SUB_FOLDER = L"Rasterisation/";\
}

DECLARE_CASE_IN_RASTERISATION_FOR(WuXiaolinLines, "draw WuXiaolin lines");

DECLARE_CASE_IN_RASTERISATION_FOR(BresenhamLine, "draw bresenham lines");

DECLARE_CASE_IN_RASTERISATION_FOR(2DLineClip, "draw bresenham lines");

DECLARE_CASE_IN_RASTERISATION_FOR(FlagBottomTriangle, "draw triangle flat bottom");

DECLARE_CASE_IN_RASTERISATION_FOR(GeneralTriangle, "draw general triangle");

DECLARE_CASE_IN_RASTERISATION_FOR(LineInPipline, "draw general triangle");

DECLARE_CASE_IN_RASTERISATION_FOR(LineClipping, "draw general triangle");

DECLARE_CASE_IN_RASTERISATION_FOR(VertexTransform, "vertex transformation");

DECLARE_CASE_IN_RASTERISATION_FOR(OrthoTransform, "orthographic view transformation");

DECLARE_CASE_IN_RASTERISATION_FOR(PerspectTransform, "perspective view transformation");

DECLARE_CASE_IN_RASTERISATION_FOR(LineClippingErrorAnalysis, "line clipping analysis");

using SuitForRasterisation = SuitForPipline<
    CASE_NAME_IN_RASTERISATION(WuXiaolinLines),
    CASE_NAME_IN_RASTERISATION(BresenhamLine),
    CASE_NAME_IN_RASTERISATION(2DLineClip),
    CASE_NAME_IN_RASTERISATION(FlagBottomTriangle),
    CASE_NAME_IN_RASTERISATION(GeneralTriangle),
    CASE_NAME_IN_RASTERISATION(LineInPipline),
    CASE_NAME_IN_RASTERISATION(LineClipping),
    CASE_NAME_IN_RASTERISATION(VertexTransform),
    CASE_NAME_IN_RASTERISATION(OrthoTransform),
    CASE_NAME_IN_RASTERISATION(PerspectTransform),
    CASE_NAME_IN_RASTERISATION(LineClippingErrorAnalysis)
>;