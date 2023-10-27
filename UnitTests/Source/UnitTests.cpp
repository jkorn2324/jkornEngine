// UnitTests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "MathUnitTests.h"
#include "EngineAssert.h"
#include "StringUtils.h"

bool RunWideStringUnitTests();

int main()
{
    JKORN_ENGINE_ASSERT(RunWideStringUnitTests() == true,
		"Wide String Unit Tests.");

    JKORN_ENGINE_ASSERT(RunVector2UnitTests() == true,
		"Vector2 UnitTest Failed.");
    JKORN_ENGINE_ASSERT(RunVector3UnitTests() == true,
		"Vector3 UnitTest Failed.");
    JKORN_ENGINE_ASSERT(RunVector4UnitTests() == true,
		"Vector4 UnitTest Failed.");
    JKORN_ENGINE_ASSERT(RunMatrix3UnitTests() == true,
		"Matrix3x3 UnitTest Failed.");
    JKORN_ENGINE_ASSERT(RunMatrix4UnitTests() == true,
		"Matrix4x4 UnitTest Failed.");
    JKORN_ENGINE_ASSERT(Run2DIntersectionUnitTests() == true,
		"2D Intersection UnitTests Failed");
    JKORN_ENGINE_ASSERT(Run3DIntersectionUnitTests() == true,
		"2D Intersection UnitTests Failed");

	std::printf("Unit Tests Passed!\n");
	return 0;
}

bool RunWideStringUnitTests()
{
	bool isValid = true;
	{

		std::wstring wstring(L"BigGiantWs");
		std::string str;
		Engine::WStringToString(wstring, str);
		isValid &= str == "BigGiantWs";
	}

	{
		std::string string("BigGiantWs");
		std::wstring str;
		Engine::StringToWString(string, str);
		isValid &= str == L"BigGiantWs";
	}
	return isValid;
}
