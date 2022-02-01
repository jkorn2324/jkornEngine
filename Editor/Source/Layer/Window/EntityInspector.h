#pragma once


namespace Engine
{
	class Event;
}

namespace Editor
{
	enum class ComponentSelectionType
	{
		TYPE_NONE,
		TYPE_POPUP
	};

	class EntityInspector
	{
	public:
		EntityInspector();
		~EntityInspector();

		void SetOpen(bool open) { m_open = open; }
		bool IsOpen() const { return m_open; }

		void OnEvent(Engine::Event& event);

		void Draw();

	private:
		bool m_open;
		ComponentSelectionType m_selectionType;
	};
}