#pragma once

#include "CommonHeaders.h"

#define CASE_NAME_IN_RAY_RENDER(RestClassName) CaseFor##RestClassName

#define DECLARE_CASE_IN_RAY_RENDER_FOR(RestClassName, CaseName) \
class CASE_NAME_IN_RAY_RENDER(RestClassName) : public CaseForPipline\
{\
public:\
    CASE_NAME_IN_RAY_RENDER(RestClassName)() :CaseForPipline(CaseName) {}\
    virtual void Run() override;\
    virtual std::wstring GetStoragePath() const override {return OUTPUT_PATH + SUB_FOLDER;}\
protected:\
    const std::wstring SUB_FOLDER = L"RayRender/";\
}