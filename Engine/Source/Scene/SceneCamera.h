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

	struct CameraProperties
	{
		float nearPlane;
		float farPlane;

		float perspFOV; 
		float perspAspectRatio;

		float orthoWidth;
		float orthoHeight;
	};

	class SceneCamera : public Camera
	{
	public:
		explicit SceneCamera();
		explicit SceneCamera(const SceneCameraType& type);
		explicit SceneCamera(const SceneCameraType& type,
			const CameraProperties& properties);
		~SceneCamera() = default;

		CameraProperties& GetProperties() { return m_cameraProperties; }
		const CameraProperties& GetProperties() const { return m_cameraProperties; }

		SceneCameraType GetSceneCameraType() const { return m_sceneCameraType; }
		void SetSceneCameraType(SceneCameraType cameraType) { m_sceneCameraType = cameraType; }

		void SetViewMatrix(const MathLib::Matrix4x4& mat);

	private:
		void UpdateProjectionMatrix();

	private:
		CameraProperties m_cameraProperties;
		SceneCameraType m_sceneCameraType;

		friend class Scene;
	};
}