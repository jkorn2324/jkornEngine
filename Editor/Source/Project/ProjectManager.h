#pragma once


namespace Editor
{

	class ProjectManager
	{
	public:
		static void Init();
		static void Reload();

		static bool IsInitialized();

		static const std::filesystem::path GetAssetsPath();

	private:
		static void InitializeProject();
	};
}