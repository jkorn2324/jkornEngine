#include "EnginePCH.h"
#include "AssetManager.h"

#include "Job.h"
#include "AssetMapper.h"

namespace Engine
{

	bool TypedAssetManagerFuncs::GetGUID(const std::wstring& name, GUID& guid)
	{
		return AssetManager::GetAssetMapper().GetGUID(name, guid);
	}

	void TypedAssetManagerFuncs::SetPath(const std::filesystem::path& path, const GUID& guid)
	{
		AssetManager::GetAssetMapper().SetPath(guid, path);
	}

	AssetMapper* s_assetMapper = nullptr;
	TypedAssetManager<Shader> s_shaderAssets = TypedAssetManager<Shader>();

	TypedAssetManager<Shader>& AssetManager::GetShaders()
	{
		return s_shaderAssets;
	}

	TypedAssetManager<Mesh> s_meshAssets = TypedAssetManager<Mesh>();

	TypedAssetManager<Mesh>& AssetManager::GetMeshes()
	{
		return s_meshAssets;
	}

	TypedAssetManager<Texture> s_textureAssets = TypedAssetManager<Texture>();

	void AssetManager::Init(const std::filesystem::path& guidsDatabasePath)
	{
		PROFILE_SCOPE(Init, AssetManager);

		s_assetMapper = new AssetMapper(guidsDatabasePath);
	}

	TypedAssetManager<Texture>& AssetManager::GetTextures()
	{
		return s_textureAssets;
	}

	TypedAssetManager<Material> s_materialAssets = TypedAssetManager<Material>();

	TypedAssetManager<Material>& AssetManager::GetMaterials()
	{
		return s_materialAssets;
	}

	AssetMapper& AssetManager::GetAssetMapper()
	{
		return *s_assetMapper;
	}

	void AssetManager::Release()
	{
		s_shaderAssets.Clear();
		s_textureAssets.Clear();
		s_meshAssets.Clear();
		s_materialAssets.Clear();

		// Deletes the assets.
		{
			delete s_assetMapper;
			s_assetMapper = nullptr;
		}
	}
}