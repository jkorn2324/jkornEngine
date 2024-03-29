#pragma once

#include <cstdint>

namespace MathLib
{

	// TODO: Add SIMD/intrinsic support (Reason why we don't use SIMD as its different based on platform
	// ARM, x64, etc...)

    class Quaternion;

	class Vector2
	{
	public:
		float x;
		float y;

		Vector2();
		Vector2(float x, float y);
		
		Vector2(const Vector2& vec);
		Vector2(const class Vector3& vec);
		Vector2(const class Vector4& vec);

		Vector2& operator=(const class Vector3& vec);
		Vector2& operator=(const class Vector4& vec);

		float Length() const;
		float LengthSquared() const;
		void Normalize();
		bool IsNormalized() const;

		friend Vector2 Normalize(const Vector2& input);
		friend bool IsNormalized(const Vector2& vec);

		friend float Dot(const Vector2& a, const Vector2& b);
		friend float Cross(const Vector2& a, const Vector2& b);
		friend Vector2 Reflect(const Vector2& vec, const Vector2& normal);

		friend Vector2 Lerp(const Vector2& a, const Vector2& b, float alpha);
		friend Vector2 LerpClamped(const Vector2& a, const Vector2& b, float alpha);
		
		friend Vector2 Min(const Vector2& a, const Vector2& b);
		friend Vector2 Max(const Vector2& a, const Vector2& b);

		friend bool operator==(const Vector2& a, const Vector2& b);
		friend bool operator!=(const Vector2& a, const Vector2& b);

		friend Vector2 operator+(const Vector2& a, const Vector2& b);
		friend Vector2 operator-(const Vector2& a, const Vector2& b);
		friend Vector2 operator-(const Vector2& vec);
		friend Vector2 operator*(const Vector2& a, const Vector2& b);
		friend Vector2 operator*(float scalar, const Vector2& v);
		friend Vector2 operator*(const Vector2& v, float scalar);

		friend Vector2 operator/(const Vector2& a, const Vector2& b);
		friend Vector2 operator/(const Vector2& v, float scalar);

		Vector2& operator+=(const Vector2& v);
		Vector2& operator-=(const Vector2& v);
		Vector2& operator*=(const Vector2& v);
		Vector2& operator*=(float scalar);
		Vector2& operator/=(const Vector2& v);
		Vector2& operator/=(float scalar);

		static const Vector2 UnitX;
		static const Vector2 UnitY;
		static const Vector2 Zero;
		static const Vector2 One;
	};

	class Vector2Int
	{
	public:
		int32_t x;
		int32_t y;

		Vector2Int();
		Vector2Int(int32_t x, int32_t y);
		Vector2Int(const Vector2& vec);
	};

	class Vector3
	{
	public:
		float x;
		float y;
		float z;

		Vector3();
		Vector3(float x, float y, float z);
		
		Vector3(const Vector3& vec);
		Vector3(const Vector2& vec, float z = 0.0f);
		Vector3(const class Vector4& vec);

		Vector3& operator=(const Vector2& vec);
		Vector3& operator=(const Vector4& vec);

		float Length() const;
		float LengthSquared() const;
		void Normalize();
		bool IsNormalized() const;

        static float Dot(const Vector3& a, const Vector3& b);
		friend float Dot(const Vector3& a, const Vector3& b);
        
        static Vector3 Cross(const Vector3& a, const Vector3& b);
		friend Vector3 Cross(const Vector3& a, const Vector3& b);
        
		friend Vector3 Reflect(const Vector3& dir, const Vector3& normal);

		friend bool IsNormalized(const Vector3& vec);
		friend Vector3 Normalize(const Vector3& vec);

		friend Vector3 Lerp(const Vector3& a, const Vector3& b, float alpha);
		friend Vector3 LerpClamped(const Vector3& a, const Vector3& b, float alpha);

		friend Vector3 Rotate(const Quaternion& quat, const Vector3& direction);
        static Vector3 Rotate(const Quaternion& quat, const Vector3& direction);

		friend Vector3 Min(const Vector3& a, const Vector3& b);
		friend Vector3 Max(const Vector3& a, const Vector3& b);

		friend bool operator==(const Vector3& a, const Vector3& b);
		friend bool operator!=(const Vector3& a, const Vector3& b);

