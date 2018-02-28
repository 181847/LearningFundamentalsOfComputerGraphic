// For Unit Test
#include <MyTools\UnitTestModules.h>
#include <iostream>

// include test target code
#include "../CommonClasses/vector2.h"
#pragma comment(lib, "CommonClasses.lib")


DECLARE_TEST_UNITS;

namespace TestUnit
{
void GetReady() {}
void AfterTest() {}

void AddTestUnit()
{
#pragma region Try the testUnit
	TEST_UNIT_START("a test always success")
		return true;
	TEST_UNIT_END;
#pragma endregion

#pragma region test vector2
	TEST_UNIT_START("test vector2")
		CommonClass::vector2 v2;
		errorLogger.LogIfNotEq(v2.m_x, 0.0f);
		errorLogger.LogIfNotEq(v2.m_y, 0.0f);
	TEST_UNIT_END;
#pragma endregion

}// AddTestUnit()

}// namespace TestUnit

int main()
{
	TestUnit::testMain();
	return 0;
}
