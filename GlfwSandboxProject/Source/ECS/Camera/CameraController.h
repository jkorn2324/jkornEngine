#pragma once

#include "Engine.h"

namespace GlfwSandbox
{
	struct CameraController;

	namespace Camera
	{
		/**
		 * Executes an update for the camera.
		 */
		void ExecuteUpdate(const Engine::Timestep& ts, Engine::Entity& entity);

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
		void OnUpdate(const Engine::Timestep& ts, Engine::Entity& entity, CameraController& cameraController)
		{
			if (entity.HasComponent<Engine::Transform3DComponent>())
			{
				OnUpdate(ts, entity.GetComponent<Engine::Transform3DComponent>(), cameraController);
			}
		}
		/**
		 * Called to handle an update.
		 */
		void OnUpdate(const Engine::Timestep& ts, Engine::Entity& entity)
		{
			if (entity.HasComponent<CameraController>()
				&& entity.HasComponent<Engine::Transform3DComponent>())
			{
				OnUpdate(ts, entity.GetComponent<Engine::Transform3DComponent>(),
					entity.GetComponent<CameraController>());
			}
		}
	}

	/**
	 * The Camera Controller Component.
	 */
	struct CameraController
	{
		MathLib::Vector2 direction;
	};
}