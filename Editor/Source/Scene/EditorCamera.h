#pragma once

#include "Camera.h"
#include "Source\Vector.h"
#include "Source\Quaternion.h"
#include "Source\Matrix.h"

namespace Engine
{
	class Timestep;
	class Event;
}

namespace Editor
{
	enum CameraType
	{
		CAMERA_PERSPECTIVE,
		CAMERA_ORTHOGRAPHIC
	};

	struct EditorCameraProperties
	{
		float nearPlane = 0.0f;
		float farPlane = 10000.0f;

		float width = 800.0f;
		float height = 600.0f;

		float cameraSpeed = 60.0f;
		float fov = 100.0f;

		CameraType cameraType = CAMERA_PERSPECTIVE;
	};

	class EditorCamera : public Engine::Camera
	{
	public:
		explicit EditorCamera();
		explicit EditorCamera(const EditorCameraProperties& properties);

		void OnEditorUpdate(const class Engine::Timestep& ts);
		
		const EditorCameraProperties& GetCameraProperties() const { return m_editorCameraProperties; }
		EditorCameraProperties& GetCameraProperties() { return m_editorCameraProperties; }

		const MathLib::Matrix4x4& GetTransformMatrix() const { return m_transformMatrix; }

	private:
		void HandleCameraInput(const class Engine::Timestep& ts);
		void UpdateScrolling(const class Engine::Timestep& ts);
		void UpdateWASD(const class Engine::Timestep& ts);

		void UpdateViewMatrix();

	private:
		MathLib::Matrix4x4 m_transformMatrix;
		MathLib::Vector3 m_position;

		MathLib::Vector3 m_focusPosition;
		MathLib::Vector2 m_prevMousePos;
		
		float m_cameraYaw;
		float m_cameraPitch;

		EditorCameraProperties m_editorCameraProperties;
	};
}