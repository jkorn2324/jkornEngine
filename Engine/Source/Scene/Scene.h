#pragma once

#include "EngineTime.h"

#include "Source\Vector.h"
#include "Source\Matrix.h"
#include "Source\Transform.h"

#include <entt.hpp>
#include <vector>
#include <string>
#include <functional>

namespace Engine
{
	class Entity;
	class Event;
	class GUID;

	template<typename T>
	class EntityComponentAddedEvent;
	template<typename T>
	class EntityComponentRemovedEvent;
	class EntityHierarchyChangedEvent;
	
	using Transform3DComponent = MathLib::Transform3D;
	using Transform2DComponent = MathLib::Transform2D;

	using EventFunc = std::function<void(Event&)>;

	class Scene
	{
	public:
		explicit Scene();
		explicit Scene(const std::wstring& name);
		~Scene();

		void OnEvent(Event& event);

		Entity CreateEntity(const GUID& guid, const std::string& entityName, const Engine::Entity& parent);
		Entity CreateEntity(const std::string& entityName, const Engine::Entity& parent);
		Entity CreateEntity(const std::string& entityName);
		Entity CreateEntity(const char* entityName);
		Entity CreateEntity(const GUID& guid);
		Entity CreateEntity(const GUID& guid, const Engine::Entity& parent);
		Entity CreateEntity(const GUID& guid, const std::string& entityName);
		Entity CreateEntity();

		void DestroyEntity(const Entity& entity);
		Entity Find(const std::string& entityName) const;
		Entity Find(const GUID& guid) const;
		Entity Find(const uint64_t& guid) const;

		class Camera* GetCamera() const;

		const std::vector<Entity>& GetRootEntities() const;
		uint32_t GetNumEntities() const { return (uint32_t)m_entityRegistry.size(); }
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

		void BindEventFunc(const EventFunc& func);

		bool OnEntityHierarchyChanged(EntityHierarchyChangedEvent& event);

		template<typename T>
		bool OnComponentAdded(EntityComponentAddedEvent<T>& component);
		template<typename T>
		bool OnComponentRemoved(EntityComponentRemovedEvent<T>& component);

	private:
		class Camera* m_camera;
		std::vector<Entity> m_rootEntities;
		std::vector<entt::entity> m_markedForDestroyEntities;
		entt::registry m_entityRegistry;
		std::wstring m_sceneName;
		EventFunc m_eventFunc;

	public:
		static void CreateDefaultScene(Scene*& scene);
		static void CreateDefaultScene(std::shared_ptr<Scene>& sharedPointer);

	private:
		static void CreateDefaultScene(Scene& scene);

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneManager;
	};
}