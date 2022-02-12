#pragma once

#include "BehaviorScript.h"

namespace GlfwSandbox
{

	class CameraController : public Engine::BehaviorScript
	{
	protected:
		void OnCreate() override;
		void OnRuntimeUpdate(const Engine::Timestep& ts) override;
		void OnDestroy() override;
		
		Engine::BehaviorScript* CopyTo() override;

	private:
		void HandleInput();
		void HandleUpdate(const Engine::Timestep& ts);

	private:
		MathLib::Vector2 m_direction;
	};
}