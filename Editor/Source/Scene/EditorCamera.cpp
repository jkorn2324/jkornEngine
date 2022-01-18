#include "EditorPCH.h"
#include "EditorCamera.h"

#include "EditorUtils.h"
#include "Source\Matrix.h"

namespace Editor
{
	static const float LOOK_AT_MULTIPLIER = 3.0f;
	static const float SCROLL_POSITION_MULTIPLIER = 3.0f;

	EditorCamera::EditorCamera()
		: Camera::Camera(),
		m_editorCameraProperties(),
		m_transformMatrix(MathLib::Matrix4x4::Identity),
		m_focusPosition(MathLib::Vector3::Zero)
	{
	}

	EditorCamera::EditorCamera(const EditorCameraProperties& properties)
		: Camera::Camera(),
		m_editorCameraProperties(properties),
		m_transformMatrix(MathLib::Matrix4x4::Identity)
	{
	}
	
	void EditorCamera::OnEditorUpdate(const Engine::Timestep& ts)
	{
		HandleCameraInput(ts);
		UpdateViewMatrix();
	}

	void EditorCamera::HandleCameraInput(const Engine::Timestep& ts)
	{
		if (Engine::Input::IsKeyPressed(Engine::KEY_CODE_R))
		{
			m_transformMatrix = MathLib::Matrix4x4::Identity;
			m_position = MathLib::Vector3::Zero;
			return;
		}

		// Handles when camera looks at an entity, resets position.
		if (Engine::Input::IsKeyPressed(Engine::KEY_CODE_F)
			&& EditorSelection::HasSelectedEntity())
		{
			Engine::Entity selectedEntity = EditorSelection::GetSelectedEntity();
			if (selectedEntity.HasComponent<Engine::Transform3DComponent>())
			{
				Engine::Transform3DComponent& transform3D =
					selectedEntity.GetComponent<Engine::Transform3DComponent>();
				MathLib::Vector3 eyePos = transform3D.GetPosition() 
					- MathLib::Vector3::UnitZ * LOOK_AT_MULTIPLIER;
				m_transformMatrix = MathLib::Matrix4x4::CreateLookAt(
					transform3D.GetPosition(), eyePos, MathLib::Vector3::UnitY);
				m_position = m_transformMatrix.GetTranslation();
				return;
			}

			if (selectedEntity.HasComponent<Engine::Transform2DComponent>())
			{
				Engine::Transform2DComponent& transform2D =
					selectedEntity.GetComponent<Engine::Transform2DComponent>();
				MathLib::Vector3 lookAtPos = MathLib::Vector3(transform2D.GetPosition(), 0.0f);
				MathLib::Vector3 eyePos = lookAtPos
					- MathLib::Vector3::UnitZ * LOOK_AT_MULTIPLIER;
				m_transformMatrix = MathLib::Matrix4x4::CreateLookAt(
					lookAtPos, eyePos, MathLib::Vector3::UnitY);
				m_position = m_transformMatrix.GetTranslation();
				return;
			}
		}

		// Updates the position of the editor camera.
		{
			if (Engine::Input::IsMouseButtonHeld(
				Engine::InputMouseButton::MOUSE_BUTTON_RIGHT))
			{
				UpdateWASD(ts);
			}
			else
			{
				UpdateScrolling(ts);
			}
		}

		m_transformMatrix = MathLib::Matrix4x4::CreateTranslation(m_position);
	}

	void EditorCamera::UpdateWASD(const Engine::Timestep& ts)
	{
		bool hasInput = false;
		MathLib::Vector3 cameraInputDirection = MathLib::Vector3::Zero;
		if (Engine::Input::IsKeyHeld(Engine::KEY_CODE_W))
		{
			hasInput = true;
			cameraInputDirection += m_transformMatrix.GetZAxis();
		}
		if (Engine::Input::IsKeyHeld(Engine::KEY_CODE_A))
		{
			hasInput = true;
			cameraInputDirection += -m_transformMatrix.GetXAxis();
		}
		if (Engine::Input::IsKeyHeld(Engine::KEY_CODE_D))
		{
			hasInput = true;
			cameraInputDirection += m_transformMatrix.GetXAxis();
		}
		if (Engine::Input::IsKeyHeld(Engine::KEY_CODE_S))
		{
			hasInput = true;
			cameraInputDirection += -m_transformMatrix.GetZAxis();
		}
		if (Engine::Input::IsKeyHeld(Engine::KEY_CODE_Q))
		{
			hasInput = true;
			cameraInputDirection += -MathLib::Vector3::UnitY;
		}
		if (Engine::Input::IsKeyHeld(Engine::KEY_CODE_E))
		{
			hasInput = true;
			cameraInputDirection += MathLib::Vector3::UnitY;
		}

		if (hasInput)
		{
			cameraInputDirection.Normalize();
			// Set the position of the camera.
			m_position += cameraInputDirection *
				m_editorCameraProperties.cameraSpeed * ts.GetSeconds();
		}
	}

	void EditorCamera::UpdateScrolling(const Engine::Timestep& ts)
	{
		if (m_editorCameraProperties.cameraType == CAMERA_PERSPECTIVE)
		{
			MathLib::Vector2 scrollOffset = Engine::Input::GetMouseScrollOffset();
			bool hasInput = false;
			MathLib::Vector3 direction = MathLib::Vector3::Zero;
			if (scrollOffset.y > 0.0f)
			{
				hasInput = true;
				direction = m_transformMatrix.GetZAxis();
			}
			else if (scrollOffset.y < 0.0f)
			{
				hasInput = true;
				direction = -m_transformMatrix.GetZAxis();
			}
			if (hasInput)
			{
				m_position += direction 
					* ts.GetSeconds() * SCROLL_POSITION_MULTIPLIER;
			}
		}
	}


	void EditorCamera::UpdateViewMatrix()
	{
		// If we want to make forward x axis, left y axis, anx up z axis, need to create
		// a rotation matrix around y axis & z axis.
		m_viewMatrix = m_transformMatrix;
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