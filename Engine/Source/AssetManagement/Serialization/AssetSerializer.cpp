#include "EnginePCH.h"
#include "AssetSerializer.h"

#include "GUID.h"
#include "AssetManager.h"
#include "AssetMapper.h"

namespace Engine
{
	
	GUID AssetSerializerFuncs::GetGUID(const std::wstring& filePath)
	{
		return AssetManager::GetAssetMapper().GetGUID(filePath);
	}
}