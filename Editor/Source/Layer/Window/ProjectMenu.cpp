#include "EditorPCH.h"
#include "ProjectMenu.h"

#include "imgui.h"

namespace Editor
{

	static std::string s_searchedItem = "";
	// TODO: Need to find a way to set the file path

	ProjectMenu::ProjectMenu(const std::string& path)
		: m_open(true),
		m_rootPath(path),
		m_currentPath(path),
		m_selectedPathInFileMenu()
	{
	}

	void ProjectMenu::OnUpdate(const Engine::Timestep& ts)
	{
	}

	void ProjectMenu::OnEvent(Engine::Event& event)
	{
	}

	void ProjectMenu::Draw()
	{
		if (!m_open)
		{
			return;
		}

		static ImGuiWindowFlags windowFlags = 0;
		ImGui::Begin("Project Menu", &m_open, windowFlags);
		
		ImGui::BeginGroup();
		// Search Bar.
		{
			char nameOutput[50];
			std::memcpy(nameOutput, s_searchedItem.c_str(), 50);
			if (ImGui::InputText(" ", nameOutput, sizeof(nameOutput) / sizeof(char)))
			{
				s_searchedItem = nameOutput;
			}
			ImGui::Separator();
		}
		ImGui::EndGroup();

		ImGui::BeginGroup();
		{
			ImVec2 currentWindowSize = ImGui::GetWindowSize();
			currentWindowSize.x *= 0.2f;
			currentWindowSize.y = 0.0f;
			ImGui::BeginChild("files_panel", currentWindowSize, true);

			// Draws the file in the tree menu and then draws sub files.
			DrawFileInTreeMenu(m_rootPath);

			ImGui::EndChild();
		}
		ImGui::EndGroup();

		ImGui::SameLine();
		{
			ImGui::BeginChild("content_view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
			ImGui::BeginGroup();

			// Draws the files based on the current directory in content view.
			if (std::filesystem::is_directory(m_currentPath))
			{
				auto currentFiles = std::filesystem::directory_iterator(m_currentPath);
				for (const auto& entry : currentFiles)
				{
					DrawFileInContentView(entry);
				}
			}

			ImGui::EndGroup();
			ImGui::EndChild();
		}
		ImGui::End();
	}

	void ProjectMenu::DrawFileInTreeMenu(const std::filesystem::path& path)
	{
		if (!std::filesystem::is_directory(path))
		{
			return;
		}
		bool isSelected = m_selectedPathInFileMenu == path;
		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;
		if (isSelected)
		{
			nodeFlags |= ImGuiTreeNodeFlags_Selected;
		}
		nodeFlags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		if (path.empty()
			|| !Engine::FileUtils::ContainsSubdirectories(path))
		{
			nodeFlags |= ImGuiTreeNodeFlags_Leaf;
		}

		const auto& fileName = path.filename();
		bool isOpened = ImGui::TreeNodeEx(
			fileName.u8string().c_str(), nodeFlags);

		if (ImGui::IsItemClicked())
		{
			// Sets the current path.
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				m_currentPath = path;
			}
			m_selectedPathInFileMenu = path;
		}

		if (isOpened)
		{
			if (!path.empty())
			{
				auto directoryIterator = std::filesystem::directory_iterator(path);
				for (auto entry : directoryIterator)
				{
					DrawFileInTreeMenu(directoryIterator->path());
				}
			}
			ImGui::TreePop();
		}
	}
	
	void ProjectMenu::DrawFileInContentView(const std::filesystem::path& path)
	{
		// TODO: Draws the file in the content view
	}
}