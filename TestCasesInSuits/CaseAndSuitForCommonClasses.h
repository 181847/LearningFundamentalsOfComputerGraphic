#pragma once

#include "CommonHeaders.h"

/*!
    \brief declare a class for general using.
    \macroParam RestClassName the rest name of Case,
    \macroParam CaseName the inner name for this kind of case, it should be a string.
    for example:
      DECLARE_CASE_FOR(BasicImage, "test Basic Image");
    will declare a class named as CaseForBasicImage, 
    and if you call CaseForBasicImage::GetName() will return "test Basic Image".

    WARNING:
        you must implement the Run() function in a *.cpp file, or you will have a compile error.
*/
#define DECLARE_CASE_FOR(RestClassName, CaseName) \
class CaseFor##RestClassName : public CaseForPipline\
{\
public:\
    CaseFor##RestClassName() :CaseForPipline(CaseName) {}\
    virtual void Run() override;\
protected:\
    const std::wstring SUB_FOLDER = L"ForCommonClasses/";\
}

#define CASE_NAME(RestClassName) CaseFor##RestClassName

DECLARE_CASE_FOR(BasicImage, "test Basic Image");

DECLARE_CASE_FOR(RGBAlphaSetting, "set color by RGB and Alpha");

DECLARE_CASE_FOR(FilmCaptureLight, "film capture light test");

using SuitForCommonClasses =
SuitForPipline<
    CASE_NAME(BasicImage),
    CASE_NAME(RGBAlphaSetting),
    CASE_NAME(FilmCaptureLight)
>;