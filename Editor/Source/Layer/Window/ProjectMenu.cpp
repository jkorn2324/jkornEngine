#include "EditorPCH.h"
#include "ProjectMenu.h"

#include "imgui.h"
#include "imgui_internal.h"

namespace Editor
{

	static const char* DRAG_DROP_FILE_PAYLOAD = "_fileDragDropPayload";

	static const uint32_t NUM_FILES_PER_ROW = 5;
	static const float FILE_SIZE = 60.0f;

	static const wchar_t* g_fileIconPath = L"";
	static const wchar_t* g_folderIconPath = L"";

	static Engine::Texture* s_fileIconTexture = nullptr;
	static Engine::Texture* s_folderIconTexture = nullptr;

	static int32_t s_numProjectMenus = 0;

	static std::string s_searchedItem = "";
	// TODO: Need to find a way to set the file path


	ProjectMenu::ProjectMenu(const std::string& path)
		: m_open(true),
		m_currentPath(path),
		m_draggingPath(),
		m_selectedPathInFileMenu(),
		m_windowSize(),
		m_contentViewSize()
	{
		// Sets the window size.
		const auto& window = Engine::Application::Get().GetWindow();
		m_windowSize.x = (float)window.GetWidth();
		m_windowSize.y = (float)window.GetHeight();

		if (s_numProjectMenus <= 0)
		{
			s_fileIconTexture = Engine::Texture::StaticLoad(g_fileIconPath);
			s_folderIconTexture = Engine::Texture::StaticLoad(g_folderIconPath);
		}
		s_numProjectMenus++;
	}

	ProjectMenu::~ProjectMenu()
	{
		s_numProjectMenus--;

		if (s_numProjectMenus <= 0)
		{
			delete s_fileIconTexture;
			delete s_folderIconTexture;
		}
	}

	void ProjectMenu::OnUpdate(const Engine::Timestep& ts)
	{
		if (!s_searchedItem.empty())
		{
			// TODO: Search all paths for files.
		}
		else
		{
			m_currentFiles.clear();

			// Adds the files to the current files list, may need to iterate through
			// the directories once every couple of seconds for performance.
			{
				auto currentFileIterator = std::filesystem::directory_iterator(m_currentPath);
				for (const auto& entry : currentFileIterator)
				{
					m_currentFiles.push_back(entry);
				}
			}
		}
	}

	void ProjectMenu::OnEvent(Engine::Event& event)
	{
	}

	void ProjectMenu::Draw(const std::string& rootPath)
	{
		if (!m_open)
		{
			return;
		}

		static ImGuiWindowFlags windowFlags = 0;
		ImGui::Begin("Project Menu", &m_open, windowFlags);

		// Updates the project menu variables.
		{
			m_windowSize = *reinterpret_cast<MathLib::Vector2*>(
				&ImGui::GetWindowSize());
		}
		
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
			ImGui::BeginChild("files_panel", 
				ImVec2(m_windowSize.x * 0.2f, 0.0f), true);

			// Draws the file in the tree menu and then draws sub files.
			DrawFileInTreeMenu(rootPath);

			ImGui::EndChild();
		}
		ImGui::EndGroup();

		ImGui::SameLine();
		{
			ImGui::BeginChild("content_view",
				ImVec2(0.0f, -ImGui::GetFrameHeightWithSpacing()));
			
			// Update Content Views Window Size.
			{
				ImGuiContext* context = ImGui::GetCurrentContext();
				m_contentViewSize = *reinterpret_cast<MathLib::Vector2*>(
					&context->NextWindowData.SizeVal);
			}

			ImGui::BeginGroup();

			float columnLength = m_contentViewSize.x / FILE_SIZE;
			int numColumns = MathLib::Max(1, (int)columnLength);
			ImGui::Columns(numColumns, nullptr, false);
			
			float currentSize = 0.0f;
			for (size_t i = 0; i < m_currentFiles.size(); i++)
			{
				DrawFileInContentView(m_currentFiles[i]);
				currentSize += FILE_SIZE;

				if (currentSize >= columnLength)
				{
					ImGui::NextColumn();
					currentSize = 0.0f;
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
		const auto& filename = path.filename().u8string();
		bool isDirectory = std::filesystem::is_directory(path);
		void* textureID = nullptr;

		{
			if (isDirectory)
			{
				if (s_folderIconTexture != nullptr)
					textureID = (void*)s_folderIconTexture->GetTextureID();
			}
			else
			{
				if (s_fileIconTexture != nullptr)
				{
					textureID = (void*)s_fileIconTexture->GetTextureID();
				}
			}
		}

		ImGui::BeginGroup();

		bool buttonPressed = ImGui::ImageButton(textureID, ImVec2(FILE_SIZE, FILE_SIZE));
		if (!buttonPressed)
		{
		// Todo: Fix drag and drop.
#if 0
			if ((!std::filesystem::exists(m_draggingPath)
				|| m_draggingPath == path)
				&& ImGui::BeginDragDropSource())
			{
				m_draggingPath = path;
				void* dragDropSourceData = (void*)&path;
				ImGui::SetDragDropPayload(DRAG_DROP_FILE_PAYLOAD, dragDropSourceData, sizeof(path));
				ImGui::Text(filename.c_str());
				ImGui::EndDragDropSource();
			}

			if (isDirectory
				&& m_draggingPath != path
				&& ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* dragAndDropPayload
					= ImGui::AcceptDragDropPayload(DRAG_DROP_FILE_PAYLOAD))
				{
					m_draggingPath = std::filesystem::path();
				}
				ImGui::EndDragDropTarget();
			}
#endif
		}

		ImGui::Text(filename.c_str());

		ImGui::EndGroup();
	}
}