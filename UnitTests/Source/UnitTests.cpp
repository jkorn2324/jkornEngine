// UnitTests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "MathUnitTests.h"
#include "Source\DebugAssert.h"
#include "Source\Matrix.h"

int main()
{
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