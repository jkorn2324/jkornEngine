#pragma once

#include "Vector.h"
#include "Quaternion.h"


namespace MathLib
{

	class Matrix3x3
	{
	public:
		float matrix[3][3];

		explicit Matrix3x3() = default;
		explicit Matrix3x3(const float mat[3][3]);
		Matrix3x3(const Matrix3x3& mat);

		Vector2 GetTranslation() const;
		Vector2 GetScale() const;
		// Gets the x axis (Right direction).
		Vector2 GetXAxis() const;
		// Gets the y axis (Up direction).
		Vector2 GetYAxis() const;

		void Transpose();
		// Very slow func.
		void Invert();

		float* operator[](int index) const;
		friend Matrix3x3 operator*(const Matrix3x3& a, const Matrix3x3& b);
		Matrix3x3& operator*=(const Matrix3x3& mat);

		friend bool operator==(const Matrix3x3& a, const Matrix3x3& b);
		friend bool operator!=(const Matrix3x3& a, const Matrix3x3& b);

		friend Matrix3x3 Transpose(const Matrix3x3& matrix);
		friend Matrix3x3 Invert(const Matrix3x3& matrix);

		static Matrix3x3 CreateRotation(float rotation, bool inDegrees = true);
		static Matrix3x3 CreateScale(const Vector2& vec);
		static Matrix3x3 CreateScale(float x, float y);
		static Matrix3x3 CreateTranslation(const Vector2& vec);
		static Matrix3x3 CreateTranslation(float x, float y);

		static const Matrix3x3 Identity;
	};

	class Matrix4x4
	{
	public:
		float matrix[4][4];

		explicit Matrix4x4();
		explicit Matrix4x4(const float mat[4][4]);
		Matrix4x4(const Matrix4x4& mat);

		Vector3 GetScale() const;
		Vector3 GetTranslation() const;
		// Gets the x axis of the matrix (Right Direction)
		Vector3 GetXAxis() const;
		// Gets the y axis of the matrix (Up direction)
		Vector3 GetYAxis() const;
		// Gets the z axis of the matrix (Forward direction).
		Vector3 GetZAxis() const;

		void Transpose();
		void Invert();

		float* operator[](int index) const;
		friend Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b);

		Matrix4x4& operator*=(const Matrix4x4& mat);
		

		friend bool operator==(const Matrix4x4& a, const Matrix4x4& b);
		friend bool operator!=(const Matrix4x4& a, const Matrix4x4& b);

		friend Matrix4x4 Transpose(const Matrix4x4& matrix);
		friend Matrix4x4 Invert(const Matrix4x4& matrix);

		static Matrix4x4 CreateRotationX(float rotation, bool inDegrees = true);
		static Matrix4x4 CreateRotationY(float rotation, bool inDegrees = true);
		static Matrix4x4 CreateRotationZ(float rotation, bool inDegrees = true);
		static Matrix4x4 CreateEuler(const Vector3& eulers, bool inDegrees = true);
		static Matrix4x4 CreateEuler(float yaw, float pitch, float roll, bool inDegrees = true);
		static Matrix4x4 CreateFromQuaternion(const Quaternion& quaternion);
		static Matrix4x4 CreateScale(const Vector3& vec);
		static Matrix4x4 CreateScale(float x, float y, float z);
		static Matrix4x4 CreateTranslation(const Vector3& vec);
		static Matrix4x4 CreateTranslation(float x, float y, float z);

		static Matrix4x4 CreateOrtho(float left, float right, float top, float bottom, float nearPlane, float farPlane);
		static Matrix4x4 CreateOrtho(float width, float height, float nearPlane, float farPlane);

		static Matrix4x4 CreatePersp(float fov, float width, float height, float nearPlane, float farPlane, bool fovInRadians = false);
		static Matrix4x4 CreatePersp(float fov, float aspectRatio, float nearPlane, float farPlane, bool fovInRadians = false);

		static Matrix4x4 CreateLookAt(const Vector3& lookAtPosition, const Vector3& eyePos, const Vector3& upDir);

		static const Matrix4x4 Identity;
	};
}