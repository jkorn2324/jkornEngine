#pragma once

#include "Timestep.h"

#include "Source\Vector.h"
#include "Source\Matrix.h"

#include <entt.hpp>
#include <vector>


namespace Engine
{
	class Entity;

	struct CameraConstants
	{
		MathLib::Matrix4x4 c_viewProjection;
		MathLib::Vector3 c_cameraPosition;

		CameraConstants()
			: c_cameraPosition(MathLib::Vector3::Zero), 
			c_viewProjection(MathLib::Matrix4x4::Identity) { }

	private:
		float pad;
	};

	class Scene
	{
	public:
		explicit Scene();
		~Scene();

		void Update(const Timestep& ts);
		void Render();

		Entity CreateEntity();
		void DestroyEntity(const Entity& entity);

		class Camera* GetCamera() const;

	private:
		std::vector<entt::entity> m_markedForDestroyEntities;
		entt::registry m_entityRegistry;
		class Camera* m_camera;

		friend class Entity;
	};
}