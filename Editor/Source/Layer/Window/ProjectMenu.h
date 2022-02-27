#pragma once

#include <filesystem>

#include "Source\Vector.h"

namespace Engine
{
	class Event;
	class Timestep;
}

namespace Editor
{

	enum class ProjectMenuPopupView
	{
		Popup_None,
		Popup_ContentView,
		Popup_FileView,
		Popup_RenameFileView
	};

	class ProjectMenu
	{

	public:
		ProjectMenu(const std::string& path);
		~ProjectMenu();

		void SetOpen(bool open) { m_open = open; }
		bool IsOpen() const { return m_open; }

		void OnUpdate(const Engine::Timestep& ts);
		void OnEvent(Engine::Event& event);

		void Draw(const std::string& rootPath);

		const MathLib::Vector2& GetContentViewSize() const { return m_contentViewSize; }
		const std::filesystem::path& GetCurrentPath() const { return m_currentPath; }

	private:
		void DrawFileInTreeMenu(const std::filesystem::path& path);
		void DrawFileInContentView(const std::filesystem::path& path, bool& rightClickedItem);
		void DrawContentViewPopup();
		void DrawFilePopup(const std::filesystem::path& path);

		void HandleDragDropPathPayload(const std::filesystem::path& path);
		void OpenFileBasedOnExtension(const std::filesystem::path& selectedFile);

	private:
		std::vector<std::filesystem::path> m_currentFiles;
		MathLib::Vector2 m_windowSize;
		MathLib::Vector2 m_contentViewSize;

		std::filesystem::path m_currentPath;
		std::filesystem::path m_selectedPathInFileMenu;
		std::filesystem::path m_selectedFile;

		// TODO: need to add a vector of path objects that match the search
		bool m_open;

		ProjectMenuPopupView m_popupView = ProjectMenuPopupView::Popup_None;
	};
}