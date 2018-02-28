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

#pragma region test vector2 == and !=
	TEST_UNIT_START("test vector2 == and !=")
		CommonClass::vector2 cmp1(1.2f, 2.4f);
		CommonClass::vector2 cmp2(1.2f + 1e-9f, 2.4f);

		// the cmp2.m_x is a little bit bigger than cmp1.m_x,
		// the operator should ignore the difference,
		// and judge them as the equal vector.
		errorLogger.LogIfNotEq(cmp1, cmp2);
	TEST_UNIT_END;
#pragma endregion

#pragma region test vector2 +/ -/ *
	TEST_UNIT_START("test vector2 +/ -/ *")
		const Types::F32 comf1(1.2f), comf2(2.4f), comf3(3.5f), comf4(5.878f);
		CommonClass::vector2 cmp1(comf1, comf2);
		CommonClass::vector2 cmp2(comf3, comf4);

		errorLogger.LogIfNotEq(
			cmp1 + cmp2, 
			CommonClass::vector2(comf1 + comf3, comf2 + comf4));

		errorLogger.LogIfNotEq(
			cmp1 - cmp2, 
			CommonClass::vector2(comf1 - comf3, comf2 - comf4));

		errorLogger.LogIfNotEq(
			cmp1 * cmp2,
			(comf1 * comf3 + comf2 * comf4) );
	TEST_UNIT_END;
#pragma endregion

#pragma region test vector3 == and !=
	TEST_UNIT_START("test vector3 == and !=")
		CommonClass::vector3 cmp1(1.2f, 2.4f, 45.121f);
		CommonClass::vector3 cmp2(1.2f + 1e-9f, 2.4f, 45.121f + 1e-10f);

		// the cmp2.m_x is a little bit bigger than cmp1.m_x,
		// the operator should ignore the difference,
		// and judge them as the equal vector.
		errorLogger.LogIfNotEq(cmp1, cmp2);
	TEST_UNIT_END;
#pragma endregion

#pragma region test vector2 +/ -/ *
	TEST_UNIT_START("test vector2 +/ -/ *")
		const Types::F32 comf1(1.2f), comf2(2.4f), comf3(3.5f), comf4(5.878f), comf5(745.15), comf6(-45.4454);
		CommonClass::vector3 cmp1(comf1, comf2, comf3);
		CommonClass::vector3 cmp2(comf4, comf5, comf6);

		errorLogger.LogIfNotEq(
			cmp1 + cmp2,
			CommonClass::vector3(comf1 + comf3, comf2 + comf4, comf));

		errorLogger.LogIfNotEq(
			cmp1 - cmp2,
			CommonClass::vector2(comf1 - comf3, comf2 - comf4));

		errorLogger.LogIfNotEq(
			cmp1 * cmp2,
			(comf1 * comf3 + comf2 * comf4));
	TEST_UNIT_END;
#pragma endregion

}// AddTestUnit()

}// namespace TestUnit

int main()
{
	TestUnit::testMain();
	return 0;
}
