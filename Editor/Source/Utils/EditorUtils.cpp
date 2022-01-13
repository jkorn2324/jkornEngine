#include "EditorPCH.h"
#include "EditorUtils.h"

namespace Editor
{

	static std::vector<Engine::Entity> s_sceneEntities;

	static bool OnEntityCreated_SceneManager(Engine::EntityCreatedEvent& event)
	{
		auto found = std::find(s_sceneEntities.begin(),
			s_sceneEntities.end(), event.entity);
		if (found != s_sceneEntities.end())
		{
			return true;
		}
		s_sceneEntities.push_back(event.entity);
		return true;
	}


	static bool OnEntityDestroyed_SceneManager(Engine::EntityDestroyedEvent& event)
	{
		auto found = std::find(s_sceneEntities.begin(),
			s_sceneEntities.end(), event.entity);
		if (found == s_sceneEntities.end())
		{
			return true;
		}
		s_sceneEntities.erase(found);
		return true;
	}


	void EditorSceneManager::OnEvent(Engine::Event& event)
	{
		Engine::EventDispatcher dispatcher(event);
		dispatcher.Invoke<Engine::EntityCreatedEvent>(
			BIND_STATIC_EVENT_FUNCTION(OnEntityCreated_SceneManager));
		dispatcher.Invoke<Engine::EntityDestroyedEvent>(
			BIND_STATIC_EVENT_FUNCTION(OnEntityDestroyed_SceneManager));
	}
	
	std::vector<Engine::Entity>& EditorSceneManager::GetEntities()
	{
		return s_sceneEntities;
	}


	
	static Engine::Entity s_selectedEntity;
	

	static bool OnEntityCreated_EditorSelection(Engine::EntityCreatedEvent& event)
	{
		return true;
	}

	static bool OnEntityDestroyed_EditorSelection(Engine::EntityDestroyedEvent& event)
	{
		if (event.entity == s_selectedEntity)
		{
			s_selectedEntity = event.entity;
		}
		return true;
	}

	Engine::Entity EditorSelection::GetSelectedEntity()
	{
		return s_selectedEntity;
	}

	void EditorSelection::SetSelectedEntity(const Engine::Entity& entity)
	{
		s_selectedEntity = entity;
	}

	void EditorSelection::OnEvent(Engine::Event& event)
	{
		Engine::EventDispatcher dispatcher(event);
		dispatcher.Invoke<Engine::EntityCreatedEvent>(
			BIND_STATIC_EVENT_FUNCTION(OnEntityCreated_EditorSelection));
		dispatcher.Invoke<Engine::EntityDestroyedEvent>(
			BIND_STATIC_EVENT_FUNCTION(OnEntityDestroyed_EditorSelection));
	}
}