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
			bool didRightClickItem = false;
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
				DrawFileInContentView(m_currentFiles[i], didRightClickItem);
				currentSize += FILE_SIZE;

				if (currentSize >= columnLength)
				{
					ImGui::NextColumn();
					currentSize = 0.0f;
				}
			}

			if (!didRightClickItem
				&& ImGui::IsItemClicked(ImGuiMouseButton_Right))
			{
				DrawFilePopup();
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

		const auto& fileName = path.filename();
		const std::string& u8FileName = fileName.u8string();
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

		bool isOpened = ImGui::TreeNodeEx(
			u8FileName.c_str(), nodeFlags);

		if (ImGui::BeginDragDropTarget())
		{
			HandleDragDropPathPayload(path);
			ImGui::EndDragDropTarget();
		}

		if (ImGui::IsItemClicked())
		{
			// Sets the current path.
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				m_currentPath = path;
			}
			m_selectedPathInFileMenu = path;
		}
		else
		{
			if (path != Engine::Application::Get().GetRootPath()
				&& ImGui::BeginDragDropSource())
			{
				const std::wstring& fpwstr = path.wstring();
				const wchar_t* filePath = fpwstr.c_str();
				ImGui::SetDragDropPayload(DRAG_DROP_FILE_PAYLOAD,
					(void*)filePath, fpwstr.size() * sizeof(wchar_t));
				ImGui::Text(u8FileName.c_str());
				ImGui::EndDragDropSource();
			}
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
	
	void ProjectMenu::DrawFileInContentView(const std::filesystem::path& path,
		bool& rightClickedItem)
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
		ImGui::Image(textureID, ImVec2(FILE_SIZE, FILE_SIZE));

		bool isPathClicked = false;
		if (ImGui::IsItemHovered())
		{
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
			{
				isPathClicked = true;
				rightClickedItem |= true;

				// TODO: Show popup menu
			}
			else if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				isPathClicked = true;
				if (isDirectory)
				{
					m_currentPath = path;
				}
				else
				{
					// Open path based on file extension.
				}
			}
		}

		if (!isPathClicked)
		{
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
			{
				const std::wstring& fpwstr = path.wstring();
				const wchar_t* filePath = fpwstr.c_str();
				ImGui::SetDragDropPayload(DRAG_DROP_FILE_PAYLOAD, 
					(void*)filePath, fpwstr.size() * sizeof(wchar_t));
				ImGui::Text(filename.c_str());
				ImGui::EndDragDropSource();
			}
		}

		if (isDirectory
			&& ImGui::BeginDragDropTarget())
		{
			HandleDragDropPathPayload(path);
			ImGui::EndDragDropTarget();
		}
		ImGui::Text(filename.c_str());
		ImGui::EndGroup();
	}

	void ProjectMenu::DrawFilePopup()
	{
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Create"))
			{
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Scene"))
					{
						// TODO: Create new scene file.
					}
					if (ImGui::MenuItem("Folder"))
					{
						// TODO: Create new folder.
					}
					// TODO: Add other assets.
					ImGui::EndPopup();
				}
			}
			ImGui::EndPopup();
		}
	}

	void ProjectMenu::HandleDragDropPathPayload(const std::filesystem::path& path)
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DRAG_DROP_FILE_PAYLOAD);
		if (payload != nullptr)
		{
			std::wstring pathAsWstring = (const wchar_t*)payload->Data;
			pathAsWstring.resize(payload->DataSize / sizeof(wchar_t));
			std::filesystem::path currentPath(pathAsWstring);
			if (currentPath != path)
			{
				std::filesystem::path newPath = path;
				newPath /= currentPath.filename();
				if (newPath != currentPath)
				{
					std::filesystem::copy(currentPath,
						newPath, std::filesystem::copy_options::recursive);
					std::filesystem::remove(currentPath);
					m_currentPath = path;
				}
			}
		}
	}
}