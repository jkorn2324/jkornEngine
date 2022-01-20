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

		bool LookAt(const class MathLib::Vector3& lookAtPos,
			const class MathLib::Vector3& eyePos);

	private:
		bool UpdateZoom(const class Engine::Timestep& ts, float zoomDirection);

		void HandleCameraInput(const class Engine::Timestep& ts);
		void UpdateWASD(const class Engine::Timestep& ts);
		void UpdateCameraRotation(const class MathLib::Vector2& mouseDelta);

		void HandleCameraReset();
		void HandleCameraFocus();

		void UpdateTransformMatrix();
		void UpdateViewMatrix();

	private:
		MathLib::Matrix4x4 m_transformMatrix;
		MathLib::Vector3 m_position;

		MathLib::Vector3 m_focusPosition;
		MathLib::Vector2 m_prevMousePos;
		
		float m_distanceToFocus = 1.0f;
		MathLib::Vector3 m_cameraRotation;

		EditorCameraProperties m_editorCameraProperties;

		bool m_hasFocusPosition = false;
		bool m_isFocusedFar = false;
	};
}