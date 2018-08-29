cmake_minimum_required(VERSION 3.7.1)

project(LearningFundamentalsOfComputerGraphic)
if(MSVC)
    add_definitions(-DUNICODE -D_UNICODE)
endif(MSVC)

set(MY_LIBRARY_PATH "../Library" CACHE PATH "the library for entire project, which provides a large range of supported.")

find_library(MY_TOOLS_STATIC_LIB 
	NAMES "MyTool" 
	PATHS "${MY_LIBRARY_PATH}/MyTools/lib/Debug" "${MY_LIBRARY_PATH}/MyTools/lib")
include_directories(${MY_LIBRARY_PATH})

# set the CommonClass directory
add_subdirectory(CommonClasses)
add_subdirectory(TestCasesInSuits)
