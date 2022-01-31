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
	static const float FOCUSED_FAR_DISTANCE = 20.0f;
	static const float FOCUSED_NEAR_DISTANCE = 5.0f;

	static const float ROTATION_SPEED = 1.2f;

	static const float DEFAULT_SCROLL_DISTANCE = 20.0f;

	static const float SCROLL_FOCUS_MAX_DISTANCE = 13.0f;
	static const float SCROLL_FOCUS_MIN_DISTANCE = 3.0f;

	static const float MAX_SCROLL_MULTIPLIER = 30.0f;
	static const float MIN_SCROLL_MULTIPLIER = 6.0f;

	static const float CAMERA_VIEW_THRESHOLD = -0.5f;

	static const float MOUSE_CAMERA_MULTIPLIER = 0.03f;

	enum CameraType
	{
		CAMERA_PERSPECTIVE,
		CAMERA_ORTHOGRAPHIC
	};

	struct EditorCameraProperties
	{
		float nearPlane = 1.0f;
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

		MathLib::Matrix4x4 GetTransformMatrix() const;

		bool LookAt(const class MathLib::Vector3& lookAtPos,
			const class MathLib::Vector3& eyePos);

		MathLib::Vector3 GetForward() const;
		MathLib::Vector3 GetRight() const;
		MathLib::Vector3 GetUp() const;
		MathLib::Quaternion GetRotation() const;

		void ResetCamera();
		void FocusCamera();
		void MoveCamera(const MathLib::Vector3& positionDelta, bool moveFocus);
		void RotateCamera(const MathLib::Vector2& delta);
		bool ZoomCamera(float direction);

		void SetFocused(bool focused) { m_hasFocusPosition = focused; }
		bool IsFocused() const { return m_hasFocusPosition; }

	private:
		void UpdatePosition();
		void UpdateViewMatrix();


	private:
		MathLib::Vector3 m_position;
		MathLib::Vector3 m_focusPosition;
		
		float m_distanceToFocus = 1.0f;
		MathLib::Vector2 m_cameraRotation;

		EditorCameraProperties m_editorCameraProperties;

		bool m_hasFocusPosition = false;
		bool m_isFocusedFar = false;
	};
}