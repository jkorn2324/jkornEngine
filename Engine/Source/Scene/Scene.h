#pragma once

#include "EngineTime.h"

#include "Source\Vector.h"
#include "Source\Matrix.h"
#include "Source\Transform.h"

#include "EntityRef.h"

#include <vector>
#include <string>
#include <functional>

namespace Engine
{
	class Entity;
	class IEvent;
	class GUID;

	class EntityHierarchyChangedEvent;

	using Transform3DComponent = MathLib::Transform3D;
	using Transform2DComponent = MathLib::Transform2D;

	class Scene
	{
	public:
		explicit Scene();
		explicit Scene(const std::wstring& name);
		~Scene();

		void OnEvent(IEvent& event);

		EntityRef CreateEntity(const GUID& guid, const std::string& entityName, const Engine::Entity& parent);
		EntityRef CreateEntity(const std::string& entityName, const Engine::Entity& parent);
		EntityRef CreateEntity(const std::string& entityName);
		EntityRef CreateEntity(const char* entityName);
		EntityRef CreateEntity(const GUID& guid);
		EntityRef CreateEntity(const GUID& guid, const Engine::Entity& parent);
		EntityRef CreateEntity(const GUID& guid, const std::string& entityName);
		EntityRef CreateEntity();

		void DestroyEntity(const Entity& entity);

		decltype(auto) CreateEntityRef(const Entity& entity) const
		{
			return TEntityRef(entity, m_entityRegistry);
		}

		decltype(auto) CreateEntityRef(const Entity& entity)
		{
			return TEntityRef(entity, m_entityRegistry);
		}

		template<typename TComponent, typename TContext, typename TFunc>
		decltype(auto) FindEntity(const TContext& context, const TFunc& func) const
		{
			const auto entityView = m_entityRegistry.view<const TComponent>();
			for (auto entity : entityView)
			{
				auto component = entityView.get<0>(entity);
				if (func(context, component))
				{
					return TEntityRef(entity, m_entityRegistry);
				}
			}
			return TEntityRef(Entity::None, m_entityRegistry);
		}

		template<typename TComponent, typename TContext, typename TFunc>
		decltype(auto) FindEntity(const TContext& context, const TFunc& func)
		{
			auto entityView = m_entityRegistry.view<const TComponent>();
			for (auto entity : entityView)
			{
				auto component = entityView.get<0>(entity);
				if (func(context, component))
				{
					return TEntityRef(entity, m_entityRegistry);
				}
			}
			return TEntityRef(Entity::None, m_entityRegistry);
		}

		class Camera* GetCamera() const;

		const std::vector<Entity>& GetRootEntities() const;
		uint32_t GetNumEntities() const { return (uint32_t)Engine::Entt::GetTotalEntities(m_entityRegistry); }
		const std::wstring& GetSceneName() const;

		template<typename ...T>
		auto GetEntitiesWithComponents() const
		{
			return m_entityRegistry.view<T...>();
		}

		Scene* CopyScene();

	private:
		void OnUpdate(const Timestep& ts);
		void OnRuntimeUpdate(const Timestep& ts);
		void OnEditorUpdate(const Timestep& ts);

		void Render(const struct CameraConstants& cameraConstants);
		void Render();

		bool OnEntityHierarchyChanged(EntityHierarchyChangedEvent& event);

	private:
		class Camera* m_camera;
		std::vector<Entity> m_rootEntities;
		std::vector<entt::entity> m_markedForDestroyEntities;
		entt::registry m_entityRegistry;
		std::wstring m_sceneName;

	public:
		static void CreateDefaultScene(Scene*& scene);
		static void CreateDefaultScene(std::shared_ptr<Scene>& sharedPointer);
		static void CreateDefaultScene(std::unique_ptr<Scene>& uniquePointer);

	private:
		static void CreateDefaultScene(Scene& scene);

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneManager;

		// TODO: Remove Update System
		template<typename... TComponents>
		friend class IUpdateSystem;
	};
}