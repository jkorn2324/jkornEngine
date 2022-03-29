#include "EnginePCH.h"
#include "AssetSerializer.h"

#include "GUID.h"
#include "AssetManager.h"
#include "AssetMapper.h"

namespace Engine
{
	bool AssetSerializerFuncs::GetAssetPathFromGUID(const GUID& guid, std::filesystem::path& path)
	{
		path = AssetManager::GetAssetMapper().GetPath(guid);
		return !path.empty();
	}
	
	void AssetSerializerFuncs::SetAssetPath(const GUID& guid, const std::filesystem::path& path)
	{
		Engine::AssetManager::GetAssetMapper().SetPath(guid, path);
	}
}