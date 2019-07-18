#include "../CommonClasses/Utils/TestTool/Suit.h"
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

int main()
{
    TestSuit::SuitQueueRunner<
        //BasicTypesTestSuit_Runable, 
        //SuitForCommonClasses,
        SuitForRayRender//,
        //SuitForRasterisation, 
        //SuitForRasterizeTriangle,
        //SuitForFilter,
        //SuitForStbImage
    > suitQueueRunner;

    suitQueueRunner.StartBatch();

    getchar();
    return 0;
}

