#include "EditorPCH.h"
#include "EditorUtils.h"

#include "EditorCamera.h"

namespace Editor
{

	static EditorCamera s_editorCamera;
	static bool s_paused = false, s_playing = false;

	static bool OnWindowSizeChanged(Engine::WindowResizedEvent& event)
	{
		return true;
	}

	void EditorSceneManager::Init() { }

	void EditorSceneManager::OnEvent(Engine::Event& event)
	{
		Engine::EventDispatcher dispatcher(event);
		dispatcher.Invoke<Engine::WindowResizedEvent>(
			BIND_STATIC_EVENT_FUNCTION(OnWindowSizeChanged));
	}
	
	bool EditorSceneManager::IsPlaying()
	{
		return s_playing;
	}

	void EditorSceneManager::SetPlaying(bool playing)
	{
		s_playing = playing;
	}

	bool EditorSceneManager::IsPaused()
	{
		return s_paused;
	}

	void EditorSceneManager::SetPaused(bool paused)
	{
		s_paused = paused;
	}
	
	EditorCamera& EditorSceneManager::GetEditorCamera()
	{
		return s_editorCamera;
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

	bool EditorSelection::HasSelectedEntity() { return s_selectedEntity.IsValid(); }

	void EditorSelection::OnEvent(Engine::Event& event)
	{
		Engine::EventDispatcher dispatcher(event);
		dispatcher.Invoke<Engine::EntityCreatedEvent>(
			BIND_STATIC_EVENT_FUNCTION(OnEntityCreated_EditorSelection));
		dispatcher.Invoke<Engine::EntityDestroyedEvent>(
			BIND_STATIC_EVENT_FUNCTION(OnEntityDestroyed_EditorSelection));
	}
}