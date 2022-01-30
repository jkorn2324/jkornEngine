#pragma once

#include "Timestep.h"

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



		Entity CreateEntity(const char* entityName);
		Entity CreateEntity();
		void DestroyEntity(const Entity& entity);
		Entity Find(const std::string& entityName) const;

		class Camera* GetCamera() const;

		const std::vector<Entity>& GetRootEntities() const;
		uint32_t GetNumEntities() const { return (uint32_t)m_entityRegistry.size(); }
		const std::wstring& GetSceneName() const;

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
		static Scene* CreateDefaultScene();

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneManager;
	};
}