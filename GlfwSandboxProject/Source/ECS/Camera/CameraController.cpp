#pragma once

#include "Engine.h"
#include "CameraController.h"

namespace GlfwSandbox
{
	static const float s_cameraSpeed = 20.0f;

	namespace Camera
	{
		void ExecuteUpdate(const Engine::Timestep& ts, Engine::Entity& entity)
		{
			if (entity.HasComponent<CameraController>()
				&& entity.HasComponent<Engine::Transform3DComponent>())
			{
				CameraController& cam = entity.GetComponent<CameraController>();
				UpdateInput(cam);
				OnUpdate(ts, entity.GetComponent<Engine::Transform3DComponent>(), cam);
			}
		}

		void UpdateInput(CameraController& cameraController)
		{
			MathLib::Vector2 dir = MathLib::Vector2::Zero;
			if (Engine::Input::IsKeyHeld(Engine::InputKeyCode::KEY_CODE_W)
				&& !Engine::Input::IsKeyHeld(Engine::InputKeyCode::KEY_CODE_S))
			{
				dir += MathLib::Vector2::UnitY;
			}
			if (!Engine::Input::IsKeyHeld(Engine::InputKeyCode::KEY_CODE_W)
				&& Engine::Input::IsKeyHeld(Engine::InputKeyCode::KEY_CODE_S))
			{
				dir -= MathLib::Vector2::UnitY;
			}
			if (!Engine::Input::IsKeyHeld(Engine::InputKeyCode::KEY_CODE_A)
				&& Engine::Input::IsKeyHeld(Engine::InputKeyCode::KEY_CODE_D))
			{
				dir -= MathLib::Vector2::UnitX;
			}
			if (!Engine::Input::IsKeyHeld(Engine::InputKeyCode::KEY_CODE_D)
				&& Engine::Input::IsKeyHeld(Engine::InputKeyCode::KEY_CODE_A))
			{
				dir += MathLib::Vector2::UnitX;
			}
			cameraController.direction = dir;
		}

		void OnUpdate(const Engine::Timestep& ts, Engine::Transform3DComponent& transform3D, CameraController& cameraController)
		{
			MathLib::Vector3 pos = transform3D.GetLocalPosition();
			pos += cameraController.direction * ts * s_cameraSpeed;
			transform3D.SetLocalPosition(pos);
		}
	}
}