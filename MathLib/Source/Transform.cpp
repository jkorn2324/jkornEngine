#include "MathPCH.h"
#include "Transform.h"
#include "MathLib.h"

namespace MathLib
{
	using Mat3x3 = MathLib::Matrix3x3;
	using Mat4x4 = MathLib::Matrix4x4;

	Transform2D::Transform2D()
		: m_position(Vector2::Zero),
		m_rotation(0.0f),
		m_scale(1.0f, 1.0f),
		m_parentTransformMatrix(Matrix4x4::Identity)
	{
	}

	Transform2D::Transform2D(const Vector2& pos,
		float rot, const Vector2& scale)
		: m_position(pos),
		m_rotation(rot),
		m_scale(scale),
		m_parentTransformMatrix(Matrix4x4::Identity)
	{
	}

	Transform2D::Transform2D(const Transform3D& transform)
		: m_position(transform.GetLocalPosition().x, transform.GetLocalPosition().y),
		m_rotation(transform.GetLocalEulerAngles(false).z),
		m_scale(transform.GetLocalScale().x, transform.GetLocalScale().y),
		m_parentTransformMatrix(transform.GetParentTransformMatrix())
	{

	}

	Transform2D& Transform2D::operator=(const Transform3D& transform)
	{
		m_position = MathLib::Vector2{ transform.GetLocalPosition().x, transform.GetLocalPosition().y };
		m_rotation = transform.GetLocalEulerAngles().z;
		m_scale = MathLib::Vector2{ transform.GetLocalScale().x, transform.GetLocalScale().y };
		m_parentTransformMatrix = transform.GetParentTransformMatrix();
		return *this;
	}

	void Transform2D::SetParentTransformMatrix(const Matrix4x4& mat)
	{
		m_parentTransformMatrix = mat;
	}

	void Transform2D::SetLocalPosition(float x, float y)
	{
		m_position.x = x;
		m_position.y = y;
	}

	void Transform2D::SetLocalPosition(const Vector2& position)
	{
		SetLocalPosition(position.x, position.y);
	}

	const Vector2& Transform2D::GetLocalPosition() const
	{
		return m_position;
	}

	void Transform2D::SetLocalScale(float scale)
	{
		SetLocalScale(scale, scale);
	}

	void Transform2D::SetLocalScale(float x, float y)
	{
		m_scale.x = x;
		m_scale.y = y;
	}

	void Transform2D::SetLocalScale(const Vector2& scale)
	{
		SetLocalScale(scale.x, scale.y);
	}

	const Vector2& Transform2D::GetLocalScale() const
	{
		return m_scale;
	}

	void Transform2D::SetLocalRotation(float rotation, bool inDegrees)
	{
		m_rotation = inDegrees ? MathLib::DEG2RAD * rotation : rotation;
	}

	float Transform2D::GetLocalRotation() const
	{
		return m_rotation;
	}

	float Transform2D::GetLocalRotation(bool inDegrees) const
	{
		return inDegrees ? MathLib::RAD2DEG * m_rotation : m_rotation;
	}

	void Transform2D::LookAt(const MathLib::Vector2& position)
	{
		Vector2 dir = Normalize(position - GetWorldPosition());
		float angle = MathLib::ATan2(dir.y, dir.x);
		m_rotation = angle;
	}

	Vector2 Transform2D::GetLocalForward() const
	{
		float x = Cos(m_rotation, false);
		float y = Sin(m_rotation, false);
		return Normalize(Vector2(x, y));
	}

	Vector2 Transform2D::GetWorldPosition() const
	{
		Vector3 worldPosition = m_parentTransformMatrix.GetTranslation();
		return Vector2(worldPosition.x, worldPosition.y)
			+ m_position;
	}

	Vector2 Transform2D::GetWorldForward() const
	{
		Vector3 parentForward = m_parentTransformMatrix.GetZAxis();
		Vector2 forwardAdded = Vector2(parentForward.x, parentForward.y)
			+ GetLocalForward();
		forwardAdded.Normalize();
		return forwardAdded;
	}

