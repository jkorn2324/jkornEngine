#include "Engine.h"
#include "EntryPoint.h"

#include "Application.h"
#include "Layer\EditorLayer.h"

namespace Engine
{

	class EditorApplication : public Application
	{
	public:
		EditorApplication(const std::string& rootPath)
			: Application("Editor", rootPath)
		{
			AddOverlay(new Editor::EditorLayer());
		}
	};

	Application* Create(const ApplicationArgs& args)
	{
#if DEBUG
		std::string editorProjectPath = "..\\EditorSandboxProject";
		if (std::filesystem::is_directory(editorProjectPath))
		{
			return new EditorApplication(editorProjectPath);
		}
#endif
		return new EditorApplication(Engine::FileUtils::GetWorkingDirectory());
	}
}
