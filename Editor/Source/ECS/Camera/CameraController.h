#pragma once

#include "Source/Vector.h"
#include "IUpdateSystem.h"

namespace Engine
{
	class Timestep;
	class Entity;
}

namespace Editor
{
	struct CameraController;

	namespace Camera
	{
		/**
		 * Executes an update for the camera.
		 */
		void ExecuteUpdate(const Engine::Timestep& ts, Engine::EntityRef& entity);

		/**
		 * Called to handle the input.
		 */
		void UpdateInput(CameraController& cameraController);

		/**
		 * Called to handle an update.
		 */
		void OnUpdate(const Engine::Timestep& ts, Engine::Transform3DComponent& transform3D, CameraController& cameraController);
		/**
		 * Called to handle an update.
		 */
		void OnUpdate(const Engine::Timestep& ts, Engine::EntityRef& entity, CameraController& cameraController);

		/**
		 * Called to handle an update.
		 */
		void OnUpdate(const Engine::Timestep& ts, Engine::EntityRef& entity);
	}

	/**
	 * The Camera Controller Component.
	 */
	struct CameraController
	{
		MathLib::Vector2 direction;
	};

	// The camera controller system.
	class CameraControllerSystem : public Engine::IUpdateSystem<CameraController, Engine::Transform3DComponent>
	{
	protected:
		void OnUpdate(const Engine::UpdateSystemContext& ctx, Engine::EntityRef& e, Components& components) override;
	};
}