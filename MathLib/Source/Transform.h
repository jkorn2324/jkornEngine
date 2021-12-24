#pragma once

#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"

namespace MathLib
{

	class Transform2D
	{
		using Mat3x3 = MathLib::Matrix3x3;

	public:
		explicit Transform2D();

		void SetPosition(float x, float y);
		void SetPosition(const Vector2& position);
		const Vector2& GetPosition() const;

		void SetScale(float scale);
		void SetScale(float x, float y);
		void SetScale(const Vector2& scale);

		void SetRotation(float rotation, bool inDegrees = true);
		float GetRotation() const;

		void LookAt(const MathLib::Vector2& position);

		const Mat3x3& GetTransformMatrix() const;

	private:
		void RecalculateMatrix();

	private:
		Vector2 m_position;
		Vector2 m_scale;
		float m_rotation;

		Mat3x3 m_transformMatrix;
	};

	class Transform3D
	{
		using Mat4x4 = MathLib::Matrix4x4;

	public:
		explicit Transform3D();

		void SetPosition(const Vector3& pos);
		void SetPosition(float x, float y, float z);
		const Vector3& GetPosition() const;

		void SetScale(float x, float y, float z);
		void SetScale(float s);
		void SetScale(const Vector3& vec);
		const Vector3& GetScale() const;

		void SetRotation(const Vector3& eulers, bool inDegrees = true);
		void SetRotation(float yaw, float pitch, float roll, bool inDegrees = true);
		void SetRotation(const Quaternion& quat);
		const Quaternion& GetRotation() const;

		void LookAt(const MathLib::Vector3& position);

		const Mat4x4& GetTransformMatrix() const;

	private:
		void RecalculateMatrix();

	private:
		MathLib::Vector3 m_position;
		MathLib::Vector3 m_scale;
		MathLib::Quaternion m_rotation;

		Mat4x4 m_transformMatrix;
	};
}