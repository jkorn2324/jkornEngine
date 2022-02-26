#include "MathPCH.h"
#include "Vector.h"
#include "Math.h"
#include "Matrix.h"
#include "Quaternion.h"

namespace MathLib
{
#pragma region vector2


	const Vector2 Vector2::UnitX = Vector2(1.0f, 0.0f);

	const Vector2 Vector2::UnitY = Vector2(0.0f, 1.0f);

	const Vector2 Vector2::Zero = Vector2(0.0f, 0.0f);

	const Vector2 Vector2::One = Vector2(1.0f, 1.0f);


	Vector2::Vector2()
		: x(0.0f), y(0.0f) { }

	Vector2::Vector2(const Vector2& vec)
		: x(vec.x), y(vec.y) { }

	Vector2::Vector2(const Vector3& vec)
		: x(vec.x), y(vec.y) { }

	Vector2::Vector2(const Vector4& vec)
		: x(vec.x), y(vec.y)
	{
	}

	Vector2::Vector2(float x, float y)
		: x(x), y(y) { }

	Vector2& Vector2::operator=(const Vector3& vec)
	{
		x = vec.x;
		y = vec.y;
		return *this;
	}

	Vector2& Vector2::operator=(const Vector4& vec)
	{
		x = vec.x;
		y = vec.y;
		return *this;
	}

	float Vector2::Length() const
	{
		return Sqrt(LengthSquared());
	}

	float Vector2::LengthSquared() const
	{
		return x * x + y * y;
	}

	void Vector2::Normalize()
	{
		float length = Length();
		x /= length;
		y /= length;
	}

	bool Vector2::IsNormalized() const
	{
		return IsCloseEnough(LengthSquared(), 1.0f);
	}

	bool IsNormalized(const Vector2& vec)
	{
		return IsCloseEnough(vec.LengthSquared(), 1.0f);
	}

