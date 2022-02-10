#pragma once

#include "Source\Vector.h"
#include "BehaviorScript.h"

namespace Editor
{

	class CameraController : public Engine::BehaviorScript
	{
	protected:
		void OnCreate() override;
		void OnRuntimeUpdate(const Engine::Timestep& ts) override;
		void OnDestroy() override;

	private:
		void HandleInput();
		void HandleUpdate(const Engine::Timestep& ts);

	private:
		MathLib::Vector2 m_direction;
	};
}