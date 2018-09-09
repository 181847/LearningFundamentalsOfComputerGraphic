cmake_minimum_required(VERSION 3.7.1)

project(LearningFundamentalsOfComputerGraphic)
if(MSVC)
    add_definitions(-DUNICODE -D_UNICODE)
endif(MSVC)

set(MY_LIBRARY_PATH "../Library" CACHE PATH "the library for entire project, which provides a large range of supported.")

# if is not absolute path, make it absolute.
if (NOT IS_ABSOLUTE MY_LIBRARY_PATH)
	message(STATUS "convert to abosolute path" ${MY_LIBRARY_PATH})
	get_filename_component(MY_LIBRARY_PATH "${MY_LIBRARY_PATH}"
						   REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
endif (NOT IS_ABSOLUTE MY_LIBRARY_PATH) 

if (NOT EXISTS ${MY_LIBRARY_PATH})
	message(WARNING "cannot find required tool path, please consider downloading codes from https://github.com/181847/MyLibrary_of_C_Cplusplus, and build by its CMakeList.txt, then make MY_LIBRARY_PATH point to that path")
endif (NOT EXISTS ${MY_LIBRARY_PATH})

find_library(MY_TOOLS_STATIC_LIB 
	NAMES "MyTool" 
	PATHS "${MY_LIBRARY_PATH}/MyTools/lib/Debug" "${MY_LIBRARY_PATH}/MyTools/lib")
	
include_directories(${MY_LIBRARY_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set the CommonClass directory
add_subdirectory(CommonClasses)
add_subdirectory(TestCasesInSuits)
add_subdirectory(TestGui)
