cmake_minimum_required(VERSION 3.7.1)

project(LearningFundamentalsOfComputerGraphic)
if(MSVC)
    add_definitions(-DUNICODE -D_UNICODE)
endif(MSVC)

include_directories(${MY_LIBRARY_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set the CommonClass directory
add_subdirectory(CommonClasses)
add_subdirectory(TestCasesInSuits)
add_subdirectory(TestGui)
