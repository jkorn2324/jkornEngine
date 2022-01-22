#include "EditorPCH.h"
#include "ProjectMenu.h"

#include "imgui.h"

namespace Editor
{

	static std::string s_searchedItem = "";
	// TODO: Need to find a way to set the file path

	ProjectMenu::ProjectMenu(const std::string& path)
		: m_open(true),
		m_currentPath(path)
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
			currentWindowSize.y *= 0.95f;
			ImGui::BeginChild("files_panel", currentWindowSize, true);

			// TODO: Project Menu

			ImGui::EndChild();
		}
		ImGui::EndGroup();

		ImGui::End();
	}
}