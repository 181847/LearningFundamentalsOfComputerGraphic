# test codes about CommonClass
cmake_minimum_required(VERSION 3.7.1)

set (TEST_CODE_SOURCE_FILES
	CommonHeaders.h
	CaseAndSuitForCommonClasses.h
	CaseAndSuitForFilter.h
	CaseAndSuitForRasterisation.h
	CaseAndSuitForRasterizeTriangle.h
	CaseAndSuitForRayRender.h
	CaseAndSuitForStbImage.h
	BaseToolForCaseAndSuit.h
	CaseAndSuitForCommonClasses.cpp
	CaseAndSuitForFilter.cpp
	CaseAndSuitForRasterisation.cpp
	CaseAndSuitForRasterizeTriangle.cpp
	CaseAndSuitForRayRender.cpp
	CaseAndSuitForStbImage.cpp
	BaseToolForCaseAndSuit.cpp
	TestCasesInSuits.cpp)

add_executable(TestCasesInSuits ${TEST_CODE_SOURCE_FILES})
target_link_libraries(TestCasesInSuits CommonClass)
