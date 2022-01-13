#pragma once

#include "Timestep.h"

#include "Source\Vector.h"
#include "Source\Matrix.h"

#include <entt.hpp>
#include <vector>
#include <string>
#include <functional>

namespace Engine
{
	class Entity;
	class Event;

	using EventFunc = std::function<void(Event&)>;

	class Scene
	{
	public:
		explicit Scene();
		explicit Scene(const std::wstring& name);
		~Scene();

		void OnRuntimeUpdate(const Timestep& ts);
		void OnEditorUpdate(const Timestep& ts);

		void Render();

		Entity CreateEntity(const char* entityName);
		Entity CreateEntity();
		void DestroyEntity(const Entity& entity);
		Entity Find(const std::string& entityName) const;

		class Camera* GetCamera() const;

		uint32_t GetNumEntities() const { return (uint32_t)m_entityRegistry.size(); }
		const std::wstring& GetSceneName() const;

	private:
		void BindEventFunc(const EventFunc& func);

	private:
		class Camera* m_camera;
		
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