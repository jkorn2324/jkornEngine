#pragma once

#include <vector>
#include <functional>
#include "EntityRef.h"

namespace Engine
{
	class Entity;
	class IEvent;
}

namespace Editor
{
	using EventFunc = std::function<void(Engine::IEvent&)>;

	class EditorSelection
	{
	public:
		static void OnEvent(Engine::IEvent& event);

		static Engine::EntityRef GetSelectedEntity();
		static void SetSelectedEntity(const Engine::Entity& entity);
		static bool HasSelectedEntity();
	};
}