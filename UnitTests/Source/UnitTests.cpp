// UnitTests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "MathUnitTests.h"
#include "DebugAssert.h"
#include "StringUtils.h"

bool RunWideStringUnitTests();

int main()
{
	DebugAssert(RunWideStringUnitTests() == true,
		"Wide String Unit Tests.");

	DebugAssert(RunVector2UnitTests() == true, 
		"Vector2 UnitTest Failed.");
	DebugAssert(RunVector3UnitTests() == true, 
		"Vector3 UnitTest Failed.");
	DebugAssert(RunVector4UnitTests() == true, 
		"Vector4 UnitTest Failed.");
	DebugAssert(RunMatrix3UnitTests() == true,
		"Matrix3x3 UnitTest Failed.");
	DebugAssert(RunMatrix4UnitTests() == true,
		"Matrix4x4 UnitTest Failed.");

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