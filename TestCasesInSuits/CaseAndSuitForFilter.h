#pragma once

#include "CommonHeaders.h"

// help to get case name
#define CASE_NAME_IN_FILTER(RestClassName) CaseFor##RestClassName

// help to declare class body for case class
#define DECLARE_CASE_IN_FILTER_FOR(RestClassName, CaseName) \
class CASE_NAME_IN_FILTER(RestClassName) : public CaseForPipline\
{\
public:\
    CASE_NAME_IN_FILTER(RestClassName)() :CaseForPipline(CaseName) {}\
    virtual void Run() override;\
    virtual std::wstring GetStoragePath() const override {return OUTPUT_PATH + SUB_FOLDER;}\
protected:\
    const std::wstring SUB_FOLDER = L"Filter/";\
}

DECLARE_CASE_IN_FILTER_FOR(Basic, "the basic functionality of filter");

DECLARE_CASE_IN_FILTER_FOR(GenerateLocMap, "the basic functionality of filter");

DECLARE_CASE_IN_FILTER_FOR(Exercise, "the basic functionality of filter");

using SuitForFilter = SuitForPipline<
    CASE_NAME_IN_FILTER(Basic),
    CASE_NAME_IN_FILTER(GenerateLocMap),
    CASE_NAME_IN_FILTER(Exercise)
>;