	Vector2& Vector2::operator+=(const Vector2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	Vector2& Vector2::operator-=(const Vector2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	Vector2& Vector2::operator*=(const Vector2& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	Vector2& Vector2::operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	Vector2& Vector2::operator/=(const Vector2& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	Vector2& Vector2::operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	Vector2 Reflect(const Vector2& vec, const Vector2& normal)
	{
		return vec - 2.0f * Dot(vec, normal) * normal;
	}

	Vector2 Lerp(const Vector2& a, const Vector2& b, float alpha)
	{
		return a + (b - a) * alpha;
	}

	Vector2 LerpClamped(const Vector2& a, const Vector2& b, float alpha)
	{
		alpha = Clamp(0.0f, 1.0f, alpha);
		return Lerp(a, b, alpha);
	}

	Vector2 Min(const Vector2& a, const Vector2& b)
	{
		return Vector2(
			Min(a.x, b.x), Min(a.y, b.y));
	}

	Vector2 Max(const Vector2& a, const Vector2& b)
	{
		return Vector2(
			Max(a.x, b.x), Max(a.y, b.y));
	}

	Vector2 Normalize(const Vector2& vec)
	{
		Vector2 cpy(vec);
		cpy.Normalize();
		return cpy;
	}

	float Dot(const Vector2& a, const Vector2& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	float Cross(const Vector2& a, const Vector2& b)
	{
		return a.x * b.y - a.y * b.x;
	}

	bool operator==(const Vector2& a, const Vector2& b)
	{
		return IsCloseEnough(a.x, b.x)
			&& IsCloseEnough(a.y, b.y);
	}

	bool operator!=(const Vector2& a, const Vector2& b)
	{
		return !IsCloseEnough(a.x, b.x)
			|| !IsCloseEnough(a.y, b.y);
	}

	Vector2 operator+(const Vector2& a, const Vector2& b)
	{
		return Vector2(
			a.x + b.x, a.y + b.y);
	}

	Vector2 operator-(const Vector2& a, const Vector2& b)
	{
		return Vector2(
			a.x - b.x, a.y - b.y);
	}

	Vector2 operator-(const Vector2& vec)
	{
		return Vector2(-vec.x, -vec.y);
	}

	Vector2 operator*(const Vector2& a, const Vector2& b)
	{
		return Vector2(
			a.x * b.x, a.y * b.y);
	}

	Vector2 operator*(const Vector2& v, float scalar)
	{
		return Vector2(
			v.x * scalar, v.y * scalar);
	}

	Vector2 operator*(float scalar, const Vector2& v)
	{
		return Vector2(
			v.x * scalar, v.y * scalar);
	}

	Vector2 operator/(const Vector2& a, const Vector2& b)
	{
		return Vector2(
			a.x / b.x, a.y / b.y);
	}

	Vector2 operator/(const Vector2& v, float scalar)
	{
		return Vector2(
			v.x / scalar, v.y / scalar);
	}

#pragma endregion

#pragma region vector3


	const Vector3 Vector3::UnitX = Vector3(1.0f, 0.0f, 0.0f);

	const Vector3 Vector3::UnitY = Vector3(0.0f, 1.0f, 0.0f);

	const Vector3 Vector3::UnitZ = Vector3(0.0f, 0.0f, 1.0f);

	const Vector3 Vector3::One = Vector3(1.0f, 1.0f, 1.0f);
	
	const Vector3 Vector3::Zero = Vector3(0.0f, 0.0f, 0.0f);

	Vector3::Vector3()
		: x(0.0f), y(0.0f), z(0.0f) { }


	Vector3::Vector3(float x, float y, float z)
		: x(x), y(y), z(z) { }

	Vector3::Vector3(const Vector2& vec, float z)
		: x(vec.x), y(vec.y), z(z) { }

	Vector3::Vector3(const Vector3& vec)
		: x(vec.x), y(vec.y), z(vec.z) { }

	Vector3::Vector3(const Vector4& vec)
		: x(vec.x), y(vec.y), z(vec.z) { }

	Vector3& Vector3::operator=(const Vector2& vec)
	{
		x = vec.x;
		y = vec.y;
		z = 0.0f;
		return *this;
	}

	Vector3& Vector3::operator=(const Vector4& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}

	float Vector3::Length() const
	{
		return Sqrt(LengthSquared());
	}

	float Vector3::LengthSquared() const
	{
		return x * x + y * y + z * z;
	}

	void Vector3::Normalize()
	{
		float length = Length();
		x /= length;
		y /= length;
		z /= length;
	}

	float Dot(const Vector3& a, const Vector3& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	Vector3 Cross(const Vector3& a, const Vector3& b)
	{
		return Vector3(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x);
	}

	Vector3 Reflect(const Vector3& vec, const Vector3& normal)
	{
		return vec - 2.0f * Dot(vec, normal) * normal;
	}

	Vector3 Lerp(const Vector3& a, const Vector3& b, float alpha)
	{
		return a + (b - a) * alpha;
	}

	Vector3 LerpClamped(const Vector3& a, const Vector3& b, float alpha)
	{
		alpha = Clamp(0.0f, 1.0f, alpha);
		return Lerp(a, b, alpha);
	}

	Vector3 Rotate(const Quaternion& quat, const Vector3& direction)
	{
		MathLib::Vector3 normalizedCopy(direction);
		normalizedCopy.Normalize();

		Quaternion negQuat(-quat);
		Quaternion outQuat(direction.x, direction.y, direction.z, 0.0f);
		outQuat = Concatenate(quat, outQuat);
		outQuat = Concatenate(outQuat, negQuat);

		return Vector3(outQuat.x, outQuat.y, outQuat.z);
	}

	Vector3 Min(const Vector3& a, const Vector3& b)
	{
		return Vector3(Min(a.x, b.x),
			Min(a.y, b.y), Min(a.z, b.z));
	}

	Vector3 Max(const Vector3& a, const Vector3& b)
	{
		return Vector3(Max(a.x, b.x),
			Max(a.y, b.y), Max(a.z, b.z));
	}

	Vector3 Normalize(const Vector3& in)
	{
		Vector3 out(in);
		out.Normalize();
		return out;
	}

	bool Vector3::IsNormalized() const
	{
		return IsCloseEnough(LengthSquared(), 1.0f);
	}

	bool IsNormalized(const Vector3& vec)
	{
		return IsCloseEnough(vec.LengthSquared(), 1.0f);
	}

	bool operator==(const Vector3& a, const Vector3& b)
	{
		return IsCloseEnough(a.x, b.x)
			&& IsCloseEnough(a.y, b.y)
			&& IsCloseEnough(a.z, b.z);
	}

	bool operator!=(const Vector3& a, const Vector3& b)
	{
		return !IsCloseEnough(a.x, b.x)
			|| !IsCloseEnough(a.y, b.y)
			|| !IsCloseEnough(a.z, b.z);
	}

	Vector3 operator+(const Vector3& a, const Vector3& b)
	{
		return Vector3(
			a.x + b.x, a.y + b.y, a.z + b.z);
	}

	Vector3 operator+(const Vector3& a, const Vector2& b)
	{
		return Vector3(a.x + b.x, a.y + b.y, a.z);
	}

	Vector3 operator-(const Vector3& vec)
	{
		return Vector3(-vec.x, -vec.y, -vec.z);
	}

	Vector3 operator-(const Vector3& a, const Vector3& b)
	{
		return Vector3(
			a.x - b.x, a.y - b.y, a.z - b.z);
	}

	Vector3 operator-(const Vector3& a, const Vector2& b)
	{
		return Vector3(a.x - b.x, a.y - b.y, a.z);
	}

	Vector3 operator*(const Vector3& a, const Vector2& b)
	{
		return Vector3(
			a.x * b.x, a.y * b.y, a.z * 0.0f);
	}

	Vector3 operator*(const Vector3& a, const Vector3& b)
	{
		return Vector3(
			a.x * b.x, a.y * b.y, a.z * b.z);
	}

	Vector3 operator*(const Vector3& a, float scalar)
	{
		return Vector3(
			a.x * scalar, a.y * scalar, a.z * scalar);
	}

	Vector3 operator*(float scalar, const Vector3& vec)
	{
		return Vector3(
			vec.x * scalar, vec.y * scalar, vec.z * scalar);
	}

	Vector3 operator/(const Vector3& a, const Vector3& b)
	{
		return Vector3(
			a.x / b.x, a.y / b.y, a.z / b.z);
	}

	Vector3 operator/(const Vector3& a, float scalar)
	{
		return Vector3(
			a.x / scalar, a.y / scalar, a.z / scalar);
	}

	Vector3 operator*(const Vector3& vec, const Matrix3x3& mat)
	{
		return Vector3(
			vec.x * mat[0][0] + vec.y * mat[0][1] + vec.z * mat[0][2],
			vec.x * mat[1][0] + vec.y * mat[1][1] + vec.z * mat[1][2],
			vec.x * mat[2][0] + vec.y * mat[2][1] + vec.z * mat[2][2]);
	}

	Vector3 operator*(const Matrix3x3& mat, const Vector3& vec)
	{
		return vec * mat;
	}

	Vector3& Vector3::operator+=(const Vector3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vector3& Vector3::operator+=(const Vector2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	Vector3& Vector3::operator-=(const Vector3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vector3& Vector3::operator-=(const Vector2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	Vector3& Vector3::operator*=(const Vector3& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	Vector3& Vector3::operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	Vector3& Vector3::operator/=(const Vector3& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	Vector3& Vector3::operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

#pragma endregion

#pragma region vector4

	const Vector4 Vector4::UnitX = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
	const Vector4 Vector4::UnitY = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	const Vector4 Vector4::UnitZ = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
	const Vector4 Vector4::UnitW = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	const Vector4 Vector4::One = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	const Vector4 Vector4::Zero = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

	Vector4::Vector4()
		: x(0.0f), y(0.0f), z(0.0f), w(0.0f) { }


	Vector4::Vector4(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w) { }

	
	Vector4::Vector4(const Vector2& vec, float z, float w)
		: x(vec.x), y(vec.y), z(z), w(w) { }

	
	Vector4::Vector4(const Vector3& vec, float w)
		: x(vec.x), y(vec.y), z(vec.z), w(w) { }

	
	Vector4::Vector4(const Vector4& vec)
		: x(vec.x), y(vec.y), z(vec.z), w(vec.w) { }


	Vector4& Vector4::operator=(const Vector3& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = 0.0f;
		return *this;
	}

	Vector4& Vector4::operator=(const Vector2& vec)
	{
		x = vec.x;
		y = vec.y;
		z = 0.0f;
		w = 0.0f;
		return *this;
	}

	float Vector4::Length() const
	{
		return Sqrt(LengthSquared());
	}

	float Vector4::LengthSquared() const
	{
		return x * x + y * y + z * z + w * w;
	}

	void Vector4::Normalize()
	{
		float length = Length();
		x /= length;
		y /= length;
		z /= length;
		w /= length;
	}

	bool Vector4::IsNormalized() const
	{
		return IsCloseEnough(LengthSquared(), 1.0f);
	}

	bool IsNormalized(const Vector4& vec)
	{
		return IsCloseEnough(vec.LengthSquared(), 1.0f);
	}

	float Dot(const Vector4& a, const Vector4& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	Vector4 Reflect(const Vector4& vec, const Vector4& normal)
	{
		return vec - 2.0f * Dot(vec, normal) * normal;
	}

	Vector4 Lerp(const Vector4& a, const Vector4& b, float alpha)
	{
		return a + (b - a) * alpha;
	}

	Vector4 LerpClamped(const Vector4& a, const Vector4& b, float alpha)
	{
		alpha = Clamp(0.0f, 1.0f, alpha);
		return Lerp(a, b, alpha);
	}

	Vector4 Min(const Vector4& a, const Vector4& b)
	{
		return Vector4(
			Min(a.x, b.x), Min(a.y, b.y), Min(a.z, b.z), Min(a.w, b.w));
	}

	Vector4 Max(const Vector4& a, const Vector4& b)
	{
		return Vector4(
			Max(a.x, b.x), Max(a.y, b.y), Max(a.z, b.z), Max(a.w, b.w));
	}

	Vector4 Normalize(const Vector4& in)
	{
		Vector4 out(in);
		out.Normalize();
		return out;
	}

	bool operator==(const Vector4& a, const Vector4& b)
	{
		return IsCloseEnough(a.x, b.x)
			&& IsCloseEnough(a.y, b.y)
			&& IsCloseEnough(a.z, b.z)
			&& IsCloseEnough(a.w, b.w);
	}

	bool operator!=(const Vector4& a, const Vector4& b)
	{
		return !IsCloseEnough(a.x, b.x)
			|| !IsCloseEnough(a.y, b.y)
			|| !IsCloseEnough(a.z, b.z)
			|| !IsCloseEnough(a.w, b.w);
	}

	Vector4 operator+(const Vector4& a, const Vector4& b)
	{
		return Vector4(
			a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
	}

	Vector4 operator-(const Vector4& a, const Vector4& b)
	{
		return Vector4(
			a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
	}

	Vector4 operator-(const Vector4& vec)
	{
		return Vector4(-vec.x, -vec.y, -vec.z, -vec.w);
	}

	Vector4 operator*(const Vector4& a, const Vector4& b)
	{
		return Vector4(
			a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
	}

	Vector4 operator*(const Vector4& a, float scalar)
	{
		return Vector4(
			a.x * scalar, a.y * scalar, a.z * scalar, a.w * scalar);
	}

	Vector4 operator*(float scalar, const Vector4& a)
	{
		return Vector4(
			a.x * scalar, a.y * scalar, a.z * scalar, a.w * scalar);
	}

	Vector4 operator/(const Vector4& a, const Vector4& b)
	{
		return Vector4(
			a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
	}

	Vector4 operator/(const Vector4& a, float scalar)
	{
		return Vector4(
			a.x / scalar, a.y / scalar, a.z / scalar, a.w / scalar);
	}

	Vector4 operator*(const Vector4& a, const Matrix4x4& b)
	{
		return Vector4(
			a.x * b[0][0] + a.y * b[0][1] + a.z * b[0][2] + a.w * b[0][3],
			a.y * b[1][0] + a.y * b[1][1] + a.z * b[1][2] + a.w * b[1][3],
			a.z * b[2][0] + a.y * b[2][1] + a.z * b[2][2] + a.w * b[2][3],
			a.w * b[3][0] + a.y * b[3][1] + a.z * b[3][2] + a.w * b[3][3]);
	}

	Vector4 operator*(const Matrix4x4& a, const Vector4& b)
	{
		return b * a;
	}

	Vector4& Vector4::operator+=(const Vector4& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	Vector4& Vector4::operator-=(const Vector4& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	Vector4& Vector4::operator*=(const Vector4& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}

	Vector4& Vector4::operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}

	Vector4& Vector4::operator/=(const Vector4& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
		return *this;
	}

	Vector4& Vector4::operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
		return *this;
	}

#pragma endregion

#pragma endregion
	
	Vector2Int::Vector2Int()
		: x(0), y(0) { }
	
	Vector2Int::Vector2Int(int32_t x, int32_t y)
		: x(x), y(y) { }

	Vector2Int::Vector2Int(const Vector2& vec)
		: x((int32_t)(vec.x)), y((int32_t)(vec.y)) { }
	
	
	Vector3Int::Vector3Int()
		: x(0), y(0), z(0) { }

	Vector3Int::Vector3Int(int32_t x, int32_t y, int32_t z)
		: x(x), y(y), z(z) { }

	Vector3Int::Vector3Int(const Vector3& vec)
		: x((int32_t)vec.x), y((int32_t)vec.y), z((int32_t)vec.z) { }
}