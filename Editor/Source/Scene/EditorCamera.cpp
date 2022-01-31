#include "EditorPCH.h"
#include "EditorCamera.h"

#include "EditorUtils.h"
#include "Source\Matrix.h"

namespace Editor
{



	EditorCamera::EditorCamera()
		: Camera::Camera(),
		m_editorCameraProperties(),
		m_focusPosition(MathLib::Vector3::Zero)
	{
	}

	EditorCamera::EditorCamera(const EditorCameraProperties& properties)
		: Camera::Camera(),
		m_editorCameraProperties(properties),
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
		UpdatePosition();
		UpdateViewMatrix();
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

	bool EditorCamera::ZoomCamera(float zoomDirection)
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
						interpolatedSpeed * -zoomDirection;
				}
				else
				{
					MoveCamera(direction * MathLib::Abs(zoomDirection)
						* DEFAULT_SCROLL_DISTANCE, false);
				}
				return true;
			}
		}
		return false;
	}

	void EditorCamera::ResetCamera()
	{
		m_position = MathLib::Vector3::Zero;
		m_focusPosition = MathLib::Vector3::Zero;
		m_hasFocusPosition = false;
		m_cameraRotation = MathLib::Vector2::Zero;
		m_distanceToFocus = FOCUSED_NEAR_DISTANCE;
		m_isFocusedFar = false;
	}

	void EditorCamera::FocusCamera()
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

	void EditorCamera::MoveCamera(const MathLib::Vector3& positionDelta, bool moveFocus)
	{
		m_position += positionDelta;
		if (moveFocus && m_hasFocusPosition) m_focusPosition += positionDelta;
	}

	void EditorCamera::RotateCamera(const MathLib::Vector2& delta)
	{
		MathLib::Vector2 mouseDifference = delta * 0.003f;
		// y = Yaw
		m_cameraRotation.y +=
			mouseDifference.x * ROTATION_SPEED;
		// x = Pitch
		m_cameraRotation.x +=
			mouseDifference.y * ROTATION_SPEED;
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