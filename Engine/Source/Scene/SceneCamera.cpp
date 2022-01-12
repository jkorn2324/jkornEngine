#include "EnginePCH.h"
#include "SceneCamera.h"
#include "Source\Math.h"

namespace Engine
{

	static const float DEFAULT_WIDTH = 600.0f;
	static const float DEFAULT_HEIGHT = 800.0f;

	SceneCamera::SceneCamera()
		: Camera(),
		m_cameraProperties({
			1000.0f,
			-1000.0f,
			MathLib::DEG2RAD * 50.0f,
			DEFAULT_WIDTH / DEFAULT_HEIGHT,
			DEFAULT_WIDTH,
			DEFAULT_HEIGHT
			}),
		m_sceneCameraType(SceneCameraType::TYPE_ORTHOGRAPHIC)
	{
		UpdateProjectionMatrix();
	}

	SceneCamera::SceneCamera(const SceneCameraType& type)
		: Camera(),
		m_cameraProperties({
			1000.0f,
			-1000.0f,
			MathLib::DEG2RAD * 50.0f,
			DEFAULT_WIDTH / DEFAULT_HEIGHT,
			DEFAULT_WIDTH,
			DEFAULT_HEIGHT
		}),
		m_sceneCameraType(type)
	{
		UpdateProjectionMatrix();
	}

	SceneCamera::SceneCamera(const SceneCameraType& type, const CameraProperties& properties)
		: Camera(), 
		m_sceneCameraType(type),
		m_cameraProperties(properties)
	{
		UpdateProjectionMatrix();
	}

	void SceneCamera::SetViewMatrix(const MathLib::Matrix4x4& mat)
	{
		m_viewMatrix = mat;
	}

	void SceneCamera::UpdateProjectionMatrix()
	{
		MathLib::Matrix4x4 rotationMat = MathLib::Matrix4x4::Identity;
			// MathLib::Matrix4x4::CreateRotationX(-MathLib::PIOVER2)
			// * MathLib::Matrix4x4::CreateRotationZ(-MathLib::PIOVER2);

		switch (m_sceneCameraType)
		{
			case TYPE_ORTHOGRAPHIC:
			{
				m_projectionMatrix = rotationMat * MathLib::Matrix4x4::CreateOrtho(
						m_cameraProperties.orthoWidth, m_cameraProperties.orthoHeight, 
						m_cameraProperties.nearPlane, m_cameraProperties.farPlane);
				break;
			}
			case TYPE_PERSPECTIVE:
			{
				m_projectionMatrix = rotationMat * MathLib::Matrix4x4::CreatePersp(
					m_cameraProperties.perspFOV, m_cameraProperties.perspFOV, 
					m_cameraProperties.nearPlane, m_cameraProperties.farPlane);
				break;
			}
		}
	}
}