		friend Vector3 operator+(const Vector3& a, const Vector3& b);
		friend Vector3 operator+(const Vector3& a, const Vector2& b);

		friend Vector3 operator-(const Vector3& vec);
		friend Vector3 operator-(const Vector3& a, const Vector3& b);
		friend Vector3 operator-(const Vector3& a, const Vector2& b);

		friend Vector3 operator*(const Vector3& a, const Vector2& b);

		friend Vector3 operator*(const Vector3& a, const Vector3& b);
		friend Vector3 operator*(const Vector3& a, float scalar);
		friend Vector3 operator*(float scalar, const Vector3& vec);
			
		friend Vector3 operator/(const Vector3& a, const Vector3& b);
		friend Vector3 operator/(const Vector3& a, float scalar);

		friend Vector3 operator*(const Vector3& vec, const class Matrix3x3& mat);
		friend Vector3 operator*(const class Matrix3x3& mat, const Vector3& vec);

		Vector3& operator+=(const Vector3& v);
		Vector3& operator+=(const Vector2& v);

		Vector3& operator-=(const Vector3& v);
		Vector3& operator-=(const Vector2& v);

		Vector3& operator*=(const Vector3& v);
		Vector3& operator*=(float scalar);
		Vector3& operator/=(const Vector3& v);
		Vector3& operator/=(float scalar);

		// Get Right Direction.
		static const Vector3 UnitX;
		// Get Up Direction.
		static const Vector3 UnitY;
		// Get Forward Direction.
		static const Vector3 UnitZ;
		static const Vector3 One;
		static const Vector3 Zero;
	};

	class Vector3Int
	{
	public:
		int32_t x;
		int32_t y;
		int32_t z;

		Vector3Int();
		Vector3Int(int32_t x, int32_t y, int32_t z);
		Vector3Int(const Vector3& vec);
	};


	class Vector4
	{
	public:
		float x;
		float y;
		float z;
		float w;

		Vector4();
		Vector4(float x, float y, float z, float w);
		
		Vector4(const Vector4& vec);
		Vector4(const Vector3& vec, float w = 0.0f);
		Vector4(const Vector2& vec, float z = 0.0f, float w = 0.0f);

		Vector4& operator=(const Vector3& vec);
		Vector4& operator=(const Vector2& vec);

		float Length() const;
		float LengthSquared() const;
		void Normalize();
		bool IsNormalized() const;

		friend float Dot(const Vector4& a, const Vector4& b);
		friend Vector4 Reflect(const Vector4& vec, const Vector4& normal);
		friend Vector4 Lerp(const Vector4& a, const Vector4& b, float alpha);
		friend Vector4 LerpClamped(const Vector4& a, const Vector4& b, float alpha);

		friend Vector4 Normalize(const Vector4& vec);
		friend bool IsNormalized(const Vector4& vec);

		friend Vector4 Min(const Vector4& a, const Vector4& b);
		friend Vector4 Max(const Vector4& a, const Vector4& b);

		friend bool operator==(const Vector4& a, const Vector4& b);
		friend bool operator!=(const Vector4& a, const Vector4& b);

		friend Vector4 operator+(const Vector4& a, const Vector4& b);
		friend Vector4 operator-(const Vector4& a, const Vector4& b);
		friend Vector4 operator-(const Vector4& vec);
		friend Vector4 operator*(const Vector4& a, const Vector4& b);
		friend Vector4 operator*(const Vector4& a, float scalar);
		friend Vector4 operator*(float scalar, const Vector4& vec);
		friend Vector4 operator/(const Vector4& a, const Vector4& b);
		friend Vector4 operator/(const Vector4& a, float scalar);

		friend Vector4 operator*(const Vector4& a, const class Matrix4x4& b);
		friend Vector4 operator*(const class Matrix4x4& a, const Vector4& b);

		Vector4& operator+=(const Vector4& v);
		Vector4& operator-=(const Vector4& v);
		Vector4& operator*=(const Vector4& v);
		Vector4& operator*=(float scalar);
		Vector4& operator/=(const Vector4& v);
		Vector4& operator/=(float scalar);
	
		static const Vector4 UnitX;
		static const Vector4 UnitY;
		static const Vector4 UnitZ;
		static const Vector4 UnitW;

		static const Vector4 One;
		static const Vector4 Zero;
	};
}
