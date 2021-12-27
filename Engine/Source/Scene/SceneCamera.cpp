#include "EnginePCH.h"
#include "SceneCamera.h"
#include "Source\Math.h"

namespace Engine
{

	static const float DEFAULT_WIDTH = 600.0f;
	static const float DEFAULT_HEIGHT = 800.0f;

	SceneCamera::SceneCamera()
		: Camera(),
		m_viewMatrix(MathLib::Matrix4x4::Identity),
		m_nearPlane(1000.0f),
		m_farPlane(-1000.0f),
		m_perspFOV(MathLib::DEG2RAD * 50.0f), 
		m_perspAspectRatio(DEFAULT_WIDTH / DEFAULT_HEIGHT),
		m_orthoWidth(DEFAULT_WIDTH),
		m_orthoHeight(DEFAULT_HEIGHT),
		m_sceneCameraType(SceneCameraType::TYPE_ORTHOGRAPHIC)
	{
		UpdateViewMatrix();
	}

	SceneCamera::SceneCamera(const SceneCameraType& type)
		: Camera(),
		m_viewMatrix(MathLib::Matrix4x4::Identity),
		m_nearPlane(1000.0f),
		m_farPlane(-1000.0f),
		m_perspFOV(MathLib::DEG2RAD * 50.0f),
		m_perspAspectRatio(DEFAULT_WIDTH / DEFAULT_HEIGHT),
		m_orthoWidth(DEFAULT_WIDTH),
		m_orthoHeight(DEFAULT_HEIGHT),
		m_sceneCameraType(type)
	{
		UpdateViewMatrix();
	}

	SceneCamera::SceneCamera(const MathLib::Matrix4x4& mat)
		: Camera(mat),
		m_viewMatrix(MathLib::Matrix4x4::Identity),
		m_nearPlane(1000.0f),
		m_farPlane(-1000.0f),
		m_perspFOV(MathLib::DEG2RAD * 50.0f),
		m_perspAspectRatio(DEFAULT_WIDTH / DEFAULT_HEIGHT),
		m_orthoWidth(DEFAULT_WIDTH),
		m_orthoHeight(DEFAULT_HEIGHT),
		m_sceneCameraType(SceneCameraType::TYPE_ORTHOGRAPHIC)
	{
		UpdateViewMatrix();
	}

	SceneCamera::SceneCamera(const SceneCameraType& type, const MathLib::Matrix4x4& mat)
		: Camera(mat),
		m_viewMatrix(MathLib::Matrix4x4::Identity),
		m_nearPlane(1000.0f),
		m_farPlane(-1000.0f),
		m_perspFOV(MathLib::DEG2RAD * 50.0f),
		m_perspAspectRatio(DEFAULT_WIDTH / DEFAULT_HEIGHT),
		m_orthoWidth(DEFAULT_WIDTH),
		m_orthoHeight(DEFAULT_HEIGHT),
		m_sceneCameraType(type)
	{
		UpdateViewMatrix();
	}

	MathLib::Matrix4x4 SceneCamera::GetViewProjectionMatrix() const
	{
		return m_projectionMatrix * m_viewMatrix;
	}

	void SceneCamera::UpdateViewMatrix()
	{
		MathLib::Matrix4x4 rotationMat = MathLib::Matrix4x4::Identity;
			// MathLib::Matrix4x4::CreateRotationX(-MathLib::PIOVER2)
			// * MathLib::Matrix4x4::CreateRotationZ(-MathLib::PIOVER2);

		switch (m_sceneCameraType)
		{
			case TYPE_ORTHOGRAPHIC:
			{
				m_viewMatrix = rotationMat * MathLib::Matrix4x4::CreateOrtho(
						m_orthoWidth, m_orthoHeight, m_nearPlane, m_farPlane);
				break;
			}
			case TYPE_PERSPECTIVE:
			{
				m_viewMatrix = rotationMat * MathLib::Matrix4x4::CreatePersp(
					m_perspFOV, m_perspAspectRatio, m_nearPlane, m_farPlane);
				break;
			}
		}
	}
}