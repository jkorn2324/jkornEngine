#include "MathUnitTests.h"
#include "Source\Math.h"
#include "Source\Vector.h"
#include "Source\Matrix.h"

using namespace MathLib;

bool RunVector2UnitTests()
{
	bool isWorking = true;

	Vector2 testVector(-0.5f, 1.0f);

	// Todo: Create simd vector unit tests

	// Equivalent
	{
		Vector2 a(-0.5f, 1.0f);
		isWorking &= testVector == a;
		a.x = 0.5f;
		isWorking &= testVector != a;
		a.x = -0.49999f;
		isWorking &= testVector == a;
	}

	// Addition.
	{
		Vector2 addVector = testVector + Vector2(-1.0f, 1.0f);
		isWorking &= addVector == Vector2(-1.5f, 2.0f);
		addVector = testVector;
		addVector += Vector2(1.0f, 2.0f);
		isWorking &= addVector == Vector2(0.5f, 3.0f);
	}

	// Subtraction.
	{
		Vector2 subtractVector = testVector - Vector2(0.5f, 1.0f);
		isWorking &= subtractVector == Vector2(-1.0f, 0.0f);
		subtractVector = testVector;
		subtractVector -= Vector2(-0.5f, -1.0f);
		isWorking &= subtractVector == Vector2(0.0f, 2.0f);
	}

	// Multiplication
	{
		// Vector Multiplication.
		Vector2 multiplicationVector = testVector * Vector2(0.0f, 0.0f);
		isWorking &= multiplicationVector == Vector2::Zero;
		multiplicationVector = testVector;
		multiplicationVector *= Vector2::Zero;
		isWorking &= multiplicationVector == Vector2::Zero;

		// Scalar Multiplication.
		multiplicationVector = testVector * 0.0f;
		isWorking &= multiplicationVector == Vector2::Zero;
		multiplicationVector = testVector;
		multiplicationVector *= 0.0f;
		isWorking &= multiplicationVector == Vector2::Zero;
	}

	// Division
	{
		// Vector Division.
		Vector2 divisionVector = testVector / Vector2::One;
		isWorking &= divisionVector == testVector;
		divisionVector /= Vector2::One;
		isWorking &= divisionVector == testVector;

		// Scalar Division.
		divisionVector = testVector / 1.0f;
		isWorking &= divisionVector == testVector;
		divisionVector /= 1.0f;
		isWorking &= divisionVector == testVector;
	}

	// Normalized
	{
		isWorking &= !testVector.IsNormalized();
		Vector2 normalized = Normalize(testVector);
		isWorking &= normalized.IsNormalized();
	}

	// Dot Product
	{
		Vector2 comparableDotProduct = testVector;
		// Front
		isWorking &= Dot(comparableDotProduct, testVector) >= 1.0f;
		comparableDotProduct *= -1.0f;
		// Back
		isWorking &= Dot(comparableDotProduct, testVector) <= -1.0f;

		// Check the side.
		comparableDotProduct = Vector2(-1.0f, 0.0f);
		Vector2 b = Vector2(0.0f, 1.0f);
		isWorking &= IsCloseEnough(Dot(comparableDotProduct, b), 0.0f);
	}

	// Linear interpolation.
	{
		Vector2 b = Vector2::Zero;
		Vector2 outputVec = Lerp(testVector, b, 0.0f);
		isWorking &= outputVec == testVector;
		outputVec = Lerp(testVector, b, 1.0f);
		isWorking &= outputVec == b;
	}
	return isWorking;
}

bool RunVector3UnitTests()
{
	return true;
}

bool RunVector4UnitTests()
{
	// TODO: start on vector4 unit tests
	return true;
}

bool RunMatrix3UnitTests()
{
	bool isWorking = true;

	static float testMat[3][3] =
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		-1.0f, -2.0f, 1.0f
	};
	Matrix3x3 matrix(testMat);

	// Matrix multiplication
	{
		Matrix3x3 multipliedMatrix = matrix * Matrix3x3::Identity;
		isWorking &= multipliedMatrix == matrix;
	}

	// Transpose Matrix
	{
		float transposedMat[3][3] =
		{
			1.0f, 0.0f, -1.0f,
			0.0f, 1.0f, -2.0f,
			0.0f, 0.0f, 1.0f
		};
		Matrix3x3 transposedMatrix(transposedMat);
		Matrix3x3 transposedCurrent = Transpose(matrix);
		isWorking &= transposedMatrix == transposedCurrent;
		transposedCurrent.Transpose();
		isWorking &= transposedCurrent == matrix;
	}
	// TODO: 
	return isWorking;
}

bool RunMatrix4UnitTests()
{
	bool isWorking = true;

	static float testMat[4][4] =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -2.0f, 1.0f, 1.0f
	};
	Matrix4x4 matrix(testMat);

	// Matrix multiplication
	{
		Matrix4x4 multipliedMatrix = matrix * Matrix4x4::Identity;
		isWorking &= multipliedMatrix == matrix;
	}

	// Transpose Matrix
	{
		float transposedMat[4][4] =
		{
			1.0f, 0.0f, 0.0f, -1.0f,
			0.0f, 1.0f, 0.0f, -2.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
		Matrix4x4 transposedMatrix(transposedMat);
		Matrix4x4 transposedCurrent = Transpose(matrix);
		isWorking &= transposedMatrix == transposedCurrent;
		transposedCurrent.Transpose();
		isWorking &= transposedCurrent == matrix;
	}
	// TODO: 
	return isWorking;
}
