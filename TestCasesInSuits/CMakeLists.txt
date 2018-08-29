# test codes about CommonClass
cmake_minimum_required(VERSION 3.7.1)

set (TEST_CODE_SOURCE_FILES 
	CaseAndSuitForCommonClasses.cpp
	CaseAndSuitForFilter.cpp
	CaseAndSuitForRasterisation.cpp
	CaseAndSuitForRasterizeTriangle.cpp
	CaseAndSuitForRayRender.h.cpp
	CaseAndSuitForStbImage.cpp
	TestCasesInSuits.cpp)

add_executable(TestCasesInSuits ${TEST_CODE_SOURCE_FILES})
target_link_libraries(TestCasesInSuits CommonClass)
target_link_libraries(TestCasesInSuits ${MY_TOOLS_STATIC_LIB})
