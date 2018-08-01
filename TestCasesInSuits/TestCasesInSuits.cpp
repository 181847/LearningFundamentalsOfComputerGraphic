#include <MyTools/TestTool/Suit.h>  // include test framework
#include "CommonHeaders.h"
// include required suits here
#include "TestBasicTypesSuit.h"
#include "CaseAndSuitForCommonClasses.h"
#include "CaseAndSuitForRayRender.h"
#include "CaseAndSuitForRasterisation.h"
#include "CaseAndSuitForRasterizeTriangle.h"

#pragma comment(lib, "MyTools/MyTool.lib")
#pragma comment(lib, "CommonClasses.lib")


int main()
{
    SuitQueueRunner<
        BasicTypesTestSuit_Runable, 
        SuitForCommonClasses,
        SuitForRayRender,
        SuitForRasterisation, 
        SuitForRasterizeTriangle
    > suitQueueRunner;

    suitQueueRunner.StartBatch();

    getchar();
    return 0;
}