	Vector2 Transform2D::GetWorldScale() const
	{
		Vector3 scaleVec3 = m_parentTransformMatrix.GetScale();
		return Vector2(scaleVec3.x, scaleVec3.y)
			* m_scale;
	}

	Matrix4x4 Transform2D::GetTransformMatrix() const
	{
		return m_parentTransformMatrix * GetLocalTransformMatrix();
	}

	Matrix4x4 Transform2D::GetLocalTransformMatrix() const
	{
		return Matrix4x4::CreateScale(MathLib::Vector3(m_scale, 1.0f))
			* Matrix4x4::CreateRotationZ(m_rotation)
			* Matrix4x4::CreateTranslation(
				MathLib::Vector3(m_position, 0.0f));
	}

	Transform3D::Transform3D()
		: m_position(0.0f, 0.0f, 0.0f),
		m_scale(1.0f, 1.0f, 1.0f),
		m_rotator(),
		m_parentTransformMatrix(Mat4x4::Identity)
	{
	}


	Transform3D::Transform3D(const Vector3& pos,
		const Quaternion& rot, const Vector3& scale)
		: m_position(pos),
		m_scale(scale),
		m_rotator(rot),
		m_parentTransformMatrix(Mat4x4::Identity),
		m_hasParentTransformMatrix(false)
	{
	}

	Transform3D::Transform3D(const Transform2D& transform)
		: m_position(transform.GetLocalPosition(), 0.0f),
		m_scale(transform.GetLocalScale(), 1.0f),
		m_rotator(0.0f, 0.0f, transform.GetLocalRotation(false)),
		m_parentTransformMatrix(transform.GetParentTransformMatrix()),
		m_hasParentTransformMatrix(false)
	{
	}

	Transform3D& Transform3D::operator=(const Transform2D& transform)
	{
		m_position = MathLib::Vector3{ transform.GetLocalPosition(), 0.0f };
		m_scale = MathLib::Vector3{ transform.GetLocalScale(), 1.0f };
		m_rotator.eulers = MathLib::Vector3{ 0.0f, 0.0f, transform.GetLocalRotation(false) };
		m_rotator.quaternion = Quaternion::FromEuler(m_rotator.eulers, false);
		m_parentTransformMatrix = transform.GetParentTransformMatrix();
		return *this;
	}

	void Transform3D::SetParentTransformMatrix(const Matrix4x4& matrix)
	{
		m_hasParentTransformMatrix = matrix != MathLib::Matrix4x4::Identity;
		m_parentTransformMatrix = matrix;
	}

	void Transform3D::SetLocalPosition(const Vector3& pos)
	{
		SetLocalPosition(pos.x, pos.y, pos.z);
	}

	void Transform3D::SetLocalPosition(float x, float y, float z)
	{
		m_position.x = x;
		m_position.y = y;
		m_position.z = z;
	}

	const Vector3& Transform3D::GetLocalPosition() const
	{
		return m_position;
	}

	void Transform3D::SetLocalScale(float x, float y, float z)
	{
		m_scale.x = x;
		m_scale.y = y;
		m_scale.z = z;
	}

	void Transform3D::SetLocalScale(float s)
	{
		SetLocalScale(s, s, s);
	}

	void Transform3D::SetLocalScale(const Vector3& vec)
	{
		SetLocalScale(vec.x, vec.y, vec.z);
	}

	const Vector3& Transform3D::GetLocalScale() const
	{
		return m_scale;
	}

	MathLib::Matrix4x4 Transform3D::GetTransformMatrix() const
	{
		return m_parentTransformMatrix * GetLocalTransformMatrix();
	}

	Mat4x4 Transform3D::GetLocalTransformMatrix() const
	{
		return Mat4x4::CreateScale(m_scale)
			* Mat4x4::CreateFromQuaternion(m_rotator.quaternion)
			* Mat4x4::CreateTranslation(m_position);
	}
	
