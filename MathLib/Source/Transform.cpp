#include "MathPCH.h"
#include "Transform.h"
#include "Math.h"

namespace MathLib
{
	using Mat3x3 = MathLib::Matrix3x3;
	using Mat4x4 = MathLib::Matrix4x4;

	Transform2D::Transform2D()
		: m_position(MathLib::Vector2::Zero),
		m_rotation(0.0f),
		m_scale(1.0f, 1.0f),
		m_transformMatrix(Mat3x3::Identity)
	{
		RecalculateMatrix();
	}

	Transform2D::Transform2D(const Vector2& pos,
		float rot, const Vector2& scale)
		: m_position(pos),
		m_rotation(rot),
		m_scale(scale),
		m_transformMatrix()
	{
		RecalculateMatrix();
	}

	void Transform2D::SetPosition(float x, float y)
	{
		m_position.x = x;
		m_position.y = y;
		RecalculateMatrix();
	}

	void Transform2D::SetPosition(const Vector2& position)
	{
		SetPosition(position.x, position.y);
	}

	const Vector2& Transform2D::GetPosition() const
	{
		return m_position;
	}

	void Transform2D::SetScale(float scale)
	{
		SetScale(scale, scale);
	}

	void Transform2D::SetScale(float x, float y)
	{
		m_scale.x = x;
		m_scale.y = y;
		RecalculateMatrix();
	}

	void Transform2D::SetScale(const Vector2& scale)
	{
		SetScale(scale.x, scale.y);
	}

	const Vector2& Transform2D::GetScale() const
	{
		return m_scale;
	}

	void Transform2D::SetRotation(float rotation, bool inDegrees)
	{
		m_rotation = inDegrees ? MathLib::DEG2RAD * rotation : rotation;
		RecalculateMatrix();
	}

	float Transform2D::GetRotation() const
	{
		return m_rotation;
	}

	float Transform2D::GetRotation(bool inDegrees) const
	{
		return inDegrees ? MathLib::DEG2RAD * m_rotation : m_rotation;
	}

	const Mat3x3& Transform2D::GetTransformMatrix3x3() const
	{
		return m_transformMatrix;
	}

	Mat4x4 Transform2D::GetTransformMatrix4x4() const
	{
		return Matrix4x4::CreateScale(m_scale.x, m_scale.y, 1.0f)
			* Matrix4x4::CreateRotationX(m_rotation)
			* Matrix4x4::CreateTranslation(m_position.x, m_position.y, 0.0f);
	}

	void Transform2D::LookAt(const MathLib::Vector2& position)
	{
		Vector2 dir = Normalize(position - m_position);
		float angle = MathLib::ATan2(dir.y, dir.x);
		m_rotation = angle;
		RecalculateMatrix();
	}

	void Transform2D::RecalculateMatrix()
	{
		m_transformMatrix = Mat3x3::CreateScale(m_scale)
			* Mat3x3::CreateRotation(m_rotation)
			* Mat3x3::CreateTranslation(m_position);
	}

	Transform3D::Transform3D()
		: m_position(0.0f, 0.0f, 0.0f),
		m_scale(1.0f, 1.0f, 1.0f),
		m_rotation(MathLib::Quaternion::Identity),
		m_transformMatrix(Mat4x4::Identity)
	{
		RecalculateMatrix();
	}


	Transform3D::Transform3D(const Vector3& pos,
		const Quaternion& rot, const Vector3& scale)
		: m_position(pos),
		m_scale(scale),
		m_rotation(rot),
		m_transformMatrix(Mat4x4::Identity)
	{
		RecalculateMatrix();
	}

	void Transform3D::SetPosition(const Vector3& pos)
	{
		SetPosition(pos.x, pos.y, pos.z);
	}

	void Transform3D::SetPosition(float x, float y, float z)
	{
		m_position.x = x;
		m_position.y = y;
		m_position.z = z;
		RecalculateMatrix();
	}

	const Vector3& Transform3D::GetPosition() const
	{
		return m_position;
	}

	void Transform3D::SetScale(float x, float y, float z)
	{
		m_scale.x = x;
		m_scale.y = y;
		m_scale.z = z;
		RecalculateMatrix();
	}

	void Transform3D::SetScale(float s)
	{
		SetScale(s, s, s);
	}

	void Transform3D::SetScale(const Vector3& vec)
	{
		SetScale(vec.x, vec.y, vec.z);
	}

	const Vector3& Transform3D::GetScale() const
	{
		return m_scale;
	}

	void Transform3D::RecalculateMatrix()
	{
		m_transformMatrix = Mat4x4::CreateScale(m_scale)
			* Mat4x4::CreateFromQuaternion(m_rotation)
			* Mat4x4::CreateTranslation(m_position);
	}

	const MathLib::Matrix4x4& Transform3D::GetTransformMatrix() const
	{
		return m_transformMatrix;
	}
	
	void Transform3D::SetRotation(const Vector3& eulers, bool inDegrees)
	{
		SetRotation(Quaternion::FromEuler(eulers, inDegrees));
	}

	void Transform3D::SetRotation(float yaw, float pitch, float roll, bool inDegrees)
	{
		SetRotation(Quaternion::FromEuler(yaw, pitch, roll, inDegrees));
	}

	void Transform3D::SetRotation(const Quaternion& quat)
	{
		m_rotation = quat;
		m_rotation.Normalize();

		RecalculateMatrix();
	}

	const Quaternion& Transform3D::GetRotation() const
	{
		return m_rotation;
	}

	void Transform3D::LookAt(const MathLib::Vector3& position)
	{
		Vector3 forwardAxis = Vector3::UnitZ;
		Vector3 forward = m_transformMatrix.GetZAxis();
		Vector3 dir = Normalize(position - m_position);
		float dotForwardNDir = Dot(forward, dir);
		if (dotForwardNDir >= 1.0f)
		{
			return;
		}
		float dot = Dot(forwardAxis, dir);
		Vector3 axis = Normalize(Cross(forwardAxis, dir));
		SetRotation(Quaternion(axis, MathLib::ACos(dot, false)));
	}
}