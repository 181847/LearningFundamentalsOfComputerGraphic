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

DECLARE_CASE_FOR(CameraBasis, "check camera's basis is constructed correctly");

DECLARE_CASE_FOR(CameraCaptureLight, "test capmera capture light");

DECLARE_CASE_FOR(RayConstruct, "ray construct");

DECLARE_CASE_FOR(RayFromOrthoCamera, "get ray from orthographic camera");

DECLARE_CASE_FOR(CameraPixelLoc, "test camere pixel location");

DECLARE_CASE_FOR(OrthoCameraAndSphere, "check orhographic camera and sphere-ray collision");

DECLARE_CASE_FOR(ConstColors, "show the constant colors");

DECLARE_CASE_FOR(PerspectiveCameraAndSphere, "perspective camera and sphere-ray collision");

DECLARE_CASE_FOR(TriangleAndRay, "ray hit triangle");

DECLARE_CASE_FOR(SceneAndRay, "hit with scene");

DECLARE_CASE_FOR(PolygoneAndRay, "hit with polygon");

DECLARE_CASE_FOR(PointLight, "render a scene with a point light");

DECLARE_CASE_FOR(RayColorFunction, "render a scene with Scene::RayColor()");

using SuitForCommonClasses =
SuitForPipline<
    CASE_NAME(BasicImage),
    CASE_NAME(RGBAlphaSetting),
    CASE_NAME(FilmCaptureLight),
    CASE_NAME(CameraBasis),
    CASE_NAME(CameraCaptureLight),
    CASE_NAME(RayConstruct),
    CASE_NAME(RayFromOrthoCamera),
    CASE_NAME(CameraPixelLoc),
    CASE_NAME(OrthoCameraAndSphere),
    CASE_NAME(ConstColors),
    CASE_NAME(PerspectiveCameraAndSphere),
    CASE_NAME(TriangleAndRay),
    CASE_NAME(SceneAndRay),
    CASE_NAME(PolygoneAndRay),
    CASE_NAME(PointLight),
    CASE_NAME(RayColorFunction)
>;