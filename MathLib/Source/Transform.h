#pragma once

#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"
#include "MathLib.h"

namespace MathLib
{
	class Transform3D;

	class Transform2D
	{
		using Mat3x3 = MathLib::Matrix3x3;

	public:
		explicit Transform2D();
		explicit Transform2D(const Vector2& pos,
			float rot, const Vector2& scale);
		explicit Transform2D(const Transform3D& transform);

		Transform2D& operator=(const Transform3D& transform);

		void SetParentTransformMatrix(const Matrix4x4& mat);

		void SetLocalPosition(float x, float y);
		void SetLocalPosition(const Vector2& position);
		
		const Vector2& GetLocalPosition() const;

		void SetLocalScale(float scale);
		void SetLocalScale(float x, float y);
		void SetLocalScale(const Vector2& scale);
		const Vector2& GetLocalScale() const;

		void SetLocalRotation(float rotation, bool inDegrees = true);
		float GetLocalRotation() const;
		float GetLocalRotation(bool inDegrees) const;

		void LookAt(const MathLib::Vector2& position);

		Vector2 GetLocalForward() const;

		Vector2 GetWorldPosition() const;
		Vector2 GetWorldForward() const;
		Vector2 GetWorldScale() const;

		Matrix4x4 GetTransformMatrix() const;
		const Matrix4x4& GetParentTransformMatrix() const { return m_parentTransformMatrix; }
		Matrix4x4 GetLocalTransformMatrix() const;

	private:
		Vector2 m_position;
		Vector2 m_scale;
		float m_rotation;

		Matrix4x4 m_parentTransformMatrix;
	};

	class Transform3D
	{
		struct TransformRotator
		{
			Quaternion quaternion = Quaternion::Identity;
			Vector3 eulers = Vector3::Zero;

			TransformRotator() {}

			TransformRotator(const Quaternion& quat)
				: quaternion(quat), eulers(quat.ToEuler(false))
			{

			}

			TransformRotator(float yaw, float pitch, float roll, bool inDegrees = false)
				: quaternion(Quaternion::FromEuler(yaw, pitch, roll, inDegrees)),
				eulers(inDegrees ? pitch * DEG2RAD : pitch,
					inDegrees ? yaw * DEG2RAD : yaw,
					inDegrees ? roll * DEG2RAD : roll)
			{
			}
		};

		using Mat4x4 = MathLib::Matrix4x4;

	public:
		explicit Transform3D();
		explicit Transform3D(const Vector3& pos,
			const Quaternion& rot, const Vector3& scale);
		explicit Transform3D(const Transform2D& transform);

		Transform3D& operator=(const Transform2D& transform);

		void SetParentTransformMatrix(const Matrix4x4& matrix);

		void SetLocalPosition(const Vector3& pos);
		void SetLocalPosition(float x, float y, float z);
		const Vector3& GetLocalPosition() const;

		void SetLocalScale(float x, float y, float z);
		void SetLocalScale(float s);
		void SetLocalScale(const Vector3& vec);
		const Vector3& GetLocalScale() const;

		void SetLocalEulerAngles(const Vector3& eulers, bool inDegrees = true);
		void SetLocalEulerAngles(float yaw, float pitch, float roll, bool inDegrees = true);
		const Vector3 GetLocalEulerAngles(bool inDegrees = true) const;

		void SetLocalRotation(const Quaternion& quat);
		const Quaternion& GetLocalRotation() const;

		Vector3 GetWorldPosition() const;
		Vector3 GetWorldScale() const;

		Vector3 GetLocalForward() const;
		Vector3 GetLocalUp() const;
		Vector3 GetLocalRight() const;

		Vector3 GetWorldForward() const;
		Vector3 GetWorldUp() const;
		Vector3 GetWorldRight() const;

		void LookAt(const MathLib::Vector3& position, const MathLib::Vector3& up);
		void LookAt(const MathLib::Vector3& position);

		Mat4x4 GetTransformMatrix() const;
		Mat4x4 GetLocalTransformMatrix() const;

		const Mat4x4& GetParentTransformMatrix() const { return m_parentTransformMatrix; }
		bool HasParentTransformMatrix() const { return m_hasParentTransformMatrix; }

	private:
		MathLib::Vector3 m_position;
		MathLib::Vector3 m_scale;
		TransformRotator m_rotator;
		MathLib::Matrix4x4 m_parentTransformMatrix;
		bool m_hasParentTransformMatrix;
	};
}