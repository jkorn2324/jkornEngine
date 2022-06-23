#pragma once

namespace Editor
{

	template<typename T>
	class TypedEditorAssetManager
	{
		// TODO: Implementation
	};

	class EditorAssetManager
	{
	public:
		static void SaveAssets();

		template<typename T>
		static void SaveAsset(const std::filesystem::path& path, T* asset)
		{
			if (asset)
			{
				// TODO: Implementation
			}
		}
	};
}