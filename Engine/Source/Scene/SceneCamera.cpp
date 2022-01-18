#include "EnginePCH.h"
#include "SceneCamera.h"
#include "Source\Math.h"

namespace Engine
{

	static const float DEFAULT_WIDTH = 600.0f;
	static const float DEFAULT_HEIGHT = 800.0f;

	static const float DEFAULT_NEAR_PLANE = 25.0f;
	static const float DEFAULT_FAR_PLANE = 10000.0f;

	SceneCamera::SceneCamera()
		: Camera(),
		m_cameraProperties({
			DEFAULT_NEAR_PLANE,
			DEFAULT_FAR_PLANE,
			70.0f,
			DEFAULT_HEIGHT / DEFAULT_WIDTH,
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
			DEFAULT_NEAR_PLANE,
			DEFAULT_FAR_PLANE,
			70.0f,
			DEFAULT_HEIGHT / DEFAULT_WIDTH,
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
#if 0
		static MathLib::Matrix4x4 rotation = MathLib::Matrix4x4::CreateRotationY(-MathLib::PIOVER2, false)
			* MathLib::Matrix4x4::CreateRotationZ(-MathLib::PIOVER2, false);
#else
		static MathLib::Matrix4x4 rotation = MathLib::Matrix4x4::Identity;
#endif

		switch (m_sceneCameraType)
		{
			case TYPE_ORTHOGRAPHIC:
			{
				m_projectionMatrix = MathLib::Matrix4x4::CreateOrtho(
						m_cameraProperties.orthoWidth, m_cameraProperties.orthoHeight, 
						m_cameraProperties.nearPlane, m_cameraProperties.farPlane);
				break;
			}
			case TYPE_PERSPECTIVE:
			{
				m_projectionMatrix = rotation * MathLib::Matrix4x4::CreatePersp(
					m_cameraProperties.perspFOV, m_cameraProperties.perspAspectRatio, 
					m_cameraProperties.nearPlane, m_cameraProperties.farPlane);
				break;
			}
		}
	}
}