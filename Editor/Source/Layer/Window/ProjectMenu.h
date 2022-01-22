#pragma once

#include <filesystem>

namespace Engine
{
	class Event;
	class Timestep;
}

namespace Editor
{

	class ProjectMenu
	{

	public:
		ProjectMenu(const std::string& path);

		void SetOpen(bool open) { m_open = open; }
		bool IsOpen() const { return m_open; }

		void OnUpdate(const Engine::Timestep& ts);
		void OnEvent(Engine::Event& event);

		void Draw();

	private:
		std::filesystem::path m_currentPath;
		bool m_open;
	};
}