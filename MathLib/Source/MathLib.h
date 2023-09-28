#pragma once

#include "Defines.h"

#include <cmath>

namespace MathLib
{

	const float EPSILON = 0.001f;
	const float PI = 3.14159265358979323846f;
	const float RAD2DEG = 180.0f / PI;
	const float DEG2RAD = PI / 180.0f;
	const float PIOVER2 = PI / 2.0f;

	static float Sqrt(float input)
	{
		return std::sqrtf(input);
	}

	static double Sqrt(double input)
	{
		return std::sqrt(input);
	}

	template<typename T>
	static T Sqrt(T input)
	{
		using InputType = T;
		ASSERT_IS_ARITHMETIC(InputType);
		return input;
	}

	template<>
	static float Sqrt(float input)
	{
		return Sqrt(input);
	}

	template<>
	static double Sqrt(double input)
	{
		return Sqrt(input);
	}

	template<typename T>
	static T Abs(T input)
	{
		using InputType = T;
		ASSERT_IS_ARITHMETIC(InputType);
		const InputType negativeOne = static_cast<InputType>(-1);
		const InputType zero = static_cast<InputType>(0);
		return input < zero ? input * negativeOne : input;
	}

	template<>
	float Abs(float input)
	{
		return std::fabsf(input);
	}

	template<>
	double Abs(double input)
	{
		return std::abs(input);
	}

	template<typename T>
	static bool IsCloseEnough(T a, T b, T epsilon)
	{
		return a == b;
	}

	template<>
	static bool IsCloseEnough(float a, float b, float epsilon)
	{
		return MathLib::Abs(a - b) <= epsilon;
	}

	static bool IsCloseEnough(float a, float b)
	{
		return IsCloseEnough(a, b, EPSILON);
	}


	static float Sin(float input, bool inDegrees)
	{
		float theta = inDegrees ? DEG2RAD * input : input;
		return std::sinf(theta);
	}

	static float Cos(float input, bool inDegrees)
	{
		float theta = inDegrees ? DEG2RAD * input : input;
		return std::cosf(theta);
	}

	static float Tan(float input, bool inDegrees)
	{
		float theta = inDegrees ? DEG2RAD * input : input;
		return std::tanf(theta);
	}

	static float ACos(float input, bool inDegrees)
	{
		float theta = inDegrees ? DEG2RAD * input : input;
		return std::acosf(theta);
	}

	static float ATan2(float y, float x)
	{
		return std::atan2f(y, x);
	}

	static float ASin(float input, bool inDegrees)
	{
		float theta = inDegrees ? DEG2RAD * input : input;
		return std::asinf(theta);
	}

	template<typename T>
	static T Max(const T& a, const T& b)
	{
		ASSERT_IS_ARITHMETIC(T);
		return a > b ? a : b;
	}

	template<typename T>
	static T Min(const T& a, const T& b)
	{
		ASSERT_IS_ARITHMETIC(T);
		return a < b ? a : b;
	}

	template<typename T>
	static T Clamp(const T& min, const T& max, const T& val)
	{
		ASSERT_IS_ARITHMETIC(T);
		return Min(max, Max(min, val));
	}

	static float Clamp01(float val)
	{
		return Clamp(0.0f, 1.0f, val);
	}

	static float LerpUnclamped(float a, float b, float alpha)
	{
		return a + (b - a) * alpha;
	}

	static float Lerp(float a, float b, float alpha)
	{
		return LerpUnclamped(a, b, Clamp(0.0f, 1.0f, alpha));
	}
}
