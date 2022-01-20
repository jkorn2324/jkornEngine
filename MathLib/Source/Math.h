#pragma once

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

	static float Abs(float input)
	{
		return static_cast<float>(std::abs(input));
	}

	static bool IsCloseEnough(float a, float b)
	{
		return Abs((a - b)) < EPSILON;
	}

	static float Sin(float input, bool inDegrees = true)
	{
		float theta = inDegrees ? DEG2RAD * input : input;
		return std::sinf(theta);
	}

	static float Cos(float input, bool inDegrees = true)
	{
		float theta = inDegrees ? DEG2RAD * input : input;
		return std::cosf(theta);
	}

	static float Tan(float input, bool inDegrees = true)
	{
		float theta = inDegrees ? DEG2RAD * input : input;
		return std::tanf(theta);
	}

	static float ACos(float input, bool inDegrees = true)
	{
		float theta = inDegrees ? DEG2RAD * input : input;
		return std::acosf(theta);
	}

	static float ATan2(float y, float x)
	{
		return std::atan2f(y, x);
	}

	static float ASin(float input, bool inDegrees = true)
	{
		float theta = inDegrees ? DEG2RAD * input : input;
		return std::asinf(theta);
	}

	template<typename T>
	static T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}

	template<typename T>
	static T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template<typename T>
	static T Clamp(const T& min, const T& max, const T& val)
	{
		return Min(max, Max(min, val));
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