#pragma once

#include "CommonHeaders.h"

#define CASE_NAME_IN_RASTER_TRI(RestClassName) CaseFor##RestClassName

#define DECLARE_CASE_IN_RASTER_TRI_FOR(RestClassName, CaseName) \
class CASE_NAME_IN_RASTER_TRI(RestClassName) : public CaseForPipline\
{\
public:\
    CASE_NAME_IN_RASTER_TRI(RestClassName)() :CaseForPipline(CaseName) {}\
    virtual void Run() override;\
    virtual std::wstring GetStoragePath() const override {return Super::GetStoragePath() + SUB_FOLDER;}\
protected:\
    const std::wstring SUB_FOLDER = L"RasterizeTriangle/";\
}

DECLARE_CASE_IN_RASTER_TRI_FOR(DrawTriInScreenSpace, "directly draw triangle in screen space");

DECLARE_CASE_IN_RASTER_TRI_FOR(SphereRayTriangle, "basic sphereRayTri test");

DECLARE_CASE_IN_RASTER_TRI_FOR(TriangleCut, "triangle cut test");

DECLARE_CASE_IN_RASTER_TRI_FOR(MultipleCutTriangle, "multiple planes cut tirangle");

DECLARE_CASE_IN_RASTER_TRI_FOR(AbstractFrustrumCut, "abstract frustrum plane test");

DECLARE_CASE_IN_RASTER_TRI_FOR(CubeMesh, "render a cube mesh builded by triangles");

DECLARE_CASE_IN_RASTER_TRI_FOR(CylinderMesh, "render a cylinder mesh builded by triangles");

DECLARE_CASE_IN_RASTER_TRI_FOR(SphereMesh, "render a sphere mesh builded by triangles");

DECLARE_CASE_IN_RASTER_TRI_FOR(GeoSphereMesh, "render a geo sphere mesh builded by triangles");

DECLARE_CASE_IN_RASTER_TRI_FOR(UsingCameraFrame, "render a geo sphere mesh builded by triangles");

DECLARE_CASE_IN_RASTER_TRI_FOR(PixelShading, "simple shading equation");

DECLARE_CASE_IN_RASTER_TRI_FOR(TextureMapping, "mapping texture to cube");

DECLARE_CASE_IN_RASTER_TRI_FOR(NoiseBumpMap, "noise normal from a texture");

using SuitForRasterizeTriangle = SuitForPipline<
    CASE_NAME_IN_RASTER_TRI(DrawTriInScreenSpace),
    CASE_NAME_IN_RASTER_TRI(SphereRayTriangle),
    CASE_NAME_IN_RASTER_TRI(TriangleCut),
    CASE_NAME_IN_RASTER_TRI(MultipleCutTriangle),
    CASE_NAME_IN_RASTER_TRI(AbstractFrustrumCut),
    CASE_NAME_IN_RASTER_TRI(CubeMesh),
    CASE_NAME_IN_RASTER_TRI(CylinderMesh),
    CASE_NAME_IN_RASTER_TRI(SphereMesh),
    CASE_NAME_IN_RASTER_TRI(GeoSphereMesh),
    CASE_NAME_IN_RASTER_TRI(UsingCameraFrame),
    CASE_NAME_IN_RASTER_TRI(PixelShading),
    CASE_NAME_IN_RASTER_TRI(TextureMapping),
    CASE_NAME_IN_RASTER_TRI(NoiseBumpMap)
>;

