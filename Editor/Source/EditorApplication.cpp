#include "Engine.h"
#include "EntryPoint.h"

#include "Application.h"

#include "EditorSceneManager.h"
#include "ProjectManager.h"
#include "EditorLayer.h"

namespace Engine
{

	class EditorApplication : public Application
	{
	public:
		EditorApplication(const std::filesystem::path& rootPath)
			: Application("Editor", rootPath)
		{
			Editor::EditorSceneManager::Init();
			Editor::ProjectManager::Init();

			AddOverlay(new Editor::EditorLayer());
		}

		~EditorApplication()
		{
			Editor::EditorSceneManager::Release();
		}
	};

	Application* Create(const ApplicationArgs& args)
	{
#if DEBUG
		std::filesystem::path editorProjectPath = "..\\EditorSandboxProject";
		if (std::filesystem::is_directory(editorProjectPath))
		{
			return new EditorApplication(editorProjectPath);
		}
#endif
		return new EditorApplication(Engine::FileUtils::GetWorkingDirectory());
	}
}
