#include "EditorPCH.h"
#include "ProjectManager.h"
#include "EditorSceneManager.h"

#include <filesystem>

namespace Editor
{

	static bool s_projectInitialized = false;

	void ProjectManager::Init()
	{
		if (s_projectInitialized) return;
		Reload();
		s_projectInitialized = true;
	}
	
	bool ProjectManager::IsInitialized() { return s_projectInitialized; }

	const std::filesystem::path ProjectManager::GetAssetsPath()
	{
		const auto& application = Engine::Application::Get();
		return application.GetRootPath() / L"Assets";
	}

	void ProjectManager::Reload()
	{
		const std::filesystem::path& rootPath = Engine::Application::Get().GetRootPath();
		if (!std::filesystem::exists(rootPath))
		{
			std::filesystem::create_directory(rootPath);
		}

		// Create Assets Folder
		{
			std::filesystem::path assetsPath = GetAssetsPath();
			if(!std::filesystem::exists(assetsPath))
			{
				std::filesystem::create_directory(assetsPath);
				// Create Default Scene.
				std::filesystem::path defaultScene = assetsPath / L"DefaultScene.scene";
				if (!std::filesystem::exists(defaultScene))
				{
					// Saves the default scene.
					EditorSceneManager::SaveScene(assetsPath);
				}
			}
		}
		InitializeProject();
	}

	void ProjectManager::InitializeProject()
	{
		// TODO: Implementation
	}
}