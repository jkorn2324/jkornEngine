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
		explicit SceneCamera(const MathLib::Matrix4x4& mat);
		explicit SceneCamera(const SceneCameraType& type, const MathLib::Matrix4x4& mat);
		~SceneCamera() = default;

		MathLib::Matrix4x4 GetViewProjectionMatrix() const override;

	private:
		void UpdateViewMatrix();

	private:
		MathLib::Matrix4x4 m_viewMatrix;
		float m_nearPlane, m_farPlane;

		float m_perspFOV, m_perspAspectRatio;
		float m_orthoWidth, m_orthoHeight;

		SceneCameraType m_sceneCameraType;

		friend class Scene;
	};
}