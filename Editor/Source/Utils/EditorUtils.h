#pragma once

#include <vector>
#include <functional>

namespace Engine
{
	class Entity;
	class Event;
}

namespace Editor
{
	using EventFunc = std::function<void(Engine::Event&)>;

	class EditorSceneManager
	{
	public:
		static void Init();
		static void OnEvent(Engine::Event& event);

		static std::vector<Engine::Entity>& GetEntities();

		static bool IsPlaying();
		static void SetPlaying(bool playing);

		static bool IsPaused();
		static void SetPaused(bool paused);

		static class EditorCamera& GetEditorCamera();
	};

	class EditorSelection
	{
	public:
		static void OnEvent(Engine::Event& event);

		static Engine::Entity GetSelectedEntity();
		static void SetSelectedEntity(const Engine::Entity& entity);
		static bool HasSelectedEntity();
	};
}