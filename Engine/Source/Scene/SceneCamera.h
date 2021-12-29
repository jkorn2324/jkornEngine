#pragma once

#include "Source\Matrix.h"
#include "Camera.h"

namespace Engine
{

	enum SceneCameraType
	{
		TYPE_PERSPECTIVE,
		TYPE_ORTHOGRAPHIC
	};

	class SceneCamera : public Camera
	{
	public:
		explicit SceneCamera();
		explicit SceneCamera(const SceneCameraType& type);
		~SceneCamera() = default;

		void SetViewMatrix(const MathLib::Matrix4x4& mat);

	private:
		void UpdateProjectionMatrix();

	private:
		float m_nearPlane, m_farPlane;

		float m_perspFOV, m_perspAspectRatio;
		float m_orthoWidth, m_orthoHeight;

		SceneCameraType m_sceneCameraType;
	};
}