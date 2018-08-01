#pragma once

#include "CommonHeaders.h"


#define CASE_NAME_IN_COMMON_CLASSES(RestClassName) CaseFor##RestClassName

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
#define DECLARE_CASE_IN_COMMON_CLASSES_FOR(RestClassName, CaseName) \
class CASE_NAME_IN_COMMON_CLASSES(RestClassName) : public CaseForPipline\
{\
public:\
    CASE_NAME_IN_COMMON_CLASSES(RestClassName)() :CaseForPipline(CaseName) {}\
    virtual void Run() override;\
protected:\
    const std::wstring SUB_FOLDER = L"ForCommonClasses/";\
}

DECLARE_CASE_IN_COMMON_CLASSES_FOR(BasicImage, "test Basic Image");

DECLARE_CASE_IN_COMMON_CLASSES_FOR(RGBAlphaSetting, "set color by RGB and Alpha");

DECLARE_CASE_IN_COMMON_CLASSES_FOR(FilmCaptureLight, "film capture light test");

DECLARE_CASE_IN_COMMON_CLASSES_FOR(CameraBasis, "check camera's basis is constructed correctly");

DECLARE_CASE_IN_COMMON_CLASSES_FOR(CameraCaptureLight, "test capmera capture light");

DECLARE_CASE_IN_COMMON_CLASSES_FOR(RayConstruct, "ray construct");

DECLARE_CASE_IN_COMMON_CLASSES_FOR(RayFromOrthoCamera, "get ray from orthographic camera");

DECLARE_CASE_IN_COMMON_CLASSES_FOR(CameraPixelLoc, "test camere pixel location");

DECLARE_CASE_IN_COMMON_CLASSES_FOR(OrthoCameraAndSphere, "check orhographic camera and sphere-ray collision");

DECLARE_CASE_IN_COMMON_CLASSES_FOR(ConstColors, "show the constant colors");

DECLARE_CASE_IN_COMMON_CLASSES_FOR(PerspectiveCameraAndSphere, "perspective camera and sphere-ray collision");

DECLARE_CASE_IN_COMMON_CLASSES_FOR(TriangleAndRay, "ray hit triangle");

DECLARE_CASE_IN_COMMON_CLASSES_FOR(SceneAndRay, "hit with scene");

DECLARE_CASE_IN_COMMON_CLASSES_FOR(PolygoneAndRay, "hit with polygon");

DECLARE_CASE_IN_COMMON_CLASSES_FOR(PointLight, "render a scene with a point light");

DECLARE_CASE_IN_COMMON_CLASSES_FOR(RayColorFunction, "render a scene with Scene::RayColor()");

using SuitForCommonClasses =
SuitForPipline<
    CASE_NAME_IN_COMMON_CLASSES(BasicImage),
    CASE_NAME_IN_COMMON_CLASSES(RGBAlphaSetting),
    CASE_NAME_IN_COMMON_CLASSES(FilmCaptureLight),
    CASE_NAME_IN_COMMON_CLASSES(CameraBasis),
    CASE_NAME_IN_COMMON_CLASSES(CameraCaptureLight),
    CASE_NAME_IN_COMMON_CLASSES(RayConstruct),
    CASE_NAME_IN_COMMON_CLASSES(RayFromOrthoCamera),
    CASE_NAME_IN_COMMON_CLASSES(CameraPixelLoc),
    CASE_NAME_IN_COMMON_CLASSES(OrthoCameraAndSphere),
    CASE_NAME_IN_COMMON_CLASSES(ConstColors),
    CASE_NAME_IN_COMMON_CLASSES(PerspectiveCameraAndSphere),
    CASE_NAME_IN_COMMON_CLASSES(TriangleAndRay),
    CASE_NAME_IN_COMMON_CLASSES(SceneAndRay),
    CASE_NAME_IN_COMMON_CLASSES(PolygoneAndRay),
    CASE_NAME_IN_COMMON_CLASSES(PointLight),
    CASE_NAME_IN_COMMON_CLASSES(RayColorFunction)
>;
