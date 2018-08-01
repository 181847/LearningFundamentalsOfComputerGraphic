#include <MyTools/TestTool/Suit.h>  // include test framework
#include "CommonHeaders.h"
// include required suits here
#include "TestBasicTypesSuit.h"
#include "CaseAndSuitForCommonClasses.h"
#include "CaseAndSuitForRayRender.h"

#pragma comment(lib, "MyTools/MyTool.lib")
#pragma comment(lib, "CommonClasses.lib")

int main()
{
    BasicTypesTestSuit_Runable bttsRunable;
    //bttsRunable.Start();

    SuitForCommonClasses suitForCommonClasses;
    //suitForCommonClasses.Start();

    SuitForRayRender suitForRayRender;
    suitForRayRender.Start();

    getchar();
    return 0;
}

