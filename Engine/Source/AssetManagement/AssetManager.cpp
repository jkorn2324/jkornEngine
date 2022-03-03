#include "EnginePCH.h"
#include "AssetManager.h"

#include "Job.h"
#include "AssetMapper.h"

namespace Engine
{
	// TODO: Need to support multithreading in the asset manager.


	AssetMapper* s_assetMapper = nullptr;
	AssetCache<Shader> s_shaderAssets = AssetCache<Shader>(false);

	AssetCache<Shader>& AssetManager::GetShaders()
	{
		return s_shaderAssets;
	}

	AssetCache<Mesh> s_meshAssets = AssetCache<Mesh>();

	AssetCache<Mesh>& AssetManager::GetMeshes()
	{
		return s_meshAssets;
	}

	AssetCache<Texture> s_textureAssets = AssetCache<Texture>(false);

	void AssetManager::Init(const std::filesystem::path& guidsDatabasePath)
	{
		s_assetMapper = new AssetMapper(guidsDatabasePath);
		s_assetMapper->BeginLoad();
	}

	AssetCache<Texture>& AssetManager::GetTextures()
	{
		return s_textureAssets;
	}

	AssetCache<Material> s_materialAssets = AssetCache<Material>();

	AssetCache<Material>& AssetManager::GetMaterials()
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

		// Delets the assets.
		{
			s_assetMapper->UnLoad();
			delete s_assetMapper;
			s_assetMapper = nullptr;
		}
	}
}