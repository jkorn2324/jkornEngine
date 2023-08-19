#include "EditorPCH.h"
#include "EditorSelection.h"

#include "EditorCamera.h"

namespace Editor
{

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

	bool EditorSelection::HasSelectedEntity() { return s_selectedEntity.IsValid(); }

	void EditorSelection::OnEvent(Engine::IEvent& event)
	{
		Engine::EventDispatcher dispatcher(event);
		dispatcher.Invoke<Engine::EntityEventType, Engine::EntityCreatedEvent>(
			BIND_STATIC_EVENT_FUNCTION(OnEntityCreated_EditorSelection));
		dispatcher.Invoke<Engine::EntityEventType, Engine::EntityDestroyedEvent>(
			BIND_STATIC_EVENT_FUNCTION(OnEntityDestroyed_EditorSelection));
	}
}