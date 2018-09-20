# build CommonClass static library
cmake_minimum_required(VERSION 3.7.1)

project(LearningFundamentalsOfComputerGraphic)

set (COMMON_CLASS_SOURCE_FILES
	AABB.cpp
	Box.cpp
	Camera.cpp
	CameraFrame.cpp
	ColorTemplate.cpp
	CoordinateFrame.cpp
	DebugConfigs.cpp
	DepthBuffer.cpp
	EdgeEquation2D.cpp
	EFloat.cpp
	F32Buffer.cpp
	Film.cpp
	Filter.cpp
	FixPointNumber.cpp
	Frame.cpp
	GeomentryBuilder.cpp
	Helpers.cpp
	HitRecord.cpp
	HPlaneEquation.cpp
	hvector.cpp
	Image.cpp
	ImageWindow.cpp
	Light.cpp
	Material.cpp
	OrthographicCamera.cpp
	PerspectiveCamera.cpp
	Pipline.cpp
	PiplineStateObject.cpp
	Polygon.cpp
	RasterizeImage.cpp
	Ray.cpp
	Scene.cpp
	ScreenSpaceVertexTemplate.cpp
	Sphere.cpp
	Surface.cpp
	Transform.cpp
	Triangle.cpp
	vector2.cpp
	vector3.cpp
	Texture.cpp
	GraphicToolSet.cpp)

# set the CommonClass directory
add_library(CommonClass ${COMMON_CLASS_SOURCE_FILES})

message(STATUS "CommonClass source files = " "${COMMON_CLASS_SOURCE_FILES}")
