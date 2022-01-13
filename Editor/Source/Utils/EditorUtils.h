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
		static void OnEvent(Engine::Event& event);

		static std::vector<Engine::Entity>& GetEntities();
	};

	class EditorSelection
	{
	public:
		static void OnEvent(Engine::Event& event);

		static Engine::Entity GetSelectedEntity();
		static void SetSelectedEntity(const Engine::Entity& entity);
	};
}