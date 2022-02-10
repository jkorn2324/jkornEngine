#pragma once

#include "EditorPCH.h"
#include "CameraController.h"

namespace Editor
{
	static const float s_cameraSpeed = 20.0f;

	void CameraController::OnCreate()
	{
	}

	void CameraController::OnRuntimeUpdate(const Engine::Timestep& ts)
	{
		HandleInput();
		HandleUpdate(ts);
	}

	void CameraController::HandleInput()
	{
		m_direction = MathLib::Vector2::Zero;
		
		if (Engine::Input::IsKeyHeld(Engine::InputKeyCode::KEY_CODE_W)
			&& !Engine::Input::IsKeyHeld(Engine::InputKeyCode::KEY_CODE_S))
		{
			m_direction += MathLib::Vector2::UnitY;
		}
		if (!Engine::Input::IsKeyHeld(Engine::InputKeyCode::KEY_CODE_W)
			&& Engine::Input::IsKeyHeld(Engine::InputKeyCode::KEY_CODE_S))
		{
			m_direction -= MathLib::Vector2::UnitY;
		}
		if (!Engine::Input::IsKeyHeld(Engine::InputKeyCode::KEY_CODE_A)
			&& Engine::Input::IsKeyHeld(Engine::InputKeyCode::KEY_CODE_D))
		{
			m_direction += MathLib::Vector2::UnitX;
		}
		if (!Engine::Input::IsKeyHeld(Engine::InputKeyCode::KEY_CODE_D)
			&& Engine::Input::IsKeyHeld(Engine::InputKeyCode::KEY_CODE_A))
		{
			m_direction -= MathLib::Vector2::UnitX;
		}
	}

	void CameraController::HandleUpdate(const Engine::Timestep& ts)
	{
		Engine::Transform3DComponent& component = 
			GetEntity().GetComponent<Engine::Transform3DComponent>();

		MathLib::Vector3 pos = component.GetLocalPosition();
		pos += m_direction * ts * s_cameraSpeed;
		component.SetLocalPosition(pos);
	}
	
	void CameraController::OnDestroy()
	{
		DEBUG_LOG_EXTERNAL("Camera Controller was destroyed.");
	}
}