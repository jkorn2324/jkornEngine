#pragma once

namespace Engine
{
	class Event;
	class EntityCreatedEvent;
	class EntityDestroyedEvent;

	class Entity;
}

namespace Editor
{

	enum EntitySelectionType
	{
		TYPE_NONE,
		TYPE_VIEW_POPUP
	};

	class SceneHierarchy
	{
	public:
		SceneHierarchy();
		~SceneHierarchy();

		void SetOpen(bool open) { m_open = open; }
		bool IsOpen() const { return m_open; }
		
		void OnEvent(Engine::Event& event);
		void Draw();

	private:
		bool OnEntityDestroyed(Engine::EntityDestroyedEvent& event);
		
		void DrawEntity(const Engine::Entity& entity);

	private:
		EntitySelectionType m_selectionType = TYPE_NONE;
		bool m_open;
	};
}