	void Transform3D::SetLocalEulerAngles(const Vector3& eulers, bool inDegrees)
	{
		SetLocalEulerAngles(eulers.y, eulers.x, eulers.z, inDegrees);
	}

	void Transform3D::SetLocalEulerAngles(float yaw, float pitch, float roll, bool inDegrees)
	{
		m_rotator.eulers.x = inDegrees ? MathLib::DEG2RAD * pitch : pitch;
		m_rotator.eulers.y = inDegrees ? MathLib::DEG2RAD * yaw : yaw;
		m_rotator.eulers.z = inDegrees ? MathLib::DEG2RAD * roll : roll;
		m_rotator.quaternion = Quaternion::FromEuler(m_rotator.eulers, false);
	}

	const Vector3 Transform3D::GetLocalEulerAngles(bool inDegrees) const
	{
		if (!inDegrees) return m_rotator.eulers;
		return Vector3(m_rotator.eulers.x * MathLib::RAD2DEG,
			m_rotator.eulers.y * MathLib::RAD2DEG, m_rotator.eulers.z * MathLib::RAD2DEG);
	}

	void Transform3D::SetLocalRotation(const Quaternion& quat)
	{
		m_rotator.quaternion = quat;
		m_rotator.eulers = quat.ToEuler(false);
	}

	const Quaternion& Transform3D::GetLocalRotation() const
	{
		return m_rotator.quaternion;
	}

	Vector3 Transform3D::GetWorldPosition() const
	{
		return m_parentTransformMatrix.GetTranslation() + m_position;
	}

	Vector3 Transform3D::GetWorldScale() const
	{
		return m_parentTransformMatrix.GetScale() * m_scale;
	}

	MathLib::Vector3 Transform3D::GetLocalForward() const
	{
		return Rotate(GetLocalRotation(), MathLib::Vector3::UnitZ);
	}

	MathLib::Vector3 Transform3D::GetLocalUp() const
	{
		return Rotate(GetLocalRotation(), MathLib::Vector3::UnitY);
	}

	MathLib::Vector3 Transform3D::GetLocalRight() const
	{
		return Rotate(GetLocalRotation(), MathLib::Vector3::UnitZ);
	}

	Vector3 Transform3D::GetWorldForward() const
	{
		Vector3 combinedDirections =
			m_parentTransformMatrix.GetZAxis() + GetLocalForward();
		combinedDirections.Normalize();
		return combinedDirections;
	}

	Vector3 Transform3D::GetWorldUp() const
	{
		Vector3 combinedDirections =
			m_parentTransformMatrix.GetYAxis() + GetLocalUp();
		combinedDirections.Normalize();
		return combinedDirections;
	}

	Vector3 Transform3D::GetWorldRight() const
	{
		Vector3 combinedDirections =
			m_parentTransformMatrix.GetXAxis() + GetLocalRight();
		combinedDirections.Normalize();
		return combinedDirections;
	}

	void Transform3D::LookAt(const MathLib::Vector3& position, const MathLib::Vector3& up)
	{
		Vector3 newForward = position - m_position;
		newForward.Normalize();
		Vector3 currentForward = GetWorldForward();

		Vector3 forwardNormal = Cross(newForward, currentForward);
		float forwardAngle = ACos(Dot(newForward, currentForward), false);
		float dotOfNormalAndForward = Dot(
			Cross(forwardNormal, currentForward), newForward);

		if (dotOfNormalAndForward <= 0.0f)
		{
			forwardAngle *= -1.0f;
		}
		Quaternion angleAxis = Quaternion(forwardNormal, forwardAngle);
		SetLocalRotation(Concatenate(angleAxis, GetLocalRotation()));
	}

	void Transform3D::LookAt(const MathLib::Vector3& position)
	{
		LookAt(position, Vector3::UnitY);
	}
}