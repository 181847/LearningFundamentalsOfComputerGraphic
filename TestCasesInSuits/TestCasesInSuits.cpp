#include <MyTools/TestTool/Suit.h>  // include test framework
#include "CommonHeaders.h"
// include required suits here
#include "TestBasicTypesSuit.h"
#include "CaseAndSuitForCommonClasses.h"
#include "CaseAndSuitForRayRender.h"
#include "CaseAndSuitForRasterisation.h"
#include "CaseAndSuitForRasterizeTriangle.h"
#include "CaseAndSuitForDepthBuffer.h"
#include "CaseAndSuitForFilter.h"
#include "CaseAndSuitForStbImage.h"

#ifdef _DEBUG // static library for debug
#pragma comment(lib, "MyTools/lib/Debug/MyTool.lib")
#endif

#pragma comment(lib, "CommonClasses.lib")

int main()
{
    TestSuit::SuitQueueRunner<
        BasicTypesTestSuit_Runable, 
        SuitForCommonClasses,
        SuitForRayRender,
        SuitForRasterisation, 
        SuitForRasterizeTriangle,
        SuitForFilter,
        SuitForStbImage
    > suitQueueRunner;

    suitQueueRunner.StartBatch();

    getchar();
    return 0;
}

