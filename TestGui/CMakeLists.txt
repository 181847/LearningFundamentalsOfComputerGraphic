#build gui application for testing my rendering pipline
cmake_minimum_required(VERSION 3.7.1)

set(TEST_GUI_SOURCE_FILES
	BaseFrame.h
	App.h
	ImguiWrapImageDX11.h
	BaseFrame.cpp
	App.cpp
	ImguiWrapImageDX11.cpp
	AbstractGui.h
	AbstractGui.cpp
	RasterizeGui.h
	RasterizeGui.cpp
	imgui/imgui.cpp
	imgui/imgui_demo.cpp
	imgui/imgui_draw.cpp
	imgui/examples/imgui_impl_dx11.cpp
	imgui/examples/imgui_impl_win32.cpp
	../TestCasesInSuits/BaseToolForCaseAndSuit.h)
	
add_executable(TestGui ${TEST_GUI_SOURCE_FILES})

target_link_libraries(TestGui CommonClass)

# include fundamental headers for "imgui"
target_include_directories(TestGui PRIVATE imgui)
message(STATUS "In order to build imgui, folder 'imgui' has been added to include directories")

# initialization script for lua.
get_target_property(
	CONST_TEST_GUI_OUTPUT_PATH
	TestGui
	RUNTIME_OUTPUT_DIRECTORY
	)
message(STATUS "the output path of " ${FastCommandPrompt} " is " ${CONST_TEST_GUI_OUTPUT_PATH})
message(STATUS " build type is  " ${CMAKE_BUILD_TYPE})
# copy those script to the output path.
file(
	COPY initialize.lua controller.lua 
	DESTINATION ${CONST_TEST_GUI_OUTPUT_PATH}
)
