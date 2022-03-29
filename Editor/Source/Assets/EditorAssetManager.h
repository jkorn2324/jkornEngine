#pragma once

#include "AssetSerializer.h"
#include "AssetReferenceManager.h"

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
		static void SaveAsset(const std::filesystem::path& path, Engine::AssetRef<T>& asset)
		{
			if (asset)
			{
				Engine::AssetSerializer serializer(*asset.Get(), true);
				Engine::GUID guid;
				asset.GetGUID(guid);
				serializer.SerializeToFile(guid, path);
			}
		}
	};
}