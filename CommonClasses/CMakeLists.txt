# build CommonClass static library
cmake_minimum_required(VERSION 3.7.1)

project(LearningFundamentalsOfComputerGraphic)

set (COMMON_CLASS_SOURCE_FILES
	AABB.h
	Box.h
	Camera.h
	CameraFrame.h
	ColorTemplate.h
	CoordinateFrame.h
	DebugConfigs.h
	DepthBuffer.h
	EdgeEquation2D.h
	EFloat.h
	F32Buffer.h
	Film.h
	Filter.h
	FixPointNumber.h
	Frame.h
	GeomentryBuilder.h
	Helpers.h
	HitRecord.h
	HPlaneEquation.h
	Image.h
	ImageWindow.h
	Light.h
	Material.h
	OrthographicCamera.h
	PerspectiveCamera.h
	Pipline.h
	PiplineStateObject.h
	Polygon.h
	Ray.h
	Scene.h
	ScreenSpaceVertexTemplate.h
	Sphere.h
	Surface.h
	Transform.h
	Triangle.h
	vector2.h
	vector3.h
	vector4.h
	Texture.h
	GraphicToolSet.h
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
	Image.cpp
	ImageWindow.cpp
	Light.cpp
	Material.cpp
	OrthographicCamera.cpp
	PerspectiveCamera.cpp
	Pipline.cpp
	PiplineStateObject.cpp
	Polygon.cpp
	Ray.cpp
	Scene.cpp
	ScreenSpaceVertexTemplate.cpp
	Sphere.cpp
	Surface.cpp
	Transform.cpp
	Triangle.cpp
	vector2.cpp
	vector3.cpp
	vector4.cpp
	Texture.cpp
	GraphicToolSet.cpp
	# Utils
	Utils/MTRandom.h
	Utils/MTRandom.cpp
	Utils/MathTool.h
	Utils/stb_image.h
	Utils/svpng.inc
	#TestTool
	Utils/TestTool/Case.h
	Utils/TestTool/Miscellaneous.h
	Utils/TestTool/Suit.h
	Geometry.h
	Geometry.cpp)

# set the CommonClass directory
add_library(CommonClass ${COMMON_CLASS_SOURCE_FILES})

target_link_libraries(CommonClass glog)

message(STATUS "CommonClass source files = " "${COMMON_CLASS_SOURCE_FILES}")
