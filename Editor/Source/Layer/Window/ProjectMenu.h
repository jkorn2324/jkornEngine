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
		void DrawFileInTreeMenu(const std::filesystem::path& path);
		void DrawFileInContentView(const std::filesystem::path& path);

	private:
		std::filesystem::path m_currentPath;
		std::filesystem::path m_rootPath;
		std::filesystem::path m_selectedPathInFileMenu;

		// TODO: need to add a vector of path objects that match the search
		bool m_open;
	};
}