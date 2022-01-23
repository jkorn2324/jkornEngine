#include "EditorPCH.h"
#include "EditorCamera.h"

#include "EditorUtils.h"
#include "Source\Matrix.h"

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


	EditorCamera::EditorCamera()
		: Camera::Camera(),
		m_editorCameraProperties(),
		m_focusPosition(MathLib::Vector3::Zero),
		m_prevMousePos(MathLib::Vector2::Zero)
	{
	}

	EditorCamera::EditorCamera(const EditorCameraProperties& properties)
		: Camera::Camera(),
		m_editorCameraProperties(properties),
		m_prevMousePos(MathLib::Vector2::Zero),
		m_focusPosition(MathLib::Vector3::Zero)
	{
	}

	MathLib::Vector3 EditorCamera::GetForward() const
	{
		return MathLib::Rotate(GetRotation(), MathLib::Vector3::UnitZ);
	}

	MathLib::Vector3 EditorCamera::GetUp() const
	{
		return MathLib::Rotate(GetRotation(), MathLib::Vector3::UnitY);
	}

	MathLib::Vector3 EditorCamera::GetRight() const
	{
		return MathLib::Rotate(GetRotation(), MathLib::Vector3::UnitX);
	}

	MathLib::Quaternion EditorCamera::GetRotation() const
	{
		return MathLib::Quaternion::FromEuler(MathLib::Vector3(
			m_cameraRotation, 0.0f), false);
	}

	void EditorCamera::OnEditorUpdate(const Engine::Timestep& ts)
	{
		HandleCameraInput(ts);
		UpdateViewMatrix();

		m_prevMousePos = Engine::Input::GetMouseScreenPos();
	}

	MathLib::Matrix4x4 EditorCamera::GetTransformMatrix() const
	{
		return MathLib::Matrix4x4::CreateFromQuaternion(GetRotation())
			* MathLib::Matrix4x4::CreateTranslation(m_position);
	}

	bool EditorCamera::LookAt(const MathLib::Vector3& lookAtPos,
		const MathLib::Vector3& eyePos)
	{
		if (m_hasFocusPosition
			&& m_focusPosition == lookAtPos)
		{
			return false;
		}

		m_position = eyePos;
		m_focusPosition = lookAtPos;
		m_hasFocusPosition = true;
		m_cameraRotation = MathLib::Vector2::Zero;
		m_isFocusedFar = false;
		m_distanceToFocus = FOCUSED_NEAR_DISTANCE;
		return true;
	}

	void EditorCamera::HandleCameraInput(const Engine::Timestep& ts)
	{
		if (Engine::Input::IsKeyPressed(Engine::KEY_CODE_R))
		{
			HandleCameraReset();
			return;
		}

		// Handles when camera looks at an entity, resets position.
		if (Engine::Input::IsKeyPressed(
			Engine::KEY_CODE_F, ts.GetSeconds()))
		{
			HandleCameraFocus();
			return;
		}

		MathLib::Vector2 mouseDelta = Engine::Input::GetMouseScreenPos()
			- m_prevMousePos;

		{
			// Only moves the editor camera if the mouse button is held.
			if (Engine::Input::IsMouseButtonHeld(
				Engine::InputMouseButton::MOUSE_BUTTON_MIDDLE))
			{
				MathLib::Vector3 dir = GetRight() * -mouseDelta.x
					+ GetUp() * mouseDelta.y;
				MathLib::Vector3 directionDelta = dir * MOUSE_CAMERA_MULTIPLIER;
				m_position += directionDelta;
				m_focusPosition += directionDelta;
			}
		}


		bool handledZoom = false;
		if (Engine::Input::IsKeyHeld(
			Engine::InputKeyCode::KEY_CODE_LEFT_ALT))
		{
			float altKeyHeld = Engine::Input::GetTimeKeyHeld(
				Engine::InputKeyCode::KEY_CODE_LEFT_ALT);

			// Only updates the rotation if the left mouse button is held.
			if (Engine::Input::IsMouseButtonHeld(
				Engine::InputMouseButton::MOUSE_BUTTON_LEFT))
			{
				float leftMouseButtonTime = Engine::Input::GetMouseButtonTimeHeld(
					Engine::InputMouseButton::MOUSE_BUTTON_LEFT);
				if (altKeyHeld > leftMouseButtonTime)
				{
					UpdateCameraRotation(mouseDelta);
				}
			}

			// Only updates the zoom if the right mouse button is held.
			if (Engine::Input::IsMouseButtonHeld(
				Engine::InputMouseButton::MOUSE_BUTTON_RIGHT))
			{
				float rightMouseButtonTime = Engine::Input::GetMouseButtonTimeHeld(
					Engine::InputMouseButton::MOUSE_BUTTON_RIGHT);
				handledZoom = altKeyHeld > rightMouseButtonTime
					&& UpdateZoom(ts, mouseDelta.x * MOUSE_CAMERA_MULTIPLIER);
			}
		}
		else
		{
			if (Engine::Input::IsMouseButtonHeld(
				Engine::InputMouseButton::MOUSE_BUTTON_RIGHT))
			{
				// Updates the position of the editor camera.
				UpdateWASD(ts);
			}
		}

		// Updates the zoom if the zoom wasn't updated from the alt key.
		if (!handledZoom)
		{
			UpdateZoom(ts, Engine::Input::GetMouseScrollOffset().y);
		}

		// Updates the camera rotation.
		UpdatePosition();
	}

	bool EditorCamera::UpdateZoom(const Engine::Timestep& ts, float zoomDirection)
	{
		if (m_editorCameraProperties.cameraType == CAMERA_PERSPECTIVE)
		{
			bool hasInput = false;
			MathLib::Vector3 direction = MathLib::Vector3::Zero;

			if (zoomDirection > 0.0f)
			{
				hasInput = true;
				direction = GetForward();
			}
			else if (zoomDirection < 0.0f)
			{
				hasInput = true;
				direction = -GetForward();
			}

			if (hasInput)
			{
				if (m_hasFocusPosition)
				{
					float clampedDistance = MathLib::Clamp(SCROLL_FOCUS_MIN_DISTANCE,
						SCROLL_FOCUS_MAX_DISTANCE, m_distanceToFocus);
					clampedDistance = (clampedDistance - SCROLL_FOCUS_MIN_DISTANCE) / (SCROLL_FOCUS_MAX_DISTANCE - SCROLL_FOCUS_MIN_DISTANCE);
					float interpolatedSpeed = MathLib::Lerp(
						MIN_SCROLL_MULTIPLIER, MAX_SCROLL_MULTIPLIER, clampedDistance);
					m_distanceToFocus +=
						interpolatedSpeed * -zoomDirection * ts.GetSeconds();
				}
				else
				{
					MathLib::Vector3 difference =
						direction * ts.GetSeconds() * DEFAULT_SCROLL_DISTANCE;
					m_position += difference;
				}
				return true;
			}
		}
		return false;
	}

	void EditorCamera::HandleCameraReset()
	{
		m_position = MathLib::Vector3::Zero;
		m_focusPosition = MathLib::Vector3::Zero;
		m_hasFocusPosition = false;
		m_cameraRotation = MathLib::Vector2::Zero;
		m_distanceToFocus = FOCUSED_NEAR_DISTANCE;
		m_isFocusedFar = false;
	}

	void EditorCamera::HandleCameraFocus()
	{
		if (EditorSelection::HasSelectedEntity())
		{
			Engine::Entity selectedEntity = EditorSelection::GetSelectedEntity();
			if (selectedEntity.HasComponent<Engine::Transform3DComponent>())
			{
				Engine::Transform3DComponent& transform3D =
					selectedEntity.GetComponent<Engine::Transform3DComponent>();
				MathLib::Vector3 eyePos = transform3D.GetWorldPosition()
					- MathLib::Vector3::UnitZ * FOCUSED_NEAR_DISTANCE;
				if (LookAt(transform3D.GetWorldPosition(), eyePos))
				{
					return;
				}
			}
			if (selectedEntity.HasComponent<Engine::Transform2DComponent>())
			{
				Engine::Transform2DComponent& transform2D =
					selectedEntity.GetComponent<Engine::Transform2DComponent>();
				MathLib::Vector3 lookAtPos = MathLib::Vector3(transform2D.GetWorldPosition(), 0.0f);
				MathLib::Vector3 eyePos = lookAtPos
					- MathLib::Vector3::UnitZ * FOCUSED_NEAR_DISTANCE;
				if (LookAt(lookAtPos, eyePos))
				{
					return;
				}
			}
		}

		// If already has a focus position, just zoom in or zoom out.
		if (m_hasFocusPosition)
		{
			m_isFocusedFar = !m_isFocusedFar;
			m_distanceToFocus = m_isFocusedFar ?
				FOCUSED_FAR_DISTANCE : FOCUSED_NEAR_DISTANCE;
			return;
		}
	}

	void EditorCamera::UpdateWASD(const Engine::Timestep& ts)
	{
		bool hasInput = false;
		MathLib::Vector3 cameraInputDirection = MathLib::Vector3::Zero;
		if (Engine::Input::IsKeyHeld(Engine::KEY_CODE_W)
			&& !Engine::Input::IsKeyHeld(Engine::KEY_CODE_S))
		{
			hasInput = true;
			cameraInputDirection += GetForward();
		}
		if (Engine::Input::IsKeyHeld(Engine::KEY_CODE_S)
			&& !Engine::Input::IsKeyHeld(Engine::KEY_CODE_W))
		{
			hasInput = true;
			cameraInputDirection += -GetForward();
		}
		if (Engine::Input::IsKeyHeld(Engine::KEY_CODE_A)
			&& !Engine::Input::IsKeyHeld(Engine::KEY_CODE_D))
		{
			hasInput = true;
			cameraInputDirection += -GetRight();
		}
		if (Engine::Input::IsKeyHeld(Engine::KEY_CODE_D)
			&& !Engine::Input::IsKeyHeld(Engine::KEY_CODE_A))
		{
			hasInput = true;
			cameraInputDirection += GetRight();
		}
		if (Engine::Input::IsKeyHeld(Engine::KEY_CODE_Q)
			&& !Engine::Input::IsKeyHeld(Engine::KEY_CODE_E))
		{
			hasInput = true;
			cameraInputDirection += -MathLib::Vector3::UnitY;
		}
		if (Engine::Input::IsKeyHeld(Engine::KEY_CODE_E)
			&& !Engine::Input::IsKeyHeld(Engine::KEY_CODE_Q))
		{
			hasInput = true;
			cameraInputDirection += MathLib::Vector3::UnitY;
		}

		if (hasInput)
		{
			m_hasFocusPosition = false;
			cameraInputDirection.Normalize();
			MathLib::Vector3 difference = cameraInputDirection *
				m_editorCameraProperties.cameraSpeed * ts.GetSeconds();
			m_position += difference;
		}
	}

	void EditorCamera::UpdateCameraRotation(const MathLib::Vector2& mouseDelta)
	{
		// Perspective.
		if (m_editorCameraProperties.cameraType == CAMERA_PERSPECTIVE)
		{
			MathLib::Vector2 mouseDifference = mouseDelta * 0.003f;
			// y = Yaw
			m_cameraRotation.y +=
				mouseDifference.x * ROTATION_SPEED;
			// x = Pitch
			m_cameraRotation.x +=
				mouseDifference.y * ROTATION_SPEED;
		}
	}


	void EditorCamera::UpdatePosition()
	{
		// Updates the focus position based on the yaw and pitch.
		if (m_hasFocusPosition)
		{
			MathLib::Vector3 distanceFromFocus =
				-GetForward() * m_distanceToFocus;
			MathLib::Vector3 newPos = m_focusPosition + distanceFromFocus;
			m_position = newPos;
		}
	}

	void EditorCamera::UpdateViewMatrix()
	{
		// If we want to make forward x axis, left y axis, anx up z axis, need to create
		// a rotation matrix around y axis & z axis.
		m_viewMatrix = GetTransformMatrix();
		m_viewMatrix.Invert();

		switch (m_editorCameraProperties.cameraType)
		{
		case CAMERA_ORTHOGRAPHIC:
		{
			m_projectionMatrix = MathLib::Matrix4x4::CreateOrtho(m_editorCameraProperties.width,
				m_editorCameraProperties.height,
				m_editorCameraProperties.nearPlane,
				m_editorCameraProperties.farPlane);
			break;
		}
		case CAMERA_PERSPECTIVE:
		{
			m_projectionMatrix = MathLib::Matrix4x4::CreatePersp(m_editorCameraProperties.fov,
				m_editorCameraProperties.width,
				m_editorCameraProperties.height,
				m_editorCameraProperties.nearPlane, m_editorCameraProperties.farPlane);
			break;
		}
		}
	